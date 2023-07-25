# MSP430 2_GPIO

## 1. GPIO 寄存器

- GPIO 的特性：

> 1. 独立控制每个 GPIO 口的方向（输入/输出）
> 2. 独立控制每个 GPIO 的输出状态（高/低电平）
> 3. GPIO 在复位后有默认方向

GPIO 有两个寄存器位：

1. 选择口线方向 `GPIOxDIR`
2. 数据位 `GPIOxDATA`

GPIO的寄存器均为8位寄存器。

- 输入/输出方向寄存器（PxDIR）

![NULL](S:\STM32SSC\MSP430\Note\Picture\2_1.jpg)



![NULL](S:\STM32SSC\MSP430\Note\Picture\2_2.jpg)

相互独立的8位定义了8个引脚的输出方向。

> PxDIR:	
>
> `Bit = 1`：端口x引脚为输出模式
>
> `Bit = 0`：端口x引脚为输入模式

- 输入数据寄存器（PxIN）

![NULL](S:\STM32SSC\MSP430\Note\Picture\2_3.jpg)

此寄存器为只读的寄存器，每一位反映了对应引脚的GPIO输入信号。

> PxIN:
>
> `Bit = 1` ：输入高电平
>
> `Bit = 0`：输入低电平

- 上拉/下拉电阻使能寄存器（PxREN）

![NULL](S:\STM32SSC\MSP430\Note\Picture\2_4.jpg)

> PxREN:
>
> `Bit = 1`：使能上下拉电阻。
>
> `Bit = 0`：禁用上下拉电阻。

使能上下拉电阻时，使用PxOUT寄存器对应位进行高低电平选择。0为低电平，1为高电平。

- 输出数据寄存器（PxOUT）

![NULL](S:\STM32SSC\MSP430\Note\Picture\2_5.jpg)

> **如果为输出模式**
>
> `Bit = 1` ：高电平输出
>
> `Bit = 0`：低电平输出
>
> **如果为输入模式，PxREN对应位为1**
>
> `Bit = 1`：上拉输入
>
> `Bit = 0`：下拉输入

- 功能选择寄存器（PxSEL）

![NULL](S:\STM32SSC\MSP430\Note\Picture\2_6.jpg)

> PxSEL:
>
> `Bit = 0`：普通IO口
>
> `Bit = 1`：外设复用IO口

- LED例程演示

```c
#include "io430.h"

/**
 *	@brief	毫秒延时函数
 *  @attention	__delay_cycles()为微秒延时函数，MSP430的时钟主频为1MHz
 */
void delay_ms(int ms)
{
  for(;ms > 0;ms--)
    __delay_cycles(1000);
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P1DIR |= BIT0;        //设置1.0为输出
  P4DIR |= BIT7;
  P1OUT |= BIT0;        //设置1.0为高电平
  P4OUT |= BIT7;
  delay_ms(1000);
  P1OUT ^= BIT0;        //设置1.0翻转
  delay_ms(1000);
  
  while(1)
  {
    P4OUT ^= BIT7;
    delay_ms(1000);
  }
}
```

## 2. 库函数法

```c
#include "driverlib.h"

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDT_A_hold(WDT_A_BASE);
  
  GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);			//设置1.0为输出
  GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN0);		//设置1.0为高电平
  
  while(1)
  {
    ;
  }
}
```

