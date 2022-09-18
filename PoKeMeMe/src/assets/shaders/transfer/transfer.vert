#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in float texId;
layout(location = 4) in float matrixId;

layout (std140, binding = 0) uniform matrices {
	mat4 mvp[1000];
};


out vec3 _normal;
out vec2 _uv;
out flat int _texId;
out float _matrixId;

void main () {

	highp int matrixIndex = int(matrixId);
	gl_Position = mvp[matrixIndex] * vec4(pos, 1.0);


	_normal  = normal;
	_uv = uv;
	_texId = int(texId);
	_matrixId = matrixId;

}