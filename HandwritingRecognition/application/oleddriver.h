#ifndef __OLEDDRIVER_H__
#define __OLEDDRIVER_H__

#include "hbird_sdk_hal.h"
#include "oledfont.h"
#include <stdint.h>

#define OLED_CMD 0	// 写命令
#define OLED_DATA 1 // 写数据

// CLK信号
#define OLED_SCL_Clr() gpio_write(GPIOA, OLED_D0_GPIO_MASK, 0);
#define OLED_SCL_Set() gpio_write(GPIOA, OLED_D0_GPIO_MASK, 1);
// 数据信号
#define OLED_SDA_Clr() gpio_write(GPIOA, OLED_D1_GPIO_MASK, 0);
#define OLED_SDA_Set() gpio_write(GPIOA, OLED_D1_GPIO_MASK, 1);
// 复位信号
#define OLED_RES_Clr() gpio_write(GPIOA, OLED_RES_GPIO_MASK, 0);
#define OLED_RES_Set() gpio_write(GPIOA, OLED_RES_GPIO_MASK, 1);
// 写命令 Clr or 写数据 Set
#define OLED_DC_Clr() gpio_write(GPIOA, OLED_DC_GPIO_MASK, 0);
#define OLED_DC_Set() gpio_write(GPIOA, OLED_DC_GPIO_MASK, 1);
// 开发板未提供选片信号
// #define OLED_CS_Clr()
// #define OLED_CS_Set()

// 屏幕初始化
void OLED_Init(void);
// Utilities
uint32_t oled_pow(uint8_t m, uint8_t n);
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
// 屏幕显示控制
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_Display_On(void);
void OLED_Display_Off(void);
// 屏幕显示
void OLED_Clear(void);
void OLED_Set_Pos(uint8_t x, uint8_t y);

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t sizey);

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no, uint8_t sizey);
void OLED_DrawBMP(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[]);

void OLED_ShowString_Enhanced(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey);
void OLED_ShowString_Stepping(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey, int delay_ms);


// 初始化
void OLED_Init(void)
{
	OLED_RES_Clr();
	delay_1ms(200);
	OLED_RES_Set();

	OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00-0x3F)
	OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
	OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WR_Byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00-0x3F)
	OLED_WR_Byte(0x00, OLED_CMD); //-not offset
	OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
	OLED_WR_Byte(0xF1, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
	OLED_WR_Byte(0x12, OLED_CMD);
	OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
	OLED_WR_Byte(0x40, OLED_CMD); // Set VCOM Deselect Level
	OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02, OLED_CMD); //
	OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
	OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
	OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
	OLED_Clear();
	OLED_WR_Byte(0xAF, OLED_CMD); // display ON
}

// m^n工具函数，在OLED_ShowNum中使用
uint32_t oled_pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}

/**
 * @brief 信号传输的底层指令
 * @dat: 传输的数据（8位）
 * @cmd: 0 写命令 1 写数据，请使用宏定义
 */
void OLED_WR_Byte(uint8_t dat, uint8_t cmd)
{
	uint8_t i;
	if (cmd)
		OLED_DC_Set();
	if (cmd == 0)
		OLED_DC_Clr();
	//	OLED_CS_Clr();
	for (i = 0; i < 8; i++)
	{
		OLED_SCL_Clr();
		if (dat & 0x80)
		{
			OLED_SDA_Set();
		}
		else
		{
			OLED_SDA_Clr();
		}
		OLED_SCL_Set();
		dat <<= 1;
	}
	//	OLED_CS_Set();
	//	OLED_DC_Set();
}

/**
 * @brief 屏幕文字与衬底颜色控制 
 * @i: 0 正常显示 1 反色显示
 */
void OLED_ColorTurn(uint8_t i)
{
	if (i == 0)
	{
		OLED_WR_Byte(0xA6, OLED_CMD); // 正常显示
	}
	if (i == 1)
	{
		OLED_WR_Byte(0xA7, OLED_CMD); // 反色显示
	}
}

/**
 * @brief 屏幕显示方向控制
 * @i: 0 正常显示 1 反转显示（旋转180°）
 */
void OLED_DisplayTurn(uint8_t i)
{
	if (i == 0)
	{
		OLED_WR_Byte(0xC8, OLED_CMD); // 正常显示
		OLED_WR_Byte(0xA1, OLED_CMD);
	}
	if (i == 1)
	{
		OLED_WR_Byte(0xC0, OLED_CMD); // 反转显示
		OLED_WR_Byte(0xA0, OLED_CMD);
	}
}
// 开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}
// 关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}
// 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0-7）
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(0, OLED_DATA);
	} // 更新显示
}
// 坐标设置
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

// 在指定位置显示一个字符,包括部分字符
// x:0-127
// y:0-63
// sizey:选择字体 6x8 or 8x16（仅支持8 / 16 两个值作为参数）
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t sizey)
{
	uint8_t c = 0, sizex = sizey / 2;
	uint16_t i = 0, size1;
	if (sizey == 8)
		size1 = 6;
	else
		size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * (sizey / 2);
	c = chr - ' '; // 得到偏移后的值
	OLED_Set_Pos(x, y);
	for (i = 0; i < size1; i++)
	{
		if (i % sizex == 0 && sizey != 8)
			OLED_Set_Pos(x, y++);
		if (sizey == 8)
			OLED_WR_Byte(asc2_0806[c][i], OLED_DATA); // 6X8字号
		else if (sizey == 16)
			OLED_WR_Byte(asc2_1608[c][i], OLED_DATA); // 8x16字号
		//		else if(sizey==xx) OLED_WR_Byte(asc2_xxxx[c][i],OLED_DATA);  //用户添加字号
		else
			return;
	}
}

// 显示数字
// x,y :起点坐标
// num:要显示的数字
// len :数字的位数
// sizey:字体大小
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey)
{
	uint8_t t, temp, m = 0;
	uint8_t enshow = 0;
	if (sizey == 8)
		m = 2;
	for (t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (sizey / 2 + m) * t, y, ' ', sizey);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (sizey / 2 + m) * t, y, temp + '0', sizey);
	}
}

// 显示一个字符号串
// x (0-127),y (0-7对应8个page):起点坐标
// chr:要显示的字符串
// sizey:字体大小
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey)
{
	uint8_t j = 0;
	while (chr[j] != '\0')
	{
		OLED_ShowChar(x, y, chr[j++], sizey);
		if (sizey == 8)
			x += 6;
		else
			x += sizey / 2;
	}
}
// 显示汉字
// 需要手动制作字模，头文件中只含有“中景园电子技术”几个字
// x,y:起点坐标
// no:汉字在字库中的序号
// sizey:字体大小（仅支持16）
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no, uint8_t sizey)
{
	uint16_t i, size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
	for (i = 0; i < size1; i++)
	{
		if (i % sizey == 0)
			OLED_Set_Pos(x, y++);
		if (sizey == 16)
			OLED_WR_Byte(Hzk[no][i], OLED_DATA); // 16x16字号
		//		else if(sizey==xx) OLED_WR_Byte(xxx[c][i],OLED_DATA);  //用户添加字号
		else
			return;
	}
}

// 显示图片
// x (0-127), y (0-63)显示坐标
// sizex (0-128), sizey (0-64), 图片长宽
// BMP：要显示的图片
void OLED_DrawBMP(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[])
{
	uint16_t j = 0;
	uint8_t i, m;
	sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
	for (i = 0; i < sizey; i++)
	{
		OLED_Set_Pos(x, i + y);
		for (m = 0; m < sizex; m++)
		{
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

// 显示一个字符号串，超过一行或遇到'\n'自动换行，直到屏幕右下角为止
// x (0-127),y (0-7对应8个page):起点坐标
// chr:要显示的字符串
// sizey:字体大小
void OLED_ShowString_Enhanced(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey)
{
    if (sizey != 8 && sizey != 16)
    {
        return;
    }
    uint8_t x_initial = x;
	uint8_t j = 0;
	while (chr[j] != '\0')
	{
		OLED_ShowChar(x, y, chr[j++], sizey);
		if (sizey == 8)
        {
			x += 6;
            if (x > 122 || chr[j] == '\n')
            {
                x = x_initial;
                y += 1;
                if (chr[j] == '\n')  j++;
            }
            if (y > 7)
            {
                return;
            }
        }
		else if (sizey == 16)
        {
			x += sizey / 2;
            if (x > 120 || chr[j] == '\n')
            {
                x = x_initial;
                y += 2;
                if (chr[j] == '\n')  j++;
            }
            if (y > 7)
            {
                return;
            }
        }
	}
}

// 显示一个字符号串，超过一行或遇到'\n'自动换行，每显示一个字符停顿一段时间，屏幕满后从头开始
// x (0-127),y (0-7对应8个page):起点坐标
// chr:要显示的字符串
// sizey:字体大小
// delay_ms:每显示一个字符停顿的时间，单位ms
void OLED_ShowString_Stepping(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey, int delay_ms)
{
    if (sizey != 8 && sizey != 16)
    {
        return;
    }
    uint8_t x_initial = x;
    uint8_t y_initial = y;
	uint8_t j = 0;
	while (chr[j] != '\0')
	{
		OLED_ShowChar(x, y, chr[j++], sizey);
		if (sizey == 8)
        {
			x += 6;
            if (x > 122 || chr[j] == '\n')
            {
                x = x_initial;
                y += 1;
                if (chr[j] == '\n')  j++;
            }
            if (y > 7)
            {
                y = y_initial;
                OLED_Clear();
            }
        }
		else if (sizey == 16)
        {
			x += sizey / 2;
            if (x > 120 || chr[j] == '\n')
            {
                x = x_initial;
                y += 2;
                if (chr[j] == '\n')  j++;
            }
            if (y > 7)
            {
                y = y_initial;
                OLED_Clear();
            }
        }
        delay_1ms(delay_ms);
	}
}


#endif // __OLEDDRIVER_H__