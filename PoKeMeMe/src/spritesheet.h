#pragma once
#include <glm/vec2.hpp>

#include "src/gfx/opengl/gltexture.h"


class SpriteSheet : public archt::GLTexture {

	float pixelX = 0.0f;
	float pixelY = 0.0f;
	glm::vec2 spriteSize;


public:
	SpriteSheet();
	SpriteSheet(const std::string& path);
	~SpriteSheet();

	glm::vec2 normalizeUv(int x = 0, int y = 0);


	void setSpriteSize(const glm::vec2& size);

	const glm::vec2 getSpriteSizeNormalized() const;

	inline glm::vec2 getPixelSizes() const { return glm::vec2(pixelX, pixelY); }
	inline const glm::vec2& getSpriteSize() const { return spriteSize; }
};