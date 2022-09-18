#include "scene.h"

#include "entity_s.h"

#include "../gfx/opengl/glshader.h"
#include "component_s.h"

#include "../filesystem/filemanager.h"

namespace archt {



	Scene::Scene() {
		
	}

	Scene::~Scene() {
		Filemanager::getInstance().issueCleanup();
		printf("Deleted Scene\n");
	}

	Entity_s Scene::createEntity() {
		return Entity_s(registry.create(), this);
	}

	void Scene::deleteEntity(Entity_s entity) {
		registry.destroy(entity);
	}
	

}