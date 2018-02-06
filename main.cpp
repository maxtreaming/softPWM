/*
 * main.cpp
 *
 *  Created on: Feb 5, 2018
 *      Author: maxime
 */



#include <iostream>
#include <chrono>

#include "PWM.h"

int main()
{
	int period = 1000000/50;
	PWM my_pwm("PA12", std::chrono::microseconds(period), std::chrono::microseconds(0), true);

	int counter = 0;
	while(1)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(period));
		counter = counter + period / 100 ;
		my_pwm.set_duty(std::chrono::microseconds(counter % period));

	}
	return 0;
}
