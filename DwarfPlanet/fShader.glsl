
#version 460 core

out vec4 FragColor;

in vec2 uvCoordFrag;

uniform sampler2D texSampler;

void main()
{
	FragColor = texture(texSampler, uvCoordFrag);
}