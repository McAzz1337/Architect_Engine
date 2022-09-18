#pragma once

#include "../chronos/delta.h"

#include <tuple>
#include <functional>

namespace archt {



	class ScheduledTask {

	protected:
		float delay = 10.0f;
		delta::archt_time submissionTime;

	public:
		ScheduledTask();
		ScheduledTask(float delay);
		~ScheduledTask();

		operator bool();

		
		virtual void operator()() = 0;


	};



	class ScheduledTask_Void : public ScheduledTask {

		std::function<void()> function;
	public:
		ScheduledTask_Void(float delay, std::function<void()> func);
		~ScheduledTask_Void();

		void operator()() override;

	};


	template<typename T, typename ...Args>
	class ScheduledTask_Args : public ScheduledTask {
		static_assert(!(std::is_rvalue_reference_v<Args> && ...));

		T function;
		std::tuple<Args...> args;

	public:
		template<typename Tw,
				typename ...ArgsW,
			typename = std::enable_if_t<(std::is_convertible_v<ArgsW&&, Args> && ...)>>
		ScheduledTask_Args(float delay, Tw&& func, ArgsW&&... args)
			: ScheduledTask(delay),
			function(std::forward<Tw>(func)),
			args{ std::forward<ArgsW>(args)... } {

		}
		
		~ScheduledTask_Args() {
		}

		
		
		void operator()() override {
			std::apply(function, args);
		}


	};

	template<typename T, typename ...Args>
	ScheduledTask* createScheduledTask_Args(float delay, T&& func, Args&&... args) {

		return new ScheduledTask_Args<std::decay_t<T>, std::remove_cv_t<std::remove_reference_t<Args>>...>
			(delay, std::forward<T>(func), std::forward<Args>(args)...);
	}

}