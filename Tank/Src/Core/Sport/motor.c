#include "motor.h"

TankEngine Engine_Init(uint16_t arr, uint16_t psc)
{
	TankEngine enginContext = {arr,0,0,TANK_STOP};
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	// 时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB, ENABLE);
	// 初始化PE9和PE11为TB6612FNG的PWM源
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	   // 复用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 高速
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   // 推挽
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   // 无上下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	// 初始化PB10,PB11,PB12,PE15为TB6612FNG的ANx与BNx输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_12;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	PEout(12) = 1;

	// 设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Period = arr;
	// 设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	// 设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	// 向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// 根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	// 选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 比较输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	// CH1预装载使能
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE); // 高级定时器输出需要设置这句
	// 使能TIMx在ARR上的预装载寄存器
	TIM_ARRPreloadConfig(TIM1, ENABLE);

	// 使能TIM1
	TIM_Cmd(TIM1, ENABLE);

	return enginContext;
}

/// @brief 更新电机状态
/// @param engine 电机上下文
void Engine_Update(TankEngine *context)
{
	TIM_SetCompare1(TIM1,(uint32_t)(context->vx*context->arr));
	TIM_SetCompare2(TIM1,(uint32_t)(context->vy*context->arr));
	if(context->state == TANK_FORE)
	{
		AN1 = 1;
		AN2 = 0;
		BN1 = 1;
		BN2 = 0;
	}
	else if(context->state == TANK_BACK)
	{
		AN1 = 0;
		AN2 = 1;
		BN1 = 0;
		BN2 = 1;
	}
	else
	{
		AN1 = 0;
		AN2 = 0;
		BN1 = 0;
		BN2 = 0;
	}
}
