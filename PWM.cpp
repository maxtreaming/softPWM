/*
 * PWM.cpp
 *
 *  Created on: Feb 5, 2018
 *      Author: maxime
 */

#include "PWM.h"

template<class T>
PWM<T>::PWM(std::string pwm_pin, T period, T duty) : pin(pwm_pin), period(period), duty(duty)
{
	gpio_export(pwm_pin, "low");
	my_thread.reset(new std::thread(&PWM<T>::process, this));

}
template<class T>
PWM<T>::~PWM()
{
	// TODO Auto-generated destructor stub
	my_thread->join();
}

template<class T>
void PWM<T>::process()
{
	while(1)
	{
		std::thread Th_period(&PWM<T>::measure_period, this, period);

		gpio_set_value(pin, "1");

		std::unique_lock<std::mutex> lock(pwm_mutex);
		lock.lock();
		std::this_thread::sleep_for(duty);
		lock.unlock();
		gpio_set_value(pin, "0");

		Th_period.join();
	}
}

template<class T>
void PWM<T>::measure_period(T period)
{
	std::this_thread::sleep_for(period);
}

template<class T>
void PWM<T>::set_duty(T new_duty)
{
	std::unique_lock<std::mutex> lock(pwm_mutex);
	lock.lock();
	if(new_duty > period)
		duty = period;
	else if(new_duty < 0)
		duty = 0;
	else
		duty = new_duty;

	lock.unlock();
}

