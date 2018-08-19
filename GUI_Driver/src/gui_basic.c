/****************************************************************************************
* 文件名：GUI_BASIC.C
* 功能：GUI基本繪圖函數。進行基本繪圖運算，並調用相應的刷新程序更新LCD顯示。
* 作者：黃紹斌
* 日期：2004.02.26
* 備註：圖形操作層，進行各種圖形運算操作。
****************************************************************************************/
#include "gui_basic.h"
#include <math.h>

/****************************************************************************
* 名稱：GUI_Rectangle()
* 功能：畫矩形。
* 入口參數： x0		矩形左上角的x坐標值
*           y0		矩形左上角的y坐標值
*           x1      矩形右下角的x坐標值
*           y1      矩形右下角的y坐標值
*           color	顯示顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_Rectangle(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, TCOLOR color)
{  
	GUI_HLine(x0, y0, x1, color);
	GUI_HLine(x0, y1, x1, color);
	GUI_RLine(x0, y0, y1, color);
	GUI_RLine(x1, y0, y1, color);
}
/****************************************************************************
* 名稱：GUI_RectangleFill()
* 功能：填充矩形。畫一個填充的矩形，填充色與邊框色一樣。
* 入口參數： x0		矩形左上角的x坐標值
*           y0		矩形左上角的y坐標值
*           x1      矩形右下角的x坐標值
*           y1      矩形右下角的y坐標值
*           color	填充顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_RectangleFill(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, TCOLOR color)
{  
	uint32_t  i;

	/* 先找出矩形左上角與右下角的兩個點，保存在(x0,y0)，(x1,y1) */
	if(x0>x1) 						// 若x0>x1，則x0與x1交換
	{  
		i = x0;
		x0 = x1;
		x1 = i;
	}
	if(y0>y1)						// 若y0>y1，則y0與y1交換
	{  
		i = y0;
		y0 = y1;
		y1 = i;
	}

	/* 判斷是否只是直線 */
	if(y0==y1) 
	{  
		GUI_HLine(x0, y0, x1, color);
		return;
	}
	if(x0==x1) 
	{ 
		GUI_RLine(x0, y0, y1, color);
		return;
	}

	while(y0<=y1)						
	{  
		GUI_HLine(x0, y0, x1, color);	// 當前畫水平線
		y0++;							// 下一行
	}
}
/****************************************************************************
* 名稱：GUI_Square()
* 功能：畫正方形。
* 入口參數： x0		正方形左上角的x坐標值
*           y0		正方形左上角的y坐標值
*           with    正方形的邊長
*           color	顯示顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_Square(uint32_t x0, uint32_t y0, uint32_t  with, TCOLOR  color)
{   
	if(with==0) return;
	if( (x0+with) > GUI_LCM_XMAX ) return;
	if( (y0+with) > GUI_LCM_YMAX ) return;
	GUI_Rectangle(x0, y0, x0+with, y0+with, color);
}
/****************************************************************************
* 名稱：GUI_Line()
* 功能：畫任意兩點之間的直線。
* 入口參數： x0		直線起點的x坐標值
*           y0		直線起點的y坐標值
*           x1      直線終點的x坐標值
*           y1      直線終點的y坐標值
*           color	顯示顏色(對於黑白色LCM，為0時滅，為1時顯示)
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_Line(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, TCOLOR color)
{  
	int32_t   dx;					// 直線x軸差值變量
	int32_t   dy;          			// 直線y軸差值變量
	int8_t    dx_sym;				// x軸增長方向，為-1時減值方向，為1時增值方向
	int8_t    dy_sym;				// y軸增長方向，為-1時減值方向，為1時增值方向
	int32_t   dx_x2;				// dx*2值變量，用於加快運算速度
	int32_t   dy_x2;				// dy*2值變量，用於加快運算速度
	int32_t   di;					// 決策變量

	dx = x1-x0;						// 求取兩點之間的差值
	dy = y1-y0;
	/* 判斷增長方向，或是否為水平線、垂直線、點 */
	if(dx>0)						// 判斷x軸方向
	{  
		dx_sym = 1;					// dx>0，設置dx_sym=1
	}
	else
	{  
		if(dx<0)
			dx_sym = -1;			// dx<0，設置dx_sym=-1
		else
		{  							// dx==0，畫垂直線，或一點
			GUI_RLine(x0, y0, y1, color);
			return;
		}
	}
	if(dy>0)						// 判斷y軸方向
	{  
		dy_sym = 1;					// dy>0，設置dy_sym=1
	}
	else
	{  
		if(dy<0)
			dy_sym = -1;			// dy<0，設置dy_sym=-1
		else
		{ 							// dy==0，畫水平線，或一點
			GUI_HLine(x0, y0, x1, color);
			return;
		}
	}
	/* 將dx、dy取絕對值 */
	dx = dx_sym * dx;
	dy = dy_sym * dy;

	/* 計算2倍的dx及dy值 */
	dx_x2 = dx*2;
	dy_x2 = dy*2;

	/* 使用Bresenham法進行畫直線 */
	if(dx>=dy)						// 對於dx>=dy，則使用x軸為基準
	{  
		di = dy_x2 - dx;
		while(x0!=x1)
		{  
			GUI_Point(x0, y0, color);
			x0 += dx_sym;
			if(di<0)
				di += dy_x2;		// 計算出下一步的決策值
			else
			{  
				di += dy_x2 - dx_x2;
				y0 += dy_sym;
			}
		}
		GUI_Point(x0, y0, color);	// 顯示最後一點
	}
	else							// 對於dx<dy，則使用y軸為基準
	{  
		di = dx_x2 - dy;
		while(y0!=y1)
		{  
			GUI_Point(x0, y0, color);
			y0 += dy_sym;
			if(di<0)
				di += dx_x2;
			else
			{  
				di += dx_x2 - dy_x2;
				x0 += dx_sym;
			}
		}
		GUI_Point(x0, y0, color);		// 顯示最後一點
	} 
}


#if	GUI_LineWith_EN==1
/****************************************************************************
* 名稱：GUI_LineWith()
* 功能：畫任意兩點之間的直線，並且可設置線的寬度。
* 入口參數： x0		直線起點的x坐標值
*           y0		直線起點的y坐標值
*           x1      直線終點的x坐標值
*           y1      直線終點的y坐標值
*           with    線寬(0-50)
*           color	顯示顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_LineWith(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint8_t with, TCOLOR color)
{  
	int32_t   dx;						// 直線x軸差值變量
	int32_t   dy;          				// 直線y軸差值變量
	int8_t    dx_sym;					// x軸增長方向，為-1時減值方向，為1時增值方向
	int8_t    dy_sym;					// y軸增長方向，為-1時減值方向，為1時增值方向
	int32_t   dx_x2;					// dx*2值變量，用於加快運算速度
	int32_t   dy_x2;					// dy*2值變量，用於加快運算速度
	int32_t   di;						// 決策變量
	int32_t   wx, wy;					// 線寬變量
	int32_t   draw_a, draw_b;

	/* 參數過濾 */
	if(with==0) return;
	if(with>50) with = 50;

	dx = x1-x0;							// 求取兩點之間的差值
	dy = y1-y0;
	wx = with/2;
	wy = with-wx-1;

	/* 判斷增長方向，或是否為水平線、垂直線、點 */
	if(dx>0)							// 判斷x軸方向
	{  
		dx_sym = 1;						// dx>0，設置dx_sym=1
	}
	else
	{  
		if(dx<0) 
			dx_sym = -1;				// dx<0，設置dx_sym=-1
		else/* dx==0，畫垂直線，或一點 */
		{  						
			wx = x0-wx;
			if(wx<0) wx = 0;
			wy = x0+wy;
			while(1)
			{  
				x0 = wx;
				GUI_RLine(x0, y0, y1, color);
				if(wx>=wy) break;
				wx++;
			}
			return;
		}
	}

	if(dy>0)							// 判斷y軸方向
		dy_sym = 1;						// dy>0，設置dy_sym=1
	else
	{  
		if(dy<0)
			dy_sym = -1;				// dy<0，設置dy_sym=-1	
		else /* dy==0，畫水平線，或一點 */
		{ 
			wx = y0-wx;
			if(wx<0) wx = 0;
			wy = y0+wy;
			while(1)
			{  
				y0 = wx;
				GUI_HLine(x0, y0, x1, color);
				if(wx>=wy) break;
				wx++;
			}
			return;
		}
	}

	/* 將dx、dy取絕對值 */
	dx = dx_sym * dx;
	dy = dy_sym * dy;

	/* 計算2倍的dx及dy值 */
	dx_x2 = dx*2;
	dy_x2 = dy*2;

	/* 使用Bresenham法進行畫直線 */
	if(dx>=dy)						// 對於dx>=dy，則使用x軸為基準
	{  
		di = dy_x2 - dx;
		while(x0!=x1)/* x軸向增長，則寬度在y方向，即畫垂直線 */
		{  
			draw_a = y0-wx;
			if(draw_a<0) draw_a = 0;
			draw_b = y0+wy;
			GUI_RLine(x0, draw_a, draw_b, color);

			x0 += dx_sym;				
			if(di<0)
				di += dy_x2;			// 計算出下一步的決策值
			else
			{  
				di += dy_x2 - dx_x2;
				y0 += dy_sym;
			}
		}
		draw_a = y0-wx;
		if(draw_a<0) draw_a = 0;
		draw_b = y0+wy;
		GUI_RLine(x0, draw_a, draw_b, color);
	}
	else								// 對於dx<dy，則使用y軸為基準
	{  
		di = dx_x2 - dy;
		while(y0!=y1) /* y軸向增長，則寬度在x方向，即畫水平線 */
		{ 
			draw_a = x0-wx;
			if(draw_a<0) draw_a = 0;
			draw_b = x0+wy;
			GUI_HLine(draw_a, y0, draw_b, color);
			y0 += dy_sym;
			if(di<0)
				di += dx_x2;
			else
			{  
				di += dx_x2 - dy_x2;
				x0 += dx_sym;
			}
		}
		draw_a = x0-wx;
		if(draw_a<0) draw_a = 0;
		draw_b = x0+wy;
		GUI_HLine(draw_a, y0, draw_b, color);
	} 
}
#endif



/****************************************************************************
* 名稱：GUI_LineS()
* 功能：多個點之間的連續連線。從第一點連到第二點，再連到第三點...
* 入口參數： points  多個點坐標數據的指針，數據排列為(x0,y0)、(x1,y1)、(x2,y2)...
*           no      點數目，至少要大於1
*           color	顯示顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_LineS(uint32_t const *points, uint8_t no, TCOLOR color)
{  
	uint32_t  x0, y0;
	uint32_t  x1, y1;
	uint8_t  i;

	/* 入口參數過濾 */
	if(0==no) return;
	if(1==no)						// 單點
	{  
		x0 = *points++;
		y0 = *points;
		GUI_Point(x0, y0, color);
	}
	/* 畫多條線條 */
	x0 = *points++;					// 取出第一點坐標值，作為原起點坐標值
	y0 = *points++;
	for(i=1; i<no; i++)
	{  
		x1 = *points++;				// 取出下一點坐標值
		y1 = *points++;
		GUI_Line(x0, y0, x1, y1, color);
		x0 = x1;						// 更新原起點坐標
		y0 = y1;
	}
}



#if  GUI_CircleX_EN==1
/****************************************************************************
* 名稱：GUI_Circle()
* 功能：指定圓心位置及半徑，畫圓。
* 入口參數： x0		圓心的x坐標值
*           y0		圓心的y坐標值
*           r       圓的半徑
*           color	顯示顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_Circle(uint32_t x0, uint32_t y0, uint32_t r, TCOLOR color)
{  
	int32_t  draw_x0, draw_y0;			// 劊圖點坐標變量
	int32_t  draw_x1, draw_y1;	
	int32_t  draw_x2, draw_y2;	
	int32_t  draw_x3, draw_y3;	
	int32_t  draw_x4, draw_y4;	
	int32_t  draw_x5, draw_y5;	
	int32_t  draw_x6, draw_y6;	
	int32_t  draw_x7, draw_y7;	
	int32_t  xx, yy;					// 畫圓控制變量
	int32_t  di;						// 決策變量

	/* 參數過濾 */
	if(0==r) return;

	/* 計算出8個特殊點(0、45、90、135、180、225、270度)，進行顯示 */
	draw_x0 = draw_x1 = x0;
	draw_y0 = draw_y1 = y0 + r;
	if(draw_y0<GUI_LCM_YMAX) GUI_Point(draw_x0, draw_y0, color);	// 90度

	draw_x2 = draw_x3 = x0;
	draw_y2 = draw_y3 = y0 - r;
	if(draw_y2>=0) GUI_Point(draw_x2, draw_y2, color);			// 270度


	draw_x4 = draw_x6 = x0 + r;
	draw_y4 = draw_y6 = y0;
	if(draw_x4<GUI_LCM_XMAX) GUI_Point(draw_x4, draw_y4, color);	// 0度

	draw_x5 = draw_x7 = x0 - r;
	draw_y5 = draw_y7 = y0;
	if(draw_x5>=0) GUI_Point(draw_x5, draw_y5, color);			// 180度   
	if(1==r) return;					// 若半徑為1，則已圓畫完
   
   
	/* 使用Bresenham法進行畫圓 */
	di = 3 - 2*r;					// 初始化決策變量

	xx = 0;
	yy = r;	
	while(xx<yy)
	{  
		if(di<0)
			di += 4*xx + 6;	      
		else
		{  
			di += 4*(xx - yy) + 10;
			yy--;	  
			draw_y0--;
			draw_y1--;
			draw_y2++;
			draw_y3++;
			draw_x4--;
			draw_x5++;
			draw_x6--;
			draw_x7++;	 	
		}
		xx++;   
		draw_x0++;
		draw_x1--;
		draw_x2++;
		draw_x3--;
		draw_y4++;
		draw_y5++;
		draw_y6--;
		draw_y7--;

		/* 要判斷當前點是否在有效範圍內 */
		if( (draw_x0<=GUI_LCM_XMAX)&&(draw_y0>=0) )	
			GUI_Point(draw_x0, draw_y0, color);

		if( (draw_x1>=0)&&(draw_y1>=0) )	
			GUI_Point(draw_x1, draw_y1, color);

		if( (draw_x2<=GUI_LCM_XMAX)&&(draw_y2<=GUI_LCM_YMAX) )	
			GUI_Point(draw_x2, draw_y2, color);   

		if( (draw_x3>=0)&&(draw_y3<=GUI_LCM_YMAX) )	
			GUI_Point(draw_x3, draw_y3, color);

		if( (draw_x4<=GUI_LCM_XMAX)&&(draw_y4>=0) )	
			GUI_Point(draw_x4, draw_y4, color);

		if( (draw_x5>=0)&&(draw_y5>=0) )	
			GUI_Point(draw_x5, draw_y5, color);

		if( (draw_x6<=GUI_LCM_XMAX)&&(draw_y6<=GUI_LCM_YMAX) )	
			GUI_Point(draw_x6, draw_y6, color);

		if( (draw_x7>=0)&&(draw_y7<=GUI_LCM_YMAX) )	
			GUI_Point(draw_x7, draw_y7, color);
	}
}


/****************************************************************************
* 名稱：GUI_CircleFill()
* 功能：指定圓心位置及半徑，畫圓並填充，填充色與邊框色一樣。
* 入口參數： x0		圓心的x坐標值
*           y0		圓心的y坐標值
*           r       圓的半徑
*           color	填充顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_CircleFill(uint32_t x0, uint32_t y0, uint32_t r, TCOLOR color)
{  
	int32_t  draw_x0, draw_y0;			// 劊圖點坐標變量
	int32_t  draw_x1, draw_y1;	
	int32_t  draw_x2, draw_y2;	
	int32_t  draw_x3, draw_y3;	
	int32_t  draw_x4, draw_y4;	
	int32_t  draw_x5, draw_y5;	
	int32_t  draw_x6, draw_y6;	
	int32_t  draw_x7, draw_y7;	
	int32_t  fill_x0, fill_y0;			// 填充所需的變量，使用垂直線填充
	int32_t  fill_x1;
	int32_t  xx, yy;					// 畫圓控制變量
	int32_t  di;						// 決策變量

	/* 參數過濾 */
	if(0==r) return;

	/* 計算出4個特殊點(0、90、180、270度)，進行顯示 */
	draw_x0 = draw_x1 = x0;
	draw_y0 = draw_y1 = y0 + r;
	if(draw_y0<GUI_LCM_YMAX)
		GUI_Point(draw_x0, draw_y0, color);	// 90度

	draw_x2 = draw_x3 = x0;
	draw_y2 = draw_y3 = y0 - r;
	if(draw_y2>=0)
		GUI_Point(draw_x2, draw_y2, color);	// 270度

	draw_x4 = draw_x6 = x0 + r;
	draw_y4 = draw_y6 = y0;
	if(draw_x4<GUI_LCM_XMAX) 
	{  
		GUI_Point(draw_x4, draw_y4, color);	// 0度
		fill_x1 = draw_x4;
	}
	else
		fill_x1 = GUI_LCM_XMAX;
	
	fill_y0 = y0;							// 設置填充線條起始點fill_x0
	fill_x0 = x0 - r;						// 設置填充線條結束點fill_y1
	if(fill_x0<0) fill_x0 = 0;
	GUI_HLine(fill_x0, fill_y0, fill_x1, color);

	draw_x5 = draw_x7 = x0 - r;
	draw_y5 = draw_y7 = y0;
	if(draw_x5>=0) 
		GUI_Point(draw_x5, draw_y5, color);	// 180度
	if(1==r) return;


	/* 使用Bresenham法進行畫圓 */
	di = 3 - 2*r;							// 初始化決策變量

	xx = 0;
	yy = r;
	while(xx<yy)
	{  
		if(di<0)
			di += 4*xx + 6;
		else
		{  
			di += 4*(xx - yy) + 10;
			yy--;	  
			draw_y0--;
			draw_y1--;
			draw_y2++;
			draw_y3++;
			draw_x4--;
			draw_x5++;
			draw_x6--;
			draw_x7++;		 
		}
		xx++;   
		draw_x0++;
		draw_x1--;
		draw_x2++;
		draw_x3--;
		draw_y4++;
		draw_y5++;
		draw_y6--;
		draw_y7--;
		
		/* 要判斷當前點是否在有效範圍內 */
		if( (draw_x0<=GUI_LCM_XMAX)&&(draw_y0>=0) )	
			GUI_Point(draw_x0, draw_y0, color);

		if( (draw_x1>=0)&&(draw_y1>=0) )	
			GUI_Point(draw_x1, draw_y1, color);
	  
		/* 第二點水直線填充(下半圓的點) */
		if(draw_x1>=0)
		{  /* 設置填充線條起始點fill_x0 */
			fill_x0 = draw_x1;
			/* 設置填充線條起始點fill_y0 */
			fill_y0 = draw_y1;
			if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
			if(fill_y0<0) fill_y0 = 0; 
			/* 設置填充線條結束點fill_x1 */									
			fill_x1 = x0*2 - draw_x1;				
			if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;
			GUI_HLine(fill_x0, fill_y0, fill_x1, color);
		}
	  
		if( (draw_x2<=GUI_LCM_XMAX)&&(draw_y2<=GUI_LCM_YMAX) )	
			GUI_Point(draw_x2, draw_y2, color);   
		  
		if( (draw_x3>=0)&&(draw_y3<=GUI_LCM_YMAX) )	
			GUI_Point(draw_x3, draw_y3, color);
	  
		/* 第四點垂直線填充(上半圓的點) */
		if(draw_x3>=0)
		{  /* 設置填充線條起始點fill_x0 */
			fill_x0 = draw_x3;
			/* 設置填充線條起始點fill_y0 */
			fill_y0 = draw_y3;
			if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
			if(fill_y0<0) fill_y0 = 0;
			/* 設置填充線條結束點fill_x1 */									
			fill_x1 = x0*2 - draw_x3;				
			if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;
			GUI_HLine(fill_x0, fill_y0, fill_x1, color);
		}	 
		if( (draw_x4<=GUI_LCM_XMAX)&&(draw_y4>=0) )	
			GUI_Point(draw_x4, draw_y4, color);

		if( (draw_x5>=0)&&(draw_y5>=0) )	
			GUI_Point(draw_x5, draw_y5, color);
	  
		/* 第六點垂直線填充(上半圓的點) */
		if(draw_x5>=0)
		{  /* 設置填充線條起始點fill_x0 */
			fill_x0 = draw_x5;
			/* 設置填充線條起始點fill_y0 */
			fill_y0 = draw_y5;
			if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
			if(fill_y0<0) fill_y0 = 0;
			/* 設置填充線條結束點fill_x1 */									
			fill_x1 = x0*2 - draw_x5;				
			if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;
			GUI_HLine(fill_x0, fill_y0, fill_x1, color);
		}

		if( (draw_x6<=GUI_LCM_XMAX)&&(draw_y6<=GUI_LCM_YMAX) )	
			GUI_Point(draw_x6, draw_y6, color);
		if( (draw_x7>=0)&&(draw_y7<=GUI_LCM_YMAX) )	
			GUI_Point(draw_x7, draw_y7, color);

		/* 第八點垂直線填充(上半圓的點) */
		if(draw_x7>=0)
		{  /* 設置填充線條起始點fill_x0 */
			fill_x0 = draw_x7;
			/* 設置填充線條起始點fill_y0 */
			fill_y0 = draw_y7;
			if(fill_y0>GUI_LCM_YMAX) fill_y0 = GUI_LCM_YMAX;
			if(fill_y0<0) fill_y0 = 0;
			/* 設置填充線條結束點fill_x1 */									
			fill_x1 = x0*2 - draw_x7;				
			if(fill_x1>GUI_LCM_XMAX) fill_x1 = GUI_LCM_XMAX;
			GUI_HLine(fill_x0, fill_y0, fill_x1, color);
		}
	}
}
#endif

#if	 GUI_EllipseX_EN==1
/****************************************************************************
* 名稱：GUI_Ellipse()
* 功能：畫正橢圓。給定橢圓的四個點的參數，最左、最右點的x軸坐標值為x0、x1，最上、最下點
*      的y軸坐標為y0、y1。
* 入口參數： x0		最左點的x坐標值
*           x1		最右點的x坐標值
*           y0		最上點的y坐標值
*           y1      最下點的y坐標值
*           color	顯示顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_Ellipse(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1, TCOLOR color)
{  
	int32_t  draw_x0, draw_y0;			// 劊圖點坐標變量
	int32_t  draw_x1, draw_y1;
	int32_t  draw_x2, draw_y2;
	int32_t  draw_x3, draw_y3;
	int32_t  xx, yy;					// 畫圖控制變量

	int32_t  center_x, center_y;		// 橢圓中心點坐標變量
	int32_t  radius_x, radius_y;		// 橢圓的半徑，x軸半徑和y軸半徑
	int32_t  radius_xx, radius_yy;		// 半徑乘平方值
	int32_t  radius_xx2, radius_yy2;	// 半徑乘平方值的兩倍
	int32_t  di;						// 定義決策變量
	
	/* 參數過濾 */
	if( (x0==x1) || (y0==y1) ) return;

	/* 計算出橢圓中心點坐標 */
	center_x = (x0 + x1) >> 1;			
	center_y = (y0 + y1) >> 1;

	/* 計算出橢圓的半徑，x軸半徑和y軸半徑 */
	if(x0 > x1)
		radius_x = (x0 - x1) >> 1;
	else
		radius_x = (x1 - x0) >> 1;
	if(y0 > y1)
		radius_y = (y0 - y1) >> 1;
	else
		radius_y = (y1 - y0) >> 1;
   
	/* 計算半徑平方值 */
	radius_xx = radius_x * radius_x;
	radius_yy = radius_y * radius_y;

	/* 計算半徑平方值乘2值 */
	radius_xx2 = radius_xx<<1;
	radius_yy2 = radius_yy<<1;

	/* 初始化畫圖變量 */
	xx = 0;
	yy = radius_y;
	di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// 初始化決策變量 

	/* 計算出橢圓y軸上的兩個端點坐標，作為作圖起點 */
	draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
	draw_y0 = draw_y1 = center_y + radius_y;
	draw_y2 = draw_y3 = center_y - radius_y;

	GUI_Point(draw_x0, draw_y0, color);					// 畫y軸上的兩個端點 
	GUI_Point(draw_x2, draw_y2, color);

	while( (radius_yy*xx) < (radius_xx*yy) ) 
	{  	
		if(di<0)
	 		di+= radius_yy2*(2*xx+3);
		else
		{  
			di += radius_yy2*(2*xx+3) + 4*radius_xx - 4*radius_xx*yy;
			yy--;
			draw_y0--;
			draw_y1--;
			draw_y2++;
			draw_y3++;				 
		}
		xx ++;						// x軸加1
		draw_x0++;
		draw_x1--;
		draw_x2++;
		draw_x3--;

		GUI_Point(draw_x0, draw_y0, color);
		GUI_Point(draw_x1, draw_y1, color);
		GUI_Point(draw_x2, draw_y2, color);
		GUI_Point(draw_x3, draw_y3, color);
	}
  
	di = radius_xx2*(yy-1)*(yy-1) + radius_yy2*xx*xx + radius_yy + radius_yy2*xx - radius_xx2*radius_yy;
	while(yy>=0) 
	{  
		if(di<0)
		{  
			di+= radius_xx2*3 + 4*radius_yy*xx + 4*radius_yy - 2*radius_xx2*yy;

			xx ++;						// x軸加1	 		
			draw_x0++;
			draw_x1--;
			draw_x2++;
			draw_x3--;  
		}
		else
			di += radius_xx2*3 - 2*radius_xx2*yy;	 	 		     			 
		yy--;
		draw_y0--;
		draw_y1--;
		draw_y2++;
		draw_y3++;	

		GUI_Point(draw_x0, draw_y0, color);
		GUI_Point(draw_x1, draw_y1, color);
		GUI_Point(draw_x2, draw_y2, color);
		GUI_Point(draw_x3, draw_y3, color);
	}     
}
/****************************************************************************
* 名稱：GUI_EllipseFill()
* 功能：畫正橢圓，並填充。給定橢圓的四個點的參數，最左、最右點的x軸坐標值為x0、x1，最上、最下點
*      的y軸坐標為y0、y1。
* 入口參數： x0		最左點的x坐標值
*           x1		最右點的x坐標值
*           y0		最上點的y坐標值
*           y1      最下點的y坐標值
*           color	填充顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_EllipseFill(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1, TCOLOR color)
{  int32_t  draw_x0, draw_y0;			// 劊圖點坐標變量
   int32_t  draw_x1, draw_y1;
   int32_t  draw_x2, draw_y2;
   int32_t  draw_x3, draw_y3;
   int32_t  xx, yy;					// 畫圖控制變量
    
   int32_t  center_x, center_y;		// 橢圓中心點坐標變量
   int32_t  radius_x, radius_y;		// 橢圓的半徑，x軸半徑和y軸半徑
   int32_t  radius_xx, radius_yy;		// 半徑乘平方值
   int32_t  radius_xx2, radius_yy2;	// 半徑乘平方值的兩倍
   int32_t  di;						// 定義決策變量
	
   /* 參數過濾 */
   if( (x0==x1) || (y0==y1) ) return;
   
   /* 計算出橢圓中心點坐標 */
   center_x = (x0 + x1) >> 1;			
   center_y = (y0 + y1) >> 1;
   
   /* 計算出橢圓的半徑，x軸半徑和y軸半徑 */
   if(x0 > x1)
   {  radius_x = (x0 - x1) >> 1;
   }
   else
   {  radius_x = (x1 - x0) >> 1;
   }
   if(y0 > y1)
   {  radius_y = (y0 - y1) >> 1;
   }
   else
   {  radius_y = (y1 - y0) >> 1;
   }
		
   /* 計算半徑乘平方值 */
   radius_xx = radius_x * radius_x;
   radius_yy = radius_y * radius_y;
	
   /* 計算半徑乘4值 */
   radius_xx2 = radius_xx<<1;
   radius_yy2 = radius_yy<<1;
   
    /* 初始化畫圖變量 */
   xx = 0;
   yy = radius_y;
  
   di = radius_yy2 + radius_xx - radius_xx2*radius_y ;	// 初始化決策變量 
	
   /* 計算出橢圓y軸上的兩個端點坐標，作為作圖起點 */
   draw_x0 = draw_x1 = draw_x2 = draw_x3 = center_x;
   draw_y0 = draw_y1 = center_y + radius_y;
   draw_y2 = draw_y3 = center_y - radius_y;
  
	 
   GUI_Point(draw_x0, draw_y0, color);					// 畫y軸上的兩個端點
   GUI_Point(draw_x2, draw_y2, color);
	
   while( (radius_yy*xx) < (radius_xx*yy) ) 
   {  if(di<0)
	  {  di+= radius_yy2*(2*xx+3);
	  }
	  else
	  {  di += radius_yy2*(2*xx+3) + 4*radius_xx - 4*radius_xx*yy;
	 	  
	     yy--;
		 draw_y0--;
		 draw_y1--;
		 draw_y2++;
		 draw_y3++;				 
	  }
	  
	  xx ++;						// x軸加1
	 		
	  draw_x0++;
	  draw_x1--;
	  draw_x2++;
	  draw_x3--;
		
	  GUI_Point(draw_x0, draw_y0, color);
	  GUI_Point(draw_x1, draw_y1, color);
	  GUI_Point(draw_x2, draw_y2, color);
	  GUI_Point(draw_x3, draw_y3, color);
	  
	  /* 若y軸已變化，進行填充 */
	  if(di>=0)
	  {  GUI_HLine(draw_x0, draw_y0, draw_x1, color);
	     GUI_HLine(draw_x2, draw_y2, draw_x3, color);
	  }
   }
  
   di = radius_xx2*(yy-1)*(yy-1) + radius_yy2*xx*xx + radius_yy + radius_yy2*xx - radius_xx2*radius_yy;
   while(yy>=0) 
   {  if(di<0)
	  {  di+= radius_xx2*3 + 4*radius_yy*xx + 4*radius_yy - 2*radius_xx2*yy;
	 	  
	     xx ++;						// x軸加1	 		
	     draw_x0++;
	     draw_x1--;
	     draw_x2++;
	     draw_x3--;  
	  }
	  else
	  {  di += radius_xx2*3 - 2*radius_xx2*yy;	 	 		     			 
	  }
	  
	  yy--;
 	  draw_y0--;
	  draw_y1--;
	  draw_y2++;
	  draw_y3++;	
		
	  GUI_Point(draw_x0, draw_y0, color);
	  GUI_Point(draw_x1, draw_y1, color);
	  GUI_Point(draw_x2, draw_y2, color);
	  GUI_Point(draw_x3, draw_y3, color);
	  
	  /* y軸已變化，進行填充 */
	  GUI_HLine(draw_x0, draw_y0, draw_x1, color);
	  GUI_HLine(draw_x2, draw_y2, draw_x3, color); 
   }     
}
#endif




#if  GUI_FloodFill_EN==1
/****************************************************************************
* 名稱：GUI_ReadLeftPoint()
* 功能：找出指定點左邊最近的非color點。
* 入口參數： x0		指定點的x坐標值
*           y0		指定點的y坐標值
*           color	指定顏色值
* 出口參數：返回該點的x軸坐標值。
* 說明：若沒有找出，則返回最左的x坐標0。
****************************************************************************/
uint32_t  GUI_ReadLeftPoint(uint32_t x0, uint32_t y0, TCOLOR color)
{  uint32_t  i;
   TCOLOR  bakc;
   
   for(i=x0-1; i>0; i--)
   {  GUI_ReadPoint(i, y0, &bakc);
      if( GUI_CmpColor(bakc,color)==0 ) return(i+1);	// 若找到，則返回
   }
   GUI_ReadPoint(i, y0, &bakc);
   if( GUI_CmpColor(bakc,color)==0 ) return(1);		// 若找到，則返回
   
   return(0);
}


/****************************************************************************
* 名稱：GUI_ReadRightPoint()
* 功能：找出指定點右邊最近的非color點。
* 入口參數： x0		指定點的x軸坐標值
*           y0		指定點的y軸坐標值
*           color	指定顏色值
* 出口參數：返回該點的x軸坐標值。
* 說明：若沒有找出，則返回最右的x坐標GUI_LCM_XMAX。
****************************************************************************/
uint32_t  GUI_ReadRightPoint(uint32_t x0, uint32_t y0, TCOLOR color)
{  uint32_t  i;
   TCOLOR  bakc;
   
   for(i=x0+1; i<GUI_LCM_XMAX; i++)
   {  GUI_ReadPoint(i, y0, &bakc);
      if( GUI_CmpColor(bakc,color)==0 ) return(i-1);	// 若找到，則返回
   }
   return(GUI_LCM_XMAX);
}


/****************************************************************************
* 名稱：GUI_CmpPointColor()
* 功能：判斷指定點上的顏色是否為某種顏色。
* 入口參數：x			指定點的x軸坐標值
*		   y		指定點的y軸坐標值
*          color	顏色值
* 出口參數：返回1表示相同，返回0表示不相同。
* 說明：
****************************************************************************/
int  GUI_CmpPointColor(uint32_t x, uint32_t y, TCOLOR color)
{  TCOLOR  bakc;
   
   GUI_ReadPoint(x, y, &bakc);
   return( GUI_CmpColor(bakc,color) );	
}


/* 定義折點個數 */
#ifndef  DOWNP_N
#define  DOWNP_N		20
#endif
#ifndef	 UPP_N
#define  UPP_N			20
#endif
/****************************************************************************
* 名稱：GUI_FloodFill()
* 功能：圖形填充，將指定點內的封閉圖形進行填充。對指定點的顏色區域進行填充，即不是該顏色
*      的像素為邊界(如，指定點上的顏色為紅色，則其它顏色像素均為邊界)。
* 入口參數： x0		指定點的x坐標值
*           y0		指定點的y坐標值
*           color	填充顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍、指定點不在封閉圖形內。
****************************************************************************/
void  GUI_FloodFill(uint32_t x0, uint32_t y0, TCOLOR color)
{  PointXY  down_point[DOWNP_N];	// 定義向下填充轉折點緩衝區
   uint8_t    down_no;				// 向下折點個數
   PointXY  up_point[UPP_N];		// 定義向上填充轉折點緩衝區
   uint8_t    up_no;					// 向上折點個數
   TCOLOR   fcolor;					// 填充點上的顏色
   
   uint32_t  xx, yy;					// 填充臨時x，y變量 (當前填充行的中點)
   uint32_t  xx0;						// 當前填充行的左x值變量
   uint32_t  xx1;						// 當前填充行的右y值變量
   uint32_t  i;
   
   uint32_t  x0_bak, y0_bak;
   uint32_t  x1_bak;
   
   /* 參數過濾 */
   if(x0>=GUI_LCM_XMAX) return;
   if(y0>=GUI_LCM_YMAX) return;
   
   /* 判斷指定點是否為填充顏色，若是則直接返回 */
   GUI_ReadPoint(x0, y0, &fcolor);						// 取得填充點的顏色
   if( GUI_CmpColor(fcolor,color)!=0 ) return;
   
   y0_bak = y0;
   x0_bak = xx0 = GUI_ReadLeftPoint(x0, y0, fcolor);		// 找出當前y坐標上的最左邊的點
   x1_bak = xx1 = GUI_ReadRightPoint(x0, y0, fcolor);		// 找出當前y坐標上的最右邊的點
   down_point[0].x = up_point[0].x = (xx1 + xx0)/2;
   down_point[0].y = up_point[0].y = y0;
   down_no = 1;
   up_no = 1;
   					
   /* 開始向上填充 */
FILL_UP:  
   if(0==up_no) goto FILL_DOWN;							// 若向下掃瞄已完成，則退出
   xx = up_point[up_no-1].x;							// 否則取出下一折點
   yy = up_point[up_no-1].y;
   up_no--; 
   xx0 = GUI_ReadLeftPoint(xx, yy, fcolor);
   xx1 = GUI_ReadRightPoint(xx, yy, fcolor);
   while(1) 
   {  yy += 1;											// 中心點向上一點
       
      if( GUI_CmpPointColor(xx, yy, fcolor)==0 )					
      {  /* 判斷此點是否為終點，若是則退出此次循環 */
         for(i=xx0; i<=xx1; i++)     					// 查找此行是否有需填充點
         {  if( GUI_CmpPointColor(i, yy, fcolor)!=0 ) break;
         }
         if(i>xx1) goto FILL_UP; 
         
         /* 找出新一行中最右邊的點 */
         xx = i;										// 更新xx到要填充的有效區域內
         xx1 = GUI_ReadRightPoint(xx, yy, fcolor);
      }
      else
      {  /* 找出新一行中最右邊的點 */
         xx1 = GUI_ReadRightPoint(xx, yy, fcolor);
      }
      xx0 = GUI_ReadLeftPoint(xx, yy, fcolor);
      
      /* 向下折點。使用y0作為折點變量，x0作為上一折點變量 */
      if(down_no<DOWNP_N)
      {  y0 = xx0;
         x0 = y0-1;
         for(i=y0; i<=xx1; i++)
         {  if( GUI_CmpPointColor(i, yy-1, fcolor)==0 )	// 更新折點
            {  y0 = i;										
            }
            else
            {  if(x0!=y0)								// 找到新的折點
               {  x0 = y0;
                  down_point[down_no].x = i;
                  down_point[down_no].y = yy;
                  down_no++;
               }
            }
            if(down_no>=DOWNP_N) break;					// 若緩衝區已保存滿，則退出
         } // end  of for(i=y0+1; i<xx1; i++)
      } // end of if(down_no<DOWNP_N)
      
      xx = (xx1 + xx0)/2;								// 更新中心點
      GUI_HLine(xx0, yy, xx1, color);					// 填充一行
      
      /* 向上折點。使用y0作為折點變量，x0作為上一折點變量 */
      if(up_no<UPP_N)
      {  y0 = xx0;
         x0 = y0-1;
         for(i=y0; i<=xx1; i++)
         {  if( GUI_CmpPointColor(i, yy+1, fcolor)==0 )	// 更新折點
            {  y0 = i;										
            }
            else
            {  if(x0!=y0)								// 找到新的折點
               {  x0 = y0;
                  up_point[up_no].x = i;
                  up_point[up_no].y = yy;
                  up_no++;
               }
            }
            if(up_no>=UPP_N) break;						// 若緩衝區已保存滿，則退出
         }
      } // end of if(up_no<UPP_N)
      
   } // end of while(1) 

   /* 向下填充 */
FILL_DOWN: 
   if(0==down_no) 
   {  if(0==up_no) 
      {  GUI_HLine(x0_bak, y0_bak, x1_bak, color);
         return;								// 若向下掃瞄已完成，且沒有發現新的向上折點，則退出
      }
      else
      {  goto FILL_UP;
      }
   }
   xx = down_point[down_no-1].x;						// 否則取出下一折點
   yy = down_point[down_no-1].y;
   down_no--;
   xx0 = GUI_ReadLeftPoint(xx, yy, fcolor);
   xx1 = GUI_ReadRightPoint(xx, yy, fcolor);
   
   while(1) 
   {  yy -= 1;											// 中心點向上一點 
      if( GUI_CmpPointColor(xx, yy, fcolor)==0 )					
      {  /* 判斷此點是否為終點，若是則退出此次循環 */
         for(i=xx0; i<=xx1; i++)     					// 查找下一行是否有需填充點
         {  if( GUI_CmpPointColor(i, yy, fcolor)!=0 ) break;
         }
         if(i>xx1) goto FILL_DOWN; 
         
         /* 找出新一行中最右邊的點 */
         xx = i;
         xx1 = GUI_ReadRightPoint(xx, yy, fcolor);
      }
      else
      {  /* 找出新一行中最右邊的點 */
         xx1 = GUI_ReadRightPoint(xx, yy, fcolor);
      }
      xx0 = GUI_ReadLeftPoint(xx, yy, fcolor);
            
      /* 向上折點。使用y0作為折點變量，x0作為上一折點變量 */
      if(up_no<UPP_N)
      {  y0 = xx0;
         x0 = y0-1;
         for(i=y0; i<=xx1; i++)
         {  if( GUI_CmpPointColor(i, yy+1, fcolor)==0 )	// 更新折點
            {  y0 = i;										
            }
            else
            {  if(x0!=y0)								// 找到新的折點
               {  x0 = y0;
                  up_point[up_no].x = i;
                  up_point[up_no].y = yy;
                  up_no++;
               }
            }
            if(up_no>=UPP_N) break;						// 若緩衝區已保存滿，則退出
         }
      }
            
      xx = (xx1 + xx0)/2; 
      GUI_HLine(xx0, yy, xx1, color);					// 填充一行
      
      /* 向下折點。使用y0作為折點變量，x0作為上一折點變量 */
      if(down_no<DOWNP_N)
      {  y0 = xx0;
         x0 = y0-1;
         for(i=y0; i<=xx1; i++)
         {  if( GUI_CmpPointColor(i, yy-1, fcolor)==0 )	// 更新折點
            {  y0 = i;										
            }
            else
            {  if(x0!=y0)								// 找到新的折點
               {  x0 = y0;
                  down_point[down_no].x = i;
                  down_point[down_no].y = yy;
                  down_no++;
               }
            }
            if(down_no>=DOWNP_N) break;					// 若緩衝區已保存滿，則退出
         }
      } // end of if(down_no<DOWNP_N)
      
   } // end of while(1) 
   
//   GUI_HLine(x0_bak, y0_bak, x1_bak, color);
	 
}

#endif

#if  GUI_ArcX_EN==1
/****************************************************************************
* 名稱：GUI_Arc4()
* 功能：畫弧。起點及終點只能為0度-90度、90度-180度、180度-270度、270度-0度等。即分別
*      為第1-4像限的90度弧。
* 入口參數： x0		圓心的x坐標值
*           y0		圓心的y坐標值
*           r       圓弧的半徑
*           angle	畫弧的像限(1-4)
*           color	顯示顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_Arc4(uint32_t x, uint32_t y, uint32_t r, uint8_t angle, TCOLOR color)
{  int32_t  draw_x, draw_y;

   int32_t  op_x, op_y;
   int32_t  op_2rr;
   
   if(r==0) return;
   
   op_2rr = 2*r*r;										// 計算r平方乖以2
   
   switch(angle)
   {  case  1:
            draw_x = x+r;
            draw_y = y;
            
            op_x = r;
            op_y = 0;
 
            while(1)
            {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                 
               /* 計算下一點 */
               op_y++;
               draw_y++;
               if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
               {  op_x--;
                  draw_x--;
               }
               if(op_y>=op_x) break;
            }
            while(1)
            {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                 
               /* 計算下一點 */
               op_x--;
               draw_x--;
               if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) 	// 使用逐點比較法實現畫圓弧
               {  op_y++;
                  draw_y++;
               }
               if(op_x<=0)
               {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                  break;
               }
            }
   
            break;      
   				
      case  2:
            draw_x = x-r;
            draw_y = y;
            
            op_x = r;
            op_y = 0;
 
            while(1)
            {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                 
               /* 計算下一點 */
               op_y++;
               draw_y++;
               if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
               {  op_x--;
                  draw_x++;
               }
               if(op_y>=op_x) break;
            }
            while(1)
            {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                 
               /* 計算下一點 */
               op_x--;
               draw_x++;
               if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) 	// 使用逐點比較法實現畫圓弧
               {  op_y++;
                  draw_y++;
               }
               if(op_x<=0)
               {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                  break;
               }
            }
  
            break;
            
      case  3:
            draw_x = x-r;
            draw_y = y;
            
            op_x = r;
            op_y = 0;
 
            while(1)
            {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                 
               /* 計算下一點 */
               op_y++;
               draw_y--;
               if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
               {  op_x--;
                  draw_x++;
               }
               if(op_y>=op_x) break;
            }
            while(1)
            {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                 
               /* 計算下一點 */
               op_x--;
               draw_x++;
               if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) 	// 使用逐點比較法實現畫圓弧
               {  op_y++;
                  draw_y--;
               }
               if(op_x<=0)
               {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                  break;
               }
            }
      
            break;
            
      case  4:
            draw_x = x+r;
            draw_y = y;
            
            op_x = r;
            op_y = 0;
 
            while(1)
            {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                 
               /* 計算下一點 */
               op_y++;
               draw_y--;
               if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
               {  op_x--;
                  draw_x--;
               }
               if(op_y>=op_x) break;
            }
            while(1)
            {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                 
               /* 計算下一點 */
               op_x--;
               draw_x--;
               if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) 	// 使用逐點比較法實現畫圓弧
               {  op_y++;
                  draw_y--;
               }
               if(op_x<=0)
               {  GUI_Point(draw_x, draw_y, color);		// 開始畫圖
                  break;
               }
            }
            break;
            
      default:
            break;
      
   }

}

/****************************************************************************
* 名稱：GUI_Arc()
* 功能：指定起點、終點及半徑畫弧(不能畫圓)。使用的是順時針方向畫圖。
* 入口參數： x			圓心的x軸坐標值
*           y			圓心的y軸坐標值
*           stangle 	起始角度(0-359度)
*           endangle	終止角度(0-359度)
*           r  			圓的半徑終點
*           color		顯示顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_Arc(uint32_t x, uint32_t y, uint32_t r, uint32_t stangle, uint32_t endangle, TCOLOR color)
{  int32_t  draw_x, draw_y;					// 畫圖坐標變量
   int32_t  op_x, op_y;						// 操作坐標
   int32_t  op_2rr;							// 2*r*r值變量
   
   int32_t  pno_angle;						// 度角點的個數
   uint8_t  draw_on;							// 畫點開關，為1時畫點，為0時不畫
   
   
   /* 參數過濾 */
   if(r==0) return;							// 半徑為0則直接退出
   if(stangle==endangle) return;			// 起始角度與終止角度相同，退出
   if( (stangle>=360) || (endangle>=360) ) return;

   op_2rr = 2*r*r;							// 計算r平方乖以2
   pno_angle = 0;
   /* 先計算出在此半徑下的45度的圓弧的點數 */       
   op_x = r;
   op_y = 0;
   while(1)
   {  pno_angle++; 							// 畫點計數         
      /* 計算下一點 */
      op_y++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
      {  op_x--;
      }
      if(op_y>=op_x) break;
   }
   
   draw_on = 0;								// 最開始關畫點開關
   /* 設置起始點及終點 */
   if(endangle>stangle) draw_on = 1;		// 若終點大於起點，則從一開始即畫點(359)
   stangle = (360-stangle)*pno_angle/45;
   endangle = (360-endangle)*pno_angle/45;
   if(stangle==0) stangle=1;
   if(endangle==0) endangle=1;
   
   /* 開始順時針畫弧，從359度開始(第4像限) */
   pno_angle = 0;
   
   draw_x = x+r;
   draw_y = y;         
   op_x = r;
   op_y = 0;
   while(1)
   {  /* 計算下一點 */
      op_y++;
      draw_y--;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
      {  op_x--;
         draw_x--;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
      } 
      if(op_y>=op_x)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         break;
      }
   }
   
   while(1)
   {  /* 計算下一點 */
      op_x--;
      draw_x--;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) // 使用逐點比較法實現畫圓弧
      {  op_y++;
         draw_y--;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
      } 
      
      if(op_x<=0)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);		// 開始畫圖
         break;
      }
   }
    
    
   /* 開始順時針畫弧，從269度開始(第3像限) */
   draw_y = y-r;
   draw_x = x;         
   op_y = r;
   op_x = 0;
   while(1)
   {  /* 計算下一點 */
      op_x++;
      draw_x--;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_y +1)>0 ) // 使用逐點比較法實現畫圓弧
      {  op_y--;
         draw_y++;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
      } 
      
      if(op_x>=op_y)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);		// 開始畫圖
         break;
      }
   }
   
   while(1)
   {  /* 計算下一點 */
      op_y--;
      draw_y++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_x +1)<=0 ) // 使用逐點比較法實現畫圓弧
      {  op_x++;
         draw_x--;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
      } 
      if(op_y<=0)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         break;
      }
   }
   
   
   /* 開始順時針畫弧，從179度開始(第2像限) */
   draw_x = x-r;
   draw_y = y;         
   op_x = r;
   op_y = 0;
   while(1)
   {  /* 計算下一點 */
      op_y++;
      draw_y++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
      {  op_x--;
         draw_x++;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
      } 
      if(op_y>=op_x)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         break;
      }
   }
   
   while(1)
   {  /* 計算下一點 */
      op_x--;
      draw_x++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) // 使用逐點比較法實現畫圓弧
      {  op_y++;
         draw_y++;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
      } 
      
      if(op_x<=0)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);		// 開始畫圖
         break;
      }
   }
  
  
   /* 開始順時針畫弧，從89度開始(第1像限) */
   draw_y = y+r;
   draw_x = x;         
   op_y = r;
   op_x = 0;
   while(1)
   {  /* 計算下一點 */
      op_x++;
      draw_x++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_y +1)>0 ) // 使用逐點比較法實現畫圓弧
      {  op_y--;
         draw_y--;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
      } 
      
      if(op_x>=op_y)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);		// 開始畫圖
         break;
      }
   }
   
   while(1)
   {  /* 計算下一點 */
      op_y--;
      draw_y--;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_x +1)<=0 ) // 使用逐點比較法實現畫圓弧
      {  op_x++;
         draw_x++;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
      } 
      if(op_y<=0)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         break;
      }
   }
   
}
#endif




#if  GUI_Pieslice_EN==1
/****************************************************************************
* 名稱：GUI_Pieslice()
* 功能：指定起點、終點及半徑扇形(不能畫圓)。使用的是順時針方向畫圖。
* 入口參數： x			圓心的x軸坐標值
*           y			圓心的y軸坐標值
*           stangle 	起始角度(0-359度)
*           endangle	終止角度(0-359度)
*           r  			圓的半徑終點
*           color		顯示顏色
* 出口參數：無
* 說明：操作失敗原因是指定地址超出有效範圍。
****************************************************************************/
void  GUI_Pieslice(uint32_t x, uint32_t y, uint32_t r, uint32_t stangle, uint32_t endangle, TCOLOR color)
{  int32_t  draw_x, draw_y;					// 畫圖坐標變量
   int32_t  op_x, op_y;						// 操作坐標
   int32_t  op_2rr;							// 2*r*r值變量
   
   int32_t  pno_angle;						// 度角點的個數
   uint8_t  draw_on;							// 畫點開關，為1時畫點，為0時不畫
   
   
   /* 參數過濾 */
   if(r==0) return;							// 半徑為0則直接退出
   if(stangle==endangle) return;			// 起始角度與終止角度相同，退出
   if( (stangle>=360) || (endangle>=360) ) return;

   op_2rr = 2*r*r;							// 計算r平方乖以2
   pno_angle = 0;
   /* 先計算出在此半徑下的45度的圓弧的點數 */       
   op_x = r;
   op_y = 0;
   while(1)
   {  pno_angle++; 							// 畫點計數         
      /* 計算下一點 */
      op_y++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
      {  op_x--;
      }
      if(op_y>=op_x) break;
   }
   
   draw_on = 0;								// 最開始關畫點開關
   /* 設置起始點及終點 */
   if(endangle>stangle) draw_on = 1;		// 若終點大於起點，則從一開始即畫點(359)
   stangle = (360-stangle)*pno_angle/45;
   endangle = (360-endangle)*pno_angle/45;
   if(stangle==0) stangle=1;
   if(endangle==0) endangle=1;
   
   /* 開始順時針畫弧，從359度開始(第4像限) */
   pno_angle = 0;
   
   draw_x = x+r;
   draw_y = y;         
   op_x = r;
   op_y = 0;
   while(1)
   {  /* 計算下一點 */
      op_y++;
      draw_y--;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
      {  op_x--;
         draw_x--;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         GUI_Line(x, y, draw_x, draw_y, color);
      } 
      if(op_y>=op_x)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         break;
      }
   }
   
   while(1)
   {  /* 計算下一點 */
      op_x--;
      draw_x--;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) // 使用逐點比較法實現畫圓弧
      {  op_y++;
         draw_y--;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         GUI_Line(x, y, draw_x, draw_y, color);
      } 
      
      if(op_x<=0)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);		// 開始畫圖
         break;
      }
   }
    
    
   /* 開始順時針畫弧，從269度開始(第3像限) */
   draw_y = y-r;
   draw_x = x;         
   op_y = r;
   op_x = 0;
   while(1)
   {  /* 計算下一點 */
      op_x++;
      draw_x--;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_y +1)>0 ) // 使用逐點比較法實現畫圓弧
      {  op_y--;
         draw_y++;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         GUI_Line(x, y, draw_x, draw_y, color);
      } 
      
      if(op_x>=op_y)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);		// 開始畫圖
         break;
      }
   }
   
   while(1)
   {  /* 計算下一點 */
      op_y--;
      draw_y++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_x +1)<=0 ) // 使用逐點比較法實現畫圓弧
      {  op_x++;
         draw_x--;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         GUI_Line(x, y, draw_x, draw_y, color);
      } 
      if(op_y<=0)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         break;
      }
   }
   
   
   /* 開始順時針畫弧，從179度開始(第2像限) */
   draw_x = x-r;
   draw_y = y;         
   op_x = r;
   op_y = 0;
   while(1)
   {  /* 計算下一點 */
      op_y++;
      draw_y++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_x +1)>0 ) 	// 使用逐點比較法實現畫圓弧
      {  op_x--;
         draw_x++;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         GUI_Line(x, y, draw_x, draw_y, color);
      } 
      if(op_y>=op_x)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         break;
      }
   }
   
   while(1)
   {  /* 計算下一點 */
      op_x--;
      draw_x++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_y +1)<=0 ) // 使用逐點比較法實現畫圓弧
      {  op_y++;
         draw_y++;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         GUI_Line(x, y, draw_x, draw_y, color);
      } 
      
      if(op_x<=0)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);		// 開始畫圖
         break;
      }
   }
  
  
   /* 開始順時針畫弧，從89度開始(第1像限) */
   draw_y = y+r;
   draw_x = x;         
   op_y = r;
   op_x = 0;
   while(1)
   {  /* 計算下一點 */
      op_x++;
      draw_x++;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr - 2*op_y +1)>0 ) // 使用逐點比較法實現畫圓弧
      {  op_y--;
         draw_y--;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         GUI_Line(x, y, draw_x, draw_y, color);
      } 
      
      if(op_x>=op_y)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);		// 開始畫圖
         break;
      }
   }
   
   while(1)
   {  /* 計算下一點 */
      op_y--;
      draw_y--;
      if( (2*op_x*op_x + 2*op_y*op_y - op_2rr + 2*op_x +1)<=0 ) // 使用逐點比較法實現畫圓弧
      {  op_x++;
         draw_x++;
      }
      if(draw_on==1) GUI_Point(draw_x, draw_y, color);			// 開始畫圖
      pno_angle++;
      if( (pno_angle==stangle)||(pno_angle==endangle) )			// 若遇到起點或終點，畫點開關取反
      {  draw_on = 1-draw_on;
         if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         GUI_Line(x, y, draw_x, draw_y, color);
      } 
      if(op_y<=0)
      {  if(draw_on==1) GUI_Point(draw_x, draw_y, color);
         break;
      }
   }
}
#endif
