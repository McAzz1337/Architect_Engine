#pragma once
#include <functional>
#include <future>

namespace archt {

	template<class T>
	class Thread {

		std::function<T(void)> func;
		std::future<T> thread;

	public:
		Thread() {

		}

		Thread(std::function<T(void)> func, bool launch = false) : func(func) {
			if (launch)
				start();
		}
		
		~Thread() {

		}

		void start() {
			thread = std::async(std::launch::async, func);
		}


		void setFunction(std::function<T(void)> func) {
			this->func = func;
		}

		bool isDone() const {
			return getStatus() == std::future_status::ready;
			
		}

		bool isValid() const {
			return thread.valid();
		}

		std::future_status getStatus() const {
			return thread.wait_for(std::chrono::duration<int, std::milli>(0));
		}
	
		T result() {
			return thread.get();
		}

		
		
	};

}