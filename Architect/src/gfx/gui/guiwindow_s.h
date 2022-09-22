#pragma once
#include <functional>
#include <tuple>

namespace archt {

	using CloseCallback = void(*)();

	class GuiWindow_s {
	
	protected:

		const char* name = nullptr;
		bool open = true;
		CloseCallback callback;
		std::function<void(const char*, bool*, GuiWindow_s*)> func;

	public:
		GuiWindow_s() = default;
		GuiWindow_s(CloseCallback callback);
		GuiWindow_s(const char* name, std::function<void(const char*, bool*, GuiWindow_s*)> func, CloseCallback callback);
		 virtual ~GuiWindow_s();

		operator bool();

		virtual void operator()();


	};


	template<typename T, typename ...Args>
	class GuiWindow_W  : public  GuiWindow_s {
		static_assert(!(std::is_rvalue_reference_v<Args> && ...));

		T function;
		std::tuple<const char*, bool*, GuiWindow_s*, Args...> args;

	public:
		template<typename Tw,
				typename ...Argsw,
				typename = std::enable_if_t<(std::is_convertible_v<Argsw&&, Args> && ...) > >

		GuiWindow_W(const char* name, Tw&& func, CloseCallback callback, Argsw&&... arguments)
			: GuiWindow_s(name, nullptr, callback),
				function(std::forward<Tw>(func)),
				args{ name, &open, this, std::forward<Argsw>(arguments)... } {
		}

		~GuiWindow_W() {

		}

		void operator()() override {
			std::apply(function, args);
		}

	};

	template<typename T, typename ...Args>
	GuiWindow_s* createGuiWindow_args(const char* name, T&& func, CloseCallback callback, Args&&... args) {
		return (GuiWindow_s*) new GuiWindow_W<std::decay_t<T>,  std::remove_cv_t<std::remove_reference_t<Args>>...>
			(name, std::forward <T>(func), callback, std::forward<Args>(args)...);
	}

}