#include "entity_s.h"

#include "../gfx/gui/gui.h"

#include "scene.h"

namespace archt {



	Entity_s::Entity_s(entt::entity id, Scene* scene) : id(id), scene(scene) {}

	Entity_s::~Entity_s() {
	}




}