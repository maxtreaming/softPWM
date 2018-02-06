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
#include <string>

#include "GPIO.h"


class PWM {
private:
	std::string pin;
	std::chrono::microseconds period, duty;
	std::unique_ptr<std::thread> my_thread;

	int first_toggle, second_toggle;
	std::mutex pwm_mutex;
	void process();

public:


	PWM(std::string pwm_pin, std::chrono::microseconds period, std::chrono::microseconds duty, bool toggle);
	void set_duty(std::chrono::microseconds new_duty);
	virtual ~PWM();
};

#endif /* PWM_H_ */
