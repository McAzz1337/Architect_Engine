#pragma once
#include <functional>
#include <tuple>

namespace archt {

	using CloseCallback = void(*)();

	class GuiWindow {
	
	protected:

		const char* name = nullptr;
		bool open = true;
		CloseCallback callback;
		std::function<void(const char*, bool*, GuiWindow*)> func;

	public:
		GuiWindow() = default;
		GuiWindow(CloseCallback callback);
		GuiWindow(const char* name, std::function<void(const char*, bool*, GuiWindow*)> func, CloseCallback callback);
		 virtual ~GuiWindow();

		operator bool();

		virtual void operator()();


	};


	template<typename T, typename ...Args>
	class GuiWindow_  : public  GuiWindow {
		static_assert(!(std::is_rvalue_reference_v<Args> && ...));

		T function;
		std::tuple<const char*, bool*, GuiWindow*, Args...> args;

	public:
		template<typename Tw,
				typename ...Argsw,
				typename = std::enable_if_t<(std::is_convertible_v<Argsw&&, Args> && ...) > >

		GuiWindow_(const char* name, Tw&& func, CloseCallback callback, Argsw&&... arguments)
			: GuiWindow(name, nullptr, callback),
				function(std::forward<Tw>(func)),
				args{ name, &open, this, std::forward<Argsw>(arguments)... } {
		}

		~GuiWindow_() {

		}

		void operator()() override {
			std::apply(function, args);
		}

	};

	template<typename T, typename ...Args>
	GuiWindow* createGuiWindow_args(const char* name, T&& func, CloseCallback callback, Args&&... args) {
		return (GuiWindow*) new GuiWindow_<std::decay_t<T>,  std::remove_cv_t<std::remove_reference_t<Args>>...>
			(name, std::forward <T>(func), callback, std::forward<Args>(args)...);
	}

}