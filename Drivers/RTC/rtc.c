#include "rtc.h"


//[ref]:http://openedv.com/thread-230238-1-1.html
const uint8_t mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
_calendar_obj calendar;	//RTC結構體

static uint8_t Is_Leap_Year(uint16_t year);
static uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
static uint32_t RTC_ReadTimeCounter(RTC_HandleTypeDef* hrtc);
static HAL_StatusTypeDef RTC_SetCounter(RTC_HandleTypeDef* hrtc, uint32_t TimeCounter);
static HAL_StatusTypeDef RTC_EnterInitMode(RTC_HandleTypeDef* hrtc);
static HAL_StatusTypeDef RTC_ExitInitMode(RTC_HandleTypeDef* hrtc);

void MY_RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;

	// 備份暫存器只有在當VBAT電源和VCC電源同時斷開下會被清除為零
	// 所以只有在這種狀況發生時才會重新設定一次時間，否則沿用上次設定
	if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F1) {
		//both rtc battery power and system power diconnect
		//backup valude will be clear to zero
		HAL_RTC_Init(&hrtc);
		RTC_Set(2017,9,30,17,42,00);
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F1);

	} else {
		HAL_RTC_Init(&hrtc);
	}
	RTC_Get(); // 更新時間
}

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
static uint8_t Is_Leap_Year(uint16_t year)
{
	if(year%4==0) //必须能被4整除
	{
		if(year%100==0)
		{
			if(year%400==0)
        return 1;//如果以00结尾,还要能被400整除
			else
        return 0;
		}
    else
      return 1;
	}
  else
    return 0;
}


//设置时钟
//把输入的时钟转换为秒钟
//以2000年1月1日为基准
//2000~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表
//u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表
//平年的月份日期表
//const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
HAL_StatusTypeDef RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec)
{
  uint16_t t;
	uint32_t seccount=0;
  if(syear<2000||syear>2129)
    return HAL_ERROR;
	for(t=2000;t<syear;t++)	//°把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))
      seccount+=31622400;//闰年的秒钟数
		else
      seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(uint32_t)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)
      seccount+=86400;//闰年2月份增加一天的秒钟数
	}
	seccount+=(uint32_t)(sday-1)*86400;//把前面日期的秒钟数相加
	seccount+=(uint32_t)hour*3600;//小时秒钟数
  seccount+=(uint32_t)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去
  hrtc.State = HAL_RTC_STATE_BUSY;
  __HAL_LOCK(&hrtc);

  if (RTC_SetCounter(&hrtc, seccount) != HAL_OK)
  {
    hrtc.State = HAL_RTC_STATE_ERROR;
    return HAL_ERROR;
  }
  else
  {
    hrtc.State = HAL_RTC_STATE_READY;
  }
  __HAL_UNLOCK(&hrtc);
  return HAL_OK;

}


void RTC_Get(void)
{
  static uint16_t daycnt=0;
	uint32_t timecount=0;
	uint32_t temp=0;
	uint16_t temp1=0;
  timecount=RTC_ReadTimeCounter(&hrtc);
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{
		daycnt=temp;
		temp1=2000;	//从2000年开始
		while(temp>=365)
		{
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)
          temp-=366;//闰年的秒钟数
				else
        {
          temp1++;
          break;
        }
			}
			else
        temp-=365;	  //平年
			temp1++;
		}
		calendar.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)
          temp-=29;//闰年的秒钟数
				else
          break;
			}
			else
			{
				if(temp>=mon_table[temp1])
          temp-=mon_table[temp1];//平年
				else
          break;
			}
			temp1++;
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期
	}
	temp=timecount%86400;     		//得到秒钟数
	calendar.hour=temp/3600;     	//小时
	calendar.min=(temp%3600)/60; 	//分钟
	calendar.sec=(temp%3600)%60; 	//秒钟
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//获取星期

}

//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许2000-2129年)
//输入参数：公历年月日
//返回值：星期号
static uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day)
{
  int iweek = 0;
	uint8_t y,c;
  uint8_t m,d;
  if(month == 1 || month == 2)
  {
    c = (year-1)/100;
    y = (year-1)%100;
    m = month+12;

  }
  else
  {
    c = year/100;
    y = year%100;
    m = month;
  }
  d = day;
  iweek = y +y/4 +c/4 - 2*c +26*(m+1)/10 + d - 1;
  iweek = iweek >= 0 ? (iweek%7):(iweek%7 + 7);
  if(iweek == 0)
  {
    iweek = 7;
  }
  return iweek;
}


static HAL_StatusTypeDef RTC_SetCounter(RTC_HandleTypeDef* hrtc, uint32_t TimeCounter)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Set Initialization mode */
  if(RTC_EnterInitMode(hrtc) != HAL_OK)
  {
    status = HAL_ERROR;
  }
  else
  {
    /* Set RTC COUNTER MSB word */
    WRITE_REG(hrtc->Instance->CNTH, (TimeCounter >> 16));
    /* Set RTC COUNTER LSB word */
    WRITE_REG(hrtc->Instance->CNTL, (TimeCounter & RTC_CNTL_RTC_CNT));

    /* Wait for synchro */
    if(RTC_ExitInitMode(hrtc) != HAL_OK)
    {
      status = HAL_ERROR;
    }
  }

  return status;
}
static uint32_t RTC_ReadTimeCounter(RTC_HandleTypeDef* hrtc)
{
  uint16_t high1 = 0, high2 = 0, low = 0;
  uint32_t timecounter = 0;

  high1 = READ_REG(hrtc->Instance->CNTH & RTC_CNTH_RTC_CNT);
  low   = READ_REG(hrtc->Instance->CNTL & RTC_CNTL_RTC_CNT);
  high2 = READ_REG(hrtc->Instance->CNTH & RTC_CNTH_RTC_CNT);

  if (high1 != high2)
  { /* In this case the counter roll over during reading of CNTL and CNTH registers,
    read again CNTL register then return the counter value */
    timecounter = (((uint32_t) high2 << 16 ) | READ_REG(hrtc->Instance->CNTL & RTC_CNTL_RTC_CNT));
  }
  else
  { /* No counter roll over during reading of CNTL and CNTH registers, counter
    value is equal to first value of CNTL and CNTH */
    timecounter = (((uint32_t) high1 << 16 ) | low);
  }

  return timecounter;
}


static HAL_StatusTypeDef RTC_EnterInitMode(RTC_HandleTypeDef* hrtc)
{
  uint32_t tickstart = 0;

  tickstart = HAL_GetTick();
  /* Wait till RTC is in INIT state and if Time out is reached exit */
  while((hrtc->Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET)
  {
    if((HAL_GetTick() - tickstart) >  RTC_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_DISABLE(hrtc);


  return HAL_OK;
}
static HAL_StatusTypeDef RTC_ExitInitMode(RTC_HandleTypeDef* hrtc)
{
  uint32_t tickstart = 0;

  /* Disable the write protection for RTC registers */
  __HAL_RTC_WRITEPROTECTION_ENABLE(hrtc);

  tickstart = HAL_GetTick();
  /* Wait till RTC is in INIT state and if Time out is reached exit */
  while((hrtc->Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET)
  {
    if((HAL_GetTick() - tickstart) >  RTC_TIMEOUT_VALUE)
    {
      return HAL_TIMEOUT;
    }
  }

  return HAL_OK;
}
