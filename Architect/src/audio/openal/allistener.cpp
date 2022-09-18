#include "allistener.h"

#include <AL/al.h>
#include "aldebug.h"

namespace archt {


	ALListener::ALListener() {

		AL_CALL(alListenerfv(AL_POSITION, pos));
		AL_CALL(alListenerfv(AL_VELOCITY, velocity));
		AL_CALL(alListenerfv(AL_ORIENTATION, orientation));
	}

	ALListener::~ALListener() {

	}


	void ALListener::setPosition(const glm::vec3& pos) {
		this->pos[0] = pos[0];
		this->pos[1] = pos[1];
		this->pos[2] = pos[2];
	}

	void ALListener::setVelocity(const glm::vec3& vel) {
		velocity[0] = vel[0];
		velocity[1] = vel[1];
		velocity[2] = vel[2];
	}

}