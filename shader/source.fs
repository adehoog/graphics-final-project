#version 330 core
out vec4 FragColor;

in vec2 texCoord;
uniform sampler2D ourTexture;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = texture(ourTexture, texCoord);
}