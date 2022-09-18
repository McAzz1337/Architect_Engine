#pragma once

#include <thread>
#include <future>
#include <functional>



namespace archt {

	template<typename T, typename U>
	class MyThread {

		std::future<T> thread;
		std::function<T(U)> func;

	public:
		MyThread() {

		}

		MyThread(std::function<T(U)> func, bool launch = false) : func(func) {
			if (launch)
				start();
		}

		~MyThread() {

		}

		void start() {
			thread = std::async(std::launch::async, func);
		}

		bool isValid() {
			return thread.valid();
		}
		
		bool isDone() {
			using namespace std::literals::chrono_literals;
			return thread.wait_for(0ms) == std::future_status::ready;
		}

		T result() {
			return thread.get();
		}

		void setFunction(std::function<T(U)> func) {
			this->func = func;
		}

	};


}