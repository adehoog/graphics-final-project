#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>
#include <chrono>
#include <string>
#include <iostream>


struct PlanetaryTextures {
	unsigned int earth;
	unsigned int sun;
	unsigned int moon;
	unsigned int mercury;
	unsigned int venus;
	unsigned int mars;
	unsigned int jupiter;
	unsigned int saturn;
	unsigned int uranus;
	unsigned int neptune;
	unsigned int saturn_ring;
	unsigned int earth_clouds;
};

PlanetaryTextures loadPlanetaryTextures() {
	PlanetaryTextures textPlanet;

	textPlanet.earth = loadTexture("resources/planets/earth2k.jpg");
	textPlanet.sun = loadTexture("resources/planets/2k_sun.jpg");
	textPlanet.moon = loadTexture("resources/planets/2k_moon.jpg");
	textPlanet.mercury = loadTexture("resources/planets/2k_mercury.jpg");
	textPlanet.venus = loadTexture("resources/planets/2k_mercury.jpg");
	textPlanet.mars = loadTexture("resources/planets/2k_mars.jpg");
	textPlanet.jupiter = loadTexture("resources/planets/2k_jupiter.jpg");
	textPlanet.saturn = loadTexture("resources/planets/2k_saturn.jpg");
	textPlanet.uranus = loadTexture("resources/planets/2k_uranus.jpg");
	textPlanet.neptune = loadTexture("resources/planets/2k_neptune.jpg");
	textPlanet.saturn_ring = loadTexture("resources/planets/r.jpg");
	textPlanet.earth_clouds = loadTexture("resources/planets/2k_earth_clouds.jpg");
	return textPlanet;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;

	for (unsigned int i = 0; i < faces.size(); i++)
	{

		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}