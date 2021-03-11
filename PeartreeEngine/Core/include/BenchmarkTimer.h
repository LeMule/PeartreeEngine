#include <chrono>
#include <iostream>
class BenchmarkTimer
{
public:
	BenchmarkTimer(const char* timerName, std::ostream& outStream);
	~BenchmarkTimer();
	void Stop();

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	std::ostream& m_outStream;
};
