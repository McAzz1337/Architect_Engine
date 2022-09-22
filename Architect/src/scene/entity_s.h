#pragma once

#include <entt/entt.hpp>

#include "scene.h"

namespace archt {


	class Entity_s {

		entt::entity id;
		Scene* scene = nullptr;

	public:
		Entity_s() = default;
		Entity_s(entt::entity id, Scene* scene);
		~Entity_s();

		template<typename T, typename ...Args>
		T& addComponent(Args&&... args) {
			return scene->registry.emplace<T>(id, std::forward<Args>(args)...);
		}

		template<typename T>
		bool hasComponent() {
			return scene->registry.any_of<T>(id);
		}

		template<typename T>
		T& getComponent() {
			if (!hasComponent<T>())
				__debugbreak();
			return scene->registry.get<T>(id);
		}


		template<typename T>
		void removeComponent() {
			return scene->registry.remove<T>(id);
		}

		

		inline operator entt::entity() { return id; }
		inline operator const entt::entity() const { return id; }
	};

}