/* Write an Embedded C program to initialize ADC Scan mode.
   load lower threshold value 1000 in to ADC_LTR and higher threshold value 2000 in to ADC_HTR.
   Enable ADC watchdog timer interrupt.

   Test case: Assign a breakpoint inside ADC ISR, Adjust potentiometer knob, whenever to reach beyond threshold values program stop in ISR.
 */

#define RCC_AHB1ENR (*((int *)0x40023830))
#define RCC_APB2ENR (*((int *)0x40023844))
#define GPIOC_MODER (*((int *)0x40020800))

#define GPIOB_MODER (*((int *)0x40020400))
#define GPIOB_ODR (*((int *)0x40020414))

#define ADC_CR1 (*((int *)0x40012004))
#define ADC_CR2 (*((int *)0x40012008))
#define ADC_SQR3 (*((int *)0x40012034))
#define ADC_SR (*((int *)0x40012000))
#define ADC_DR (*((int *)0x4001204C))
#define ADC_LTR (*((int *)0x40012028))
#define ADC_HTR (*((int *)0x40012024))

#define STK_CTRL (*((int *)0xE000E010))
#define STK_LOAD (*((int *)0xE000E014))
#define STK_VAL (*((int *)0xE000E018))
#define NVIC_ISER0 (*((int *)0xE000E100))

int POT_value=0;

//System tick timer initialization
void 	STK_Timer_init()
{
	STK_CTRL |= 0X5;//write '101' bit fiels 0,1,2 to enable,
	STK_VAL = 0X578;//write G.V to STK_VAL
}

void KM_mdelay(int m)
{
	STK_LOAD = m*16000;
	while(!(STK_CTRL&0x1<<16))
	{
		;
	}
}

void BUZZ_init()
{
	RCC_AHB1ENR |= 0x1<<1;
	GPIOB_MODER &= ~(0xFCFFFFFF);
	GPIOB_MODER |= 0x01000000;
}
void MUX_init()
{
	RCC_AHB1ENR |= 0x1<<2;
	GPIOC_MODER |= 0x3<<2;
}
void KM_ADC_init()
{
	RCC_APB2ENR |= 0x1<<8;	
	ADC_SQR3 |= 0x0B;
	ADC_CR1 |= 0x1 << 23;
	ADC_CR1 |= 0x0B;
	ADC_LTR = 1000;
	ADC_HTR = 2000;
	ADC_CR1 |= 0X1<<6;
	//	ADC_CR1 |= 0x1<<5;
	//	ADC_CR2 |= 0x1<<10;
	ADC_CR2 |= 0x1;
}
void KM_ADC_read()
{
	ADC_CR2 |= 0x1<<30;
	//KM_mdelay(500);
}
int main()
{
	MUX_init();
	BUZZ_init();
	KM_ADC_init();
	STK_Timer_init();
	NVIC_ISER0 |= 0x1<<18;
	while(1)
	{
		KM_ADC_read();
		POT_value=ADC_DR;
		KM_mdelay(500);
		GPIOB_ODR &= ~(0x1<<12);
	}
}

void ADC_IRQHandler(void)
{
	GPIOB_ODR |= 0x1<<12;
	ADC_SR &= ~(0x1);
}
