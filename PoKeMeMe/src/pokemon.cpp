#include "pokemon.h"

#include "src/fileio.h"



Pokemon::Pokemon() {

}

Pokemon::Pokemon(glm::vec2 uv) : uv(uv) {

	uvs[Sprite::FRONT] = uv + uvOffsets[Sprite::FRONT];
	uvs[Sprite::FRONT_SHINY] = uv + uvOffsets[Sprite::FRONT_SHINY];

	uvs[Sprite::FRONT_ALT] = uv + uvOffsets[Sprite::FRONT_ALT];
	uvs[Sprite::FRONT_ALT_SHINY] = uv + uvOffsets[Sprite::FRONT_ALT_SHINY];

	uvs[Sprite::BACK] = uv + uvOffsets[Sprite::BACK];
	uvs[Sprite::BACK_SHINY] = uv + uvOffsets[Sprite::BACK_SHINY];

	uvs[Sprite::FOOTPRINT] = uv + uvOffsets[Sprite::FOOTPRINT];
}

Pokemon::~Pokemon() {

}

void Pokemon::setSprite(Sprite sprite) { 
	
	using namespace archt;
	this->sprite = sprite;

	glm::vec2 coord = uvs[sprite];
	glm::vec2 spriteSize = sprite == Sprite::FOOTPRINT ? footprintSize :  this->spriteSize;

	Vertex* verteces = vbo->getData();
	int size = vbo->getSize();
	for (int i = 0; i < size; i += size) {

		verteces[i].uv = coord;
		verteces[i + 1].uv = coord + glm::vec2(spriteSize.x, 0.0f);
		verteces[i + 2].uv = coord + spriteSize;
		verteces[i + 3].uv = coord + glm::vec2( 0.0f, spriteSize.y);
	}

}

void Pokemon::setUvs(const glm::vec2& uv) {
	this->uv = uv;
	uvs[Sprite::FRONT] = uv + uvOffsets[Sprite::FRONT];
	uvs[Sprite::FRONT_SHINY] = uv + uvOffsets[Sprite::FRONT_SHINY];

	uvs[Sprite::FRONT_ALT] = uv + uvOffsets[Sprite::FRONT_ALT];
	uvs[Sprite::FRONT_ALT_SHINY] = uv + uvOffsets[Sprite::FRONT_ALT_SHINY];

	uvs[Sprite::BACK] = uv + uvOffsets[Sprite::BACK];
	uvs[Sprite::BACK_SHINY] = uv + uvOffsets[Sprite::BACK_SHINY];

	uvs[Sprite::FOOTPRINT] = uv + uvOffsets[Sprite::FOOTPRINT];
	setSprite(sprite);
}

void Pokemon::translateUv(const glm::vec2& v) {
	archt::Vertex* verteces = vbo->getData();
	int size = vbo->getSize();
	for (int i = 0; i < size; i += size) {
		verteces[i].uv +=	v;
		verteces[i + 1].uv += v;
		verteces[i + 2].uv += v;
		verteces[i + 3].uv += v;
	}
}

void Pokemon::snapUvs() {

}

void Pokemon::setSpriteSheet(SpriteSheet* sheet) {
	spriteSheet = sheet;
	setTexture(spriteSheet);
}

void Pokemon::setFootprintSize(const glm::vec2& size) {
	footprintSize = size;
}


void Pokemon::setSpriteSize(const glm::vec2& size) {
	spriteSize = size;
}

void Pokemon::setCry(archt::AudioBuffer* buffer) {
	cry = buffer;
}

void Pokemon::setUVOffsets(glm::vec2* offsets) {
	for (int i = 0; i < Sprite::NONE; i++) {
		uvOffsets[i] = offsets[i];
	}
}

void Pokemon::loadOffset(const std::string& path) {
	std::vector<std::string> lines;
	archt::readFileSplit(path, lines, true);

	for (int i = 0; i < Sprite::NONE; i++) {
		std::string tokens[2];
		archt::split(lines[i], '=', tokens, 2);
		std::string values[2];
		archt::split(tokens[1], ',', values, 2);
		uvOffsets[i] = spriteSheet->normalizeUv(std::stoi(values[0]), std::stoi(values[1]));
	}
}





