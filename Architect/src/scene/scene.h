#pragma once

#include <entt/entt.hpp>

namespace archt {
	class Entity_s;

	class Scene {

		friend class Entity_s;

		entt::registry registry;

	public:
		Scene();
		~Scene();

		Entity_s createEntity();
		void deleteEntity(Entity_s entity);

	};


}