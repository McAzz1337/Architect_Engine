#pragma once

#include "scheduledtask.h"
#include <vector>

namespace archt {

	class Scheduler {

		std::vector<ScheduledTask*> tasks;
		
		static Scheduler instance;
		Scheduler();
	public:

		~Scheduler();

		static Scheduler& getInstance();


		void dispatchTasks();

		template<typename = float, typename T, typename = void>
		void submitTask(float delay, T&& func) {
			tasks.push_back(new ScheduledTask_Void(delay, (std::function<void()>) (func)));
		}
		
		template<typename = float, typename T, typename ...Args>
		void submitTask(float delay, T&& func, Args&&... args) {
			tasks.push_back(createScheduledTask_Args(delay, func, std::forward<Args>(args)...));
		}


	};


}