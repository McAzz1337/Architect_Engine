#pragma once
#include <functional>
#include <tuple>

namespace archt {

	using CloseCallback = void(*)();

	class GuiWindow_s {
	
	protected:
		bool open = true;
		CloseCallback callback;
		std::function<void(bool*, GuiWindow_s*)> func;

	public:
		GuiWindow_s() = default;
		GuiWindow_s(CloseCallback callback);
		GuiWindow_s(std::function<void(bool*, GuiWindow_s*)> func, CloseCallback callback);
		 virtual ~GuiWindow_s();

		operator bool();

		virtual void operator()();

	};


	template<typename T, typename ...Args>
	class GuiWindow_W  : public  GuiWindow_s {
		static_assert(!(std::is_rvalue_reference_v<Args> && ...));

		T function;
		std::tuple<bool*, GuiWindow_s*, Args...> args;

	public:
		template<typename Tw,
				typename ...Argsw,
				typename = std::enable_if_t<(std::is_convertible_v<Argsw&&, Args> && ...) > >

		GuiWindow_W(Tw&& func, CloseCallback callback, Argsw&&... arguments)
			: GuiWindow_s(callback),
				function(std::forward<Tw>(func)),
				args{ &open, this, std::forward<Argsw>(arguments)... } {
		}

		~GuiWindow_W() {

		}

		void operator()() override {
			std::apply(function, args);
		}

	};

	template<typename T, typename ...Args>
	GuiWindow_s* createGuiWindow_args(T&& func, CloseCallback callback, Args&&... args) {
		return (GuiWindow_s*) new GuiWindow_W<std::decay_t<T>,  std::remove_cv_t<std::remove_reference_t<Args>>...>
			(std::forward <T>(func), callback, std::forward<Args>(args)...);
	}

}