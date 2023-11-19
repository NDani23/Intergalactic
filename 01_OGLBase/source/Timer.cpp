#include "../headers/Timer.h"

#include <iostream>

Timer::Timer()
{
	m_StartTimepoint = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
	Stop();
}

void Timer::Stop()
{
	auto endTimepoint = std::chrono::high_resolution_clock::now();
	
	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch();
	
	auto duration = (end - start).count();
	double ms = duration * 0.001;
	
	std::cout << duration << " us (" << ms << " ms)" << std::endl;
}