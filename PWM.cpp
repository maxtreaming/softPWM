/*
 * PWM.cpp
 *
 *  Created on: Feb 5, 2018
 *      Author: maxime
 */

#include "PWM.h"


PWM::PWM(std::string pwm_pin, std::chrono::microseconds period, std::chrono::microseconds duty, bool toggle) : pin(pwm_pin), period(period), duty(duty)
{
	if(toggle)
	{
		gpio_export(pwm_pin, "high");
		first_toggle = 0;
		second_toggle = 1;
	}
	else
	{
		gpio_export(pwm_pin, "low");
		first_toggle = 1;
		second_toggle = 0;
	}

	my_thread.reset(new std::thread(&PWM::process, this));

}

PWM::~PWM()
{
	my_thread->join();
}

void PWM::process()
{
	while(1)
	{
		if(duty > std::chrono::microseconds(0))
		{
			gpio_set_value(pin, std::to_string(first_toggle));
			pwm_mutex.lock();
			std::this_thread::sleep_for(duty);
			pwm_mutex.unlock();
		}

		if(duty < period)
		{
			gpio_set_value(pin, std::to_string(second_toggle));
			std::this_thread::sleep_for(period - duty);
		}

	}
}


void PWM::set_duty(std::chrono::microseconds new_duty)
{
	std::lock_guard<std::mutex> lock(pwm_mutex);
	if(new_duty > period)
		duty = period;
	else
		duty = new_duty;

}

