#include "audiosource.h"

#include <AL/al.h>

#include "aldebug.h"

namespace archt {


	AudioSource::AudioSource() {
		
		AL_CALL(alGenSources(1, &id));

		AL_CALL(alSourcef(id, AL_PITCH, pitch));
		AL_CALL(alSourcef(id, AL_GAIN, gain));
		AL_CALL(alSource3f(id, AL_POSITION, pos[0], pos[1], pos[2]));
		AL_CALL(alSource3f(id, AL_VELOCITY, velocity[0], velocity[1], velocity[2]));
		AL_CALL(alSourcei(id, AL_LOOPING, loops));
		AL_CALL(alSourcei(id, AL_BUFFER, 0));
	}
	

	AudioSource::AudioSource(AudioBuffer* buffer, float pitch, float gain, int loops) 
		: buffer(buffer), pitch(pitch), gain(gain), loops(loops) {
	
		AL_CALL(alGenSources(1, &id));

		AL_CALL(alSourcef(id, AL_PITCH, pitch));
		AL_CALL(alSourcef(id, AL_GAIN, gain));
		AL_CALL(alSource3f(id, AL_POSITION, pos[0], pos[1], pos[2]));
		AL_CALL(alSource3f(id, AL_VELOCITY, velocity[0], velocity[1], velocity[2]));
		AL_CALL(alSourcei(id, AL_LOOPING, loops));
		
		if (buffer)
			AL_CALL(alSourcei(id, AL_BUFFER, buffer->getId()));

	}
	
	AudioSource::~AudioSource() {
		if (id)
			AL_CALL(alDeleteSources(1, &id));
	}


	void AudioSource::attachBuffer(AudioBuffer* buffer) const {
		if (this->buffer)
			detachBuffer();
		
		this->buffer = buffer;
		AL_CALL(alSourcei(id, AL_BUFFER, buffer->getId()));
	}

	void AudioSource::detachBuffer() const {
		AL_CALL(alSourcei(id, AL_BUFFER, 0));
		this->buffer = nullptr;
	}

	

	void AudioSource::play() const {
		AL_CALL(alSourcePlay(id));
	}
	
	void AudioSource::pause() const {
		AL_CALL(alSourcePause(id));
	}
	
	void AudioSource::rewind() const {
		AL_CALL(alSourceRewind(id));
	}
	
	void AudioSource::stop() const {
		AL_CALL(alSourceStop(id));
	}

	bool AudioSource::isPlaying() const {
		if (!buffer) return false;
		int state;
		AL_CALL(alGetSourcei(id, AL_SOURCE_STATE, &state));
		return state == AL_PLAYING;
	}

	bool AudioSource::isPaused() const {
		if (!buffer) return false;
		int state;
		AL_CALL(alGetSourcei(id, AL_SOURCE_STATE, &state));
		return state == AL_PAUSED;
	}



	void AudioSource::setPitch(float p) {
		pitch = p;
		AL_CALL(alSourcef(id, AL_PITCH, pitch));
	}

	void AudioSource::setGain(float g) {
		gain = g;
		AL_CALL(alSourcef(id, AL_GAIN, gain));
	}

	void AudioSource::setLoops(int l) {
		loops = l;
		AL_CALL(alSourcei(id, AL_LOOPING, l));
	}

	void AudioSource::setPosition(const glm::vec3& pos) {
		this->pos[0] = pos[0];
		this->pos[1] = pos[1];
		this->pos[2] = pos[2];
		AL_CALL(alSource3f(id, AL_POSITION, this->pos[0], this->pos[1], this->pos[2]));
	}

	void AudioSource::setVelocity(const glm::vec3& vel) {
		velocity[0] = vel[0];
		velocity[1] = vel[1];
		velocity[2] = vel[2];
		AL_CALL(alSource3f(id, AL_POSITION, velocity[0], velocity[1], velocity[2]));
	}

	void AudioSource::addGain(float g) {
		
		gain += g;
		
		if (gain > 1.0f)
			gain = 1.0f;
		else if (gain < 0.0f)
			gain = 0.0f;

			AL_CALL(alSourcef(id, AL_GAIN, gain));
	}

}