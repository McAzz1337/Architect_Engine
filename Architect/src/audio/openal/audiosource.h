#pragma once

#include "audiobuffer.h"

#include <glm/vec3.hpp>

namespace archt {


	class AudioSource {

		uint32_t id = 0;
		float pitch = 1.0f;
		float gain = 1.0f;
		float pos[3] = { 0.0f, 0.0f, 0.0f};
		float velocity[3] = { 0.0f, 0.0f, 0.0f };
		int loops = 0;
		mutable AudioBuffer* buffer = nullptr;

	public:
		AudioSource();
		AudioSource(AudioBuffer* buffer, float pitch = 1.0f, float gain = 1.0f, int loops = 0);
		~AudioSource();


		void attachBuffer(AudioBuffer* buffer) const;
		void detachBuffer() const;

		

		void play() const;
		void pause() const;
		void rewind() const;
		void stop() const;

		bool isPlaying() const;
		bool isPaused() const;

		void setPitch(float p);
		void setGain(float g);
		void setLoops(int l);
		void setPosition(const glm::vec3& pos);
		void setVelocity(const glm::vec3& vel);

		void addGain(float g);


		float getPitch() const { return pitch; }
		float getGain() const { return gain; }
		int getLoops() const { return loops; }
		const float* const getPosition() const { return pos; }
		const float* const getVelocity() const { return velocity; }
	};


}