// input_handler.h

#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.h"  // Include the header file for Camera class

void GetDesktopResolution(float& horizontal, float& vertical);

extern GLfloat deltaTime;
extern GLfloat lastFrame;
extern Camera camera;
extern bool onRotate;
extern bool onFreeCam;
extern bool SkyBoxExtra;
extern float SCREEN_WIDTH;
extern float SCREEN_HEIGHT;

extern glm::vec3 point;
extern glm::vec3 PlanetPos;
extern GLfloat lastX;
extern GLfloat lastY;
extern float PlanetSpeed;
extern int PlanetView;

extern bool keys[1024];
extern GLfloat SceneRotateY;
extern GLfloat SceneRotateX;
extern bool onPlanet;

extern bool firstMouse;
extern GLfloat xoff, yoff;

extern GLuint textVAO, textVBO;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
