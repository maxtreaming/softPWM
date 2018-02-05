#include "GPIO.h"

int get_port(std::string PNXX)
{
	std::string::iterator it = PNXX.begin() + 1;
	int ret_val = *it;
	return ret_val - 65;
}

int get_pin(std::string PNXX)
{
	std::string::iterator it = PNXX.begin() + 2;
	int ret_val;
	if(*it == '0')
		ret_val = *(it + 1) - 48;
	else
	{
		ret_val = *it - 48;
		ret_val = ret_val * 10 + (*(it + 1) - 48);
	}
	return ret_val;
}

std::string get_port_pin(std::string PNXX)
{
	return std::to_string(get_port(PNXX) * 32 + get_pin(PNXX));
}

std::string toggle_value(std::string value)
{
	if(value == "0")
		return "1";
	else
		return "0";
}

int gpio_export(std::string PNXX, std::string direction)
{
	std::ofstream file("/sys/class/gpio/export", std::ios::out);
	if(file.is_open())
	{
		file << get_port_pin(PNXX);
	}
	else
		return -1;
	file.close();

	file.open("/sys/class/gpio/gpio" + get_port_pin(PNXX) + "/direction", std::ios::out);
	if(file.is_open())
	{
		file << direction;
	}
	else
		return -2;

	file.close();
	return 0;
}

int gpio_unexport(std::string PNXX)
{
	std::ofstream file("/sys/class/gpio/unexport", std::ios::out);
	if(file.is_open())
	{
		file << get_port_pin(PNXX);
	}
	else
		return -1;
	file.close();
	return 0;

}

int gpio_set_event(std::string PNXX, std::string event)
{
	std::ofstream file("/sys/class/gpio/gpio" + get_port_pin(PNXX) + "/edge", std::ios::out);
	if(file.is_open())
	{
		file << event;
	}
	else
		return -1;
	file.close();
	return 0;
}

int gpio_set_value(std::string PNXX, std::string value)
{
	std::ofstream file("/sys/class/gpio/gpio" + get_port_pin(PNXX) + "/value", std::ios::out);
	if(file.is_open())
	{
		file << value;
	}
	else
		return -1;
	file.close();
	return 0;
}

int gpio_get_value(std::string PNXX, std::string &value)
{
	std::ifstream file("/sys/class/gpio/gpio" + get_port_pin(PNXX) + "/value", std::ios::in);
	if(file.is_open())
	{
		file >> value;
	}
	else
		return -1;
	file.close();
	return 0;
}

int gpio_wait_for_event(std::string PNXX, std::string &value, short events, int timeout)
{
	int fd;
	struct pollfd pfd;
	int poll_ret;
	char c;

	fd = open(std::string("/sys/class/gpio/gpio" + get_port_pin(PNXX) + "/value").c_str(), O_RDONLY);
	pfd.fd = fd;
	pfd.events = events;
	pfd.revents = 0;

	lseek(fd, 0, SEEK_SET);
	read(fd, &c, 1);

	poll_ret = poll(&pfd, 1, timeout);

	if(poll_ret > 0)
	{
		lseek(fd, 0, SEEK_SET);
		read(fd, &c, 1);
		value = c;
	}

	close(fd);

	return poll_ret;
}
