#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in float matrixId;
layout(location = 2) in float colorId;


layout(std140, binding = 0) uniform matrices {
	mat4 mvp[1000];
};

layout(std140, binding = 1) uniform colors {
	vec4 clrs[1000];
};


out vec4 _color;

void main() {
	
	gl_Position = mvp[int(matrixId)] * vec4(pos, 1.0);

	_color = clrs[int(colorId)];
}

