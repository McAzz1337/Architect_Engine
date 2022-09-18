#include "spritesheet.h"


SpriteSheet::SpriteSheet() {

}

SpriteSheet::SpriteSheet(const std::string& path) : GLTexture(path) {
	
	pixelX = 1.0f / (float) w;
	pixelY = 1.0f / (float) h;
}

SpriteSheet::~SpriteSheet() {
	
}

glm::vec2 SpriteSheet::normalizeUv(int x, int y) {
	

	glm::vec2 uv(0.0f, 0.0f);
	glm::vec2 sheetSize = getSize();

	if (x != 0) {
		uv.x = (float) x /  sheetSize.x;
	}

	if (y != 0) {
		uv.y = (float) y /  sheetSize.y;
	}

	return uv;
}

void SpriteSheet::setSpriteSize(const glm::vec2& size) {
	spriteSize = size;
}


const glm::vec2 SpriteSheet::getSpriteSizeNormalized() const {
	return glm::vec2(spriteSize.x / (float) w, spriteSize.y / (float) h);
}
