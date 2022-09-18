#version 450 core

#define MAX_TEXTURES 32

in vec3 _normal;
in vec2 _uv;
in flat int _texId;
in float _matrixId;


uniform float alpha;
uniform vec2 uvOffset;
uniform vec4 tint = vec4(1, 1, 1, 1);
uniform sampler2D tex[MAX_TEXTURES];

out vec4 _out;


void main() {
	
	vec4 color = texture(tex[_texId], _uv + uvOffset);


	if (color.w == 0.0) {
		discard;
	}

	_out = vec4(color.xyz, alpha);
}