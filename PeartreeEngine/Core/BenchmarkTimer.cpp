#include "BenchmarkTimer.h"

BenchmarkTimer::BenchmarkTimer(const char* timerName, std::ostream& outStream)
	: m_outStream(outStream)
{
	m_StartTimepoint = std::chrono::high_resolution_clock::now();
	m_outStream << "Benchmark for " << timerName << "... ";
}

BenchmarkTimer::~BenchmarkTimer()
{
	Stop();
}

void BenchmarkTimer::Stop()
{
	auto endTimepoint = std::chrono::high_resolution_clock::now();
	auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimepoint).time_since_epoch();
	auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch();
	auto duration = end - start;
	double ms = duration.count() * 0.001;
	m_outStream << duration.count() << " milliseconds (" << ms << "ms)" << std::endl;
}
