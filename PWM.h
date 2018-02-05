/*
 * PWM.h
 *
 *  Created on: Feb 5, 2018
 *      Author: maxime
 */

#ifndef PWM_H_
#define PWM_H_
#include <thread>
#include <chrono>
#include <memory>
#include <mutex>

#include "GPIO.h"

template<typename T>
class PWM {
private:
	T period, duty;
	std::unique_ptr<std::thread> my_thread;
	std::string pin;
	std::mutex pwm_mutex;
	void process();
	void measure_period(T period);

public:


	PWM(std::string pwm_pin, T period, T duty);
	void set_duty(T new_duty);
	virtual ~PWM();
};

#endif /* PWM_H_ */
