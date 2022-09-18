#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in float texId;
layout(location = 4) in float matrixId;

uniform mat4 mvp;

out vec3 _pos;
out vec3 _normal;
out vec2 _uv;
out flat int _texId;
out float _matrixId;

void main() {

	gl_Position = mvp * vec4(pos, 1.0);

	_pos = pos;
	_normal = normal;
	_uv = uv;
	_texId = int(texId);
	_matrixId = matrixId;

}