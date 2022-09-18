#include "scheduler.h"


namespace archt {



	Scheduler Scheduler::instance;

	Scheduler::Scheduler() {}

	Scheduler::~Scheduler() {}

	Scheduler& Scheduler::getInstance() {
		return instance;
	}

	void Scheduler::dispatchTasks() {

		if (tasks.size() == 0)
			return;
		
		bool erased = false;
		for (int i = tasks.size() - 1; i > -1; i--) {
			if ((*tasks[i])) {
				(*tasks[i])();
				delete tasks[i];
				tasks.erase(tasks.begin() + i);
				erased = true;
			}
		}

		if (erased)
			tasks.shrink_to_fit();
	}

	

}