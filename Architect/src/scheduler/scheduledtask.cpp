#include "scheduledtask.h"


namespace archt {




	ScheduledTask::ScheduledTask() {
		submissionTime = delta::getTimePoint();
	}

	ScheduledTask::ScheduledTask(float delay) : delay(delay) {
		submissionTime = delta::getTimePoint();
	}

	ScheduledTask::~ScheduledTask() {}

	ScheduledTask::operator bool() {
		return delta::getDelta<delta::millis>(submissionTime, false) >= delay;
	}






	ScheduledTask_Void::ScheduledTask_Void(float delay, std::function<void()> func) 
		: ScheduledTask(delay), function(func) {

	}

	ScheduledTask_Void::~ScheduledTask_Void() {

	}

	void ScheduledTask_Void::operator()() {
		function();
	}
}