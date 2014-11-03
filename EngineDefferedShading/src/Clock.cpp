#include "Clock.hpp"

using namespace Vancom;

Clock::Clock() : totalTime(0.0f), dt(0.0f){

}

Clock::~Clock(){

}

float Clock::tick(uint64_t frameRate){

	if(frameRate > 0){
		uint64_t sleepTime = 1000 / frameRate;

		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
	}

	t2 = std::chrono::high_resolution_clock::now();
	dt = std::chrono::duration_cast<std::chrono::duration<float>>(t2-t1).count();
	t1 = t2;
	totalTime += dt;
	return dt;
}

void Clock::init(){
	t1 = std::chrono::high_resolution_clock::now();
}

float Clock::getDT() const{
	return dt;
}

float Clock::getTotalTime() const{
	return totalTime;
}