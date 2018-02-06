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
	PWM my_pwm("PA01", std::chrono::milliseconds(5), std::chrono::milliseconds(5), false);

	return 0;
}
