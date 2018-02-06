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
	// TODO Auto-generated destructor stub
	my_thread->join();
}

void PWM::process()
{
	while(1)
	{
		std::thread Th_period([=](){std::this_thread::sleep_for(period);});

		gpio_set_value(pin, std::to_string(first_toggle));

		std::unique_lock<std::mutex> lock(pwm_mutex);
		lock.lock();
		std::this_thread::sleep_for(duty);
		lock.unlock();
		gpio_set_value(pin, std::to_string(second_toggle));

		Th_period.join();
	}
}


void PWM::set_duty(std::chrono::microseconds new_duty)
{
	std::unique_lock<std::mutex> lock(pwm_mutex);
	lock.lock();
	if(new_duty > period)
		duty = period;
	else
		duty = new_duty;

	lock.unlock();
}

