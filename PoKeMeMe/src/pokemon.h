#pragma once

#include "src/gfx/opengl/glmesh.h";
#include "src/audio/openal/audiobuffer.h"

#include <glm/vec2.hpp>

#include "spritesheet.h"

class Pokemon : public archt::GLMesh {

public:
	enum Sprite {
		
		FRONT,
		FRONT_SHINY,
		FRONT_ALT,
		FRONT_ALT_SHINY,
		BACK,
		BACK_SHINY,
		FOOTPRINT,
		NONE
	};

private:


	int index = 0;
	glm::vec2 uv;
	glm::vec2 uvs[Sprite::NONE];
	Sprite sprite = Sprite::FRONT;
	SpriteSheet* spriteSheet = nullptr;
	glm::vec2 spriteSize;
	glm::vec2 footprintSize;
	glm::vec2 uvOffsets[Sprite::NONE];

	archt::AudioBuffer* cry = nullptr;

public:
	Pokemon();
	Pokemon(glm::vec2 uv);
	~Pokemon();

	void setSprite(Sprite sprite);

	void setUvs(const glm::vec2& uv);
	void translateUv(const glm::vec2& v);
	void snapUvs();

	inline Sprite getSprite() const { return sprite; }
	inline SpriteSheet* getSpriteSheet() const { return spriteSheet; }
	inline const glm::vec2& getUvOffset(Sprite sprite) const { return uvOffsets[sprite]; }
	inline const glm::vec2& getUv() const { return uv; }
	inline archt::AudioBuffer* getCry() const { return cry; }

	void setSpriteSheet(SpriteSheet* sheet);
	void setSpriteSize(const glm::vec2& size);
	void setFootprintSize(const glm::vec2& size);
	void setCry(archt::AudioBuffer* buffer);

	void setUVOffsets(glm::vec2* offsets);
	void loadOffset(const std::string& path);

};
