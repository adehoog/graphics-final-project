#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

const float CAMERA_SPEED = 0.1f;

class Camera
{
public:
	//variables
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;

	//constructor
	Camera() {
		position = glm::vec3(0.0f, 250.0f, -450.0f);
		direction = glm::vec3(0.0f, 0.0f, -1.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(position, direction, up);
	}

	void zoomIn() {
		position += CAMERA_SPEED * direction;
	}
	void zoomOut() {
		position -= CAMERA_SPEED * direction;
	}

	void moveLeft() {
		glm::vec3 moveDir = glm::cross(direction, up);
		position -= CAMERA_SPEED * moveDir;
	}
	void moveRight() {
		glm::vec3 moveDir = glm::cross(direction, up);
		position += CAMERA_SPEED * moveDir;
	}
	void moveUp() {
		position += CAMERA_SPEED * up;
	}
	void moveDown() {
		position -= CAMERA_SPEED * up;
	}
};

#endif