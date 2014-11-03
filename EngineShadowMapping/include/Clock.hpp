#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <cstdint>
#include <chrono>
#include <thread>

namespace Vancom{

class Clock {

public:
	Clock();
	~Clock();

	void init();
	float tick(uint64_t frameRate = 0);

	float getDT() const;
	float getTotalTime() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
	float totalTime, dt;
};


} // end vancom namespace

#endif // END CLOCK_HPP