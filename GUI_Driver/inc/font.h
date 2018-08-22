#ifndef __FONT_H
#define __FONT_H  

#define Font_06_08_EN 1
#define Font_06_12_EN 1
#define Font_12_24_EN 0


#if Font_06_08_EN==1
	extern const unsigned char asc2_0608[95][6];
#endif
#if Font_06_12_EN==1
	extern const unsigned char asc2_0612[95][12];
#endif
#if Font_12_24_EN==1
	extern const unsigned char asc2_1224[95][36];
#endif

#endif
