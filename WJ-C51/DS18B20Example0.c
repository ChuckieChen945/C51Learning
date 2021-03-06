/**
 * @brief 超出最高值、最低值范围则报警
 * 开机后在LCD上显示温度值，可通过独立按键设定一个最高值和最低值，超过该范围则报警
 * 
 * @file DS18B20Example0.c
 * @author your name
 * @date 2018-03-05
 */

#include <reg52.h>
#include <intrins.h>
#include "LCD1602.h"
#include "DS18B20.h"
#include "delay.h"
#include "qxmcs51_config.h"
#include "datatype.h"

sbit ds = P2 ^ 2;
sbit beep = P2 ^ 3;
sbit rd = P3 ^ 7;
bit flag;
uchar ds_rom[8];
uchar H, L, Key_value;

/**
 * @brief 实现键盘的相关功能
 * 检测键盘是否被按下并对L与H进行加减
 * 
 */
void key()
{
    Key_value = P3; //4个独立按键占P3^0~P3^3
    Key_value = Key_value & 0x0f;
    if (Key_value != 0x0f) //如果有按键被按下
    {
        Delay_Ms(10);
        if (Key_value != 0x0f)
        {
            switch (Key_value)
            {
            case 0x0e:
                H++;
                break;

            case 0x0d:
                H--;
                break;

            case 0x0b:
                L++;
                break;

            case 0x07:
                L--;
                break;
            }
        }
    }
}

/**
 * @brief 主函数
 * 
 */
void DS18B20Example0()
{
    float i;
    char temp[8];

    DS18B20_SendChangeCmd();

    LCD1602_Init();

    LCD1602_Write_Com(0x80 + 0 + 3); //写位置指针
    LCD1602_Write_Dat('H');          //写入显示值
    LCD1602_Write_Com(0x80 + 0 + 8);
    LCD1602_Write_Dat('L');

    LCD1602_Write_String(0, 1, "TEMP:");
    Delay_Ms(1000);

    H = 40;
    L = 10;
    while (1)
    {
        i = DS18B20_GetTmpValue(); //获取温度值float类型
        DS18B20_SendChangeCmd();
        DS18B20_temperToStr(i, temp);               //将温度值转化为string
        LCD1602_Write_String(7, 1, temp);           //写温度
        LCD1602_Write_String(5, 0, uchartostr(H));  //写H
        LCD1602_Write_String(10, 0, uchartostr(L)); //写L
        if (i >= H || i <= L)
        {
            beep = 0;
        }
        else
        {
            beep = 1;
        }
        key();
        Delay_Ms(200);
    }
}
