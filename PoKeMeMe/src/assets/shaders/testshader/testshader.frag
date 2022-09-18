#version 450 core

#define MAX_TEXTURES 32

in vec3 _pos;
in vec3 _normal;
in vec2 _uv;
in flat int _texId;
in float _matrixId;

uniform sampler2D tex[MAX_TEXTURES];

out vec4 _out;

void main() {
	vec4 color = texture(tex[_texId], _uv);
	//vec4 color = vec4(float(_texId), float(_texId), float(_texId), 1.0);

	_out = color;
}