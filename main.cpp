#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include "Shader.h"
#include "Sphere.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "Utils.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>



void get_frame_buffer(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const * path);
unsigned int loadEnvironmentMap(std::vector<std::string> skyBox1);



GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool onRotate = false;
bool onFreeCam = true;
bool SkyBoxExtra = false;
float SCREEN_WIDTH = 1600, SCREEN_HEIGHT = 900;

glm::vec3 point = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 PlanetPos = glm::vec3(0.0f, 0.0f, 0.0f);
GLfloat lastX = (GLfloat)(SCREEN_WIDTH / 2.0);
GLfloat lastY = (GLfloat)(SCREEN_HEIGHT / 2.0);
float PlanetSpeed = .1f;
int PlanetView = 0;

bool keys[1024];
GLfloat SceneRotateY = 0.0f;
GLfloat SceneRotateX = 0.0f;
bool onPlanet = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		//camera.Position = PlanetPos;
		onPlanet = true;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

bool firstMouse = true;
GLfloat xoff = 0.0f, yoff = 0.0f;


GLuint textVAO, textVBO;


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)(xpos - lastX);
	GLfloat yoffset = (GLfloat)(lastY - ypos);
	xoff = xoffset;
	yoff = yoff;

	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;
	if (onRotate)
	{
		SceneRotateY += yoffset * 0.1f;
		SceneRotateX += xoffset * 0.1f;
	}
	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset == 1)
		camera.ProcessKeyboard(SCROLL_FORWARD, deltaTime);
	else
	{
		camera.ProcessKeyboard(SCROLL_BACKWARD, deltaTime);
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (onFreeCam && !camera.FreeCam)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			onRotate = true;
		else onRotate = false;
	}
}

int main() {

	camera.LookAtPos = point;
	/* GLFW INIT */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	/* GLFW INIT */

	/* GLFW WINDOW CREATION */
	//GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", glfwGetPrimaryMonitor(), NULL);
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, get_frame_buffer);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	/* GLFW WINDOW CREATION */


	/* LOAD GLAD */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* SHADERS */
	Shader SimpleShader("simpleVS.vs", "simpleFS.fs");
	Shader SkyboxShader("skybox.vs", "skybox.fs");
	Shader texShader("simpleVS.vs", "texFS.fs");
	Shader lighTing("lighting.vs", "lighting.fs");




	/* LOAD TEXTURES */
	PlanetaryTextures planetaryTextures = loadPlanetaryTextures();
	/* LOAD TEXTURES */

	/* SPHERE GENERATION */
	Sphere Sun(100.0f, 36 * 5, 18 * 5);
	Sphere Mercury(10.0f, 36, 18);
	Sphere Venus(12.0f, 36, 18);
	Sphere Earth(11.8f, 36, 18);
	Sphere Mars(8.0f, 36, 18);
	Sphere Jupiter(40.0f, 36, 18);
	Sphere Saturn(37.0f, 36, 18);
	Sphere Uranus(30.0f, 36, 18);
	Sphere Neptune(30.0f, 36, 19);
	Sphere Moon(5.5f, 36, 18);
	/* SPHERE GENERATION */


	/* SKYBOX GENERATION */
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* SKYBOX GENERATION */

	/* VERTEX GENERATION FOR ORBITS */
	std::vector<float> orbVert;
	GLfloat xx;
	GLfloat zz;
	float angl;
	for (int i = 0; i < 2000; i++)
	{
		angl = (float)(M_PI / 2 - i * (M_PI / 1000));
		xx = sin(angl) * 100.0f;
		zz = cos(angl) * 100.0f;
		orbVert.push_back(xx);
		orbVert.push_back(0.0f);
		orbVert.push_back(zz);

	}
	/* VERTEX GENERATION FOR ORBITS */

	/* VAO-VBO for ORBITS*/
	GLuint VBO_t, VAO_t;
	glGenVertexArrays(1, &VAO_t);
	glGenBuffers(1, &VBO_t);
	glBindVertexArray(VAO_t);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_t);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * orbVert.size(), orbVert.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	/* VAO-VBO for ORBITS*/


	unsigned int cubemapTexture = loadEnvironmentMap(skyBox1);

	unsigned int cubemapTextureExtra = loadEnvironmentMap(skyBox2);
	GLfloat camX = 10.0f;
	GLfloat camZ = 10.0f;
	
	camera.Position = glm::vec3(0.0f, 250.0f, -450.0f);
	camera.Yaw = 90.0f;
	camera.Pitch = -40.0f;
	camera.ProcessMouseMovement(xoff, yoff);
	camera.FreeCam = false;
	onFreeCam = true;
	glm::mat4 view;
	glm::vec3 PlanetsPositions[9];
	while (!glfwWindowShouldClose(window))
	{
		
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/* ZOOM CONTROL */
		if (!camera.FreeCam)
		{
			if (camera.Position.y < 200 && camera.Position.y > 200.0f)
				camera.MovementSpeed = 300.0f;
			if (camera.Position.y < 125.f && camera.Position.y > 70.0f)
				camera.MovementSpeed = 200.0f;
			if (camera.Position.y < 70.f && camera.Position.y > 50.0f)
				camera.MovementSpeed = 100.0f;

			if (camera.Position.y > 200 && camera.Position.y < 400.0f)
				camera.MovementSpeed = 400.0f;
			if (camera.Position.y > 125.f && camera.Position.y < 200.0f)
				camera.MovementSpeed = 300.0f;
			if (camera.Position.y > 70.f && camera.Position.y < 125.0f)
				camera.MovementSpeed = 200.0f;
		}
		/* ZOOM CONTROL */

		processInput(window); // input

		if (!onFreeCam)
		{
			SceneRotateY = 0.0f;
			SceneRotateX = 0.0f;
		}
		if (camera.FreeCam || PlanetView > 0)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		// render
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		lighTing.Use();

		SimpleShader.Use();

		glm::mat4 model = glm::mat4(1.0f);
	
		double viewX;
		double viewZ;
		glm::vec3 viewPos;
		
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10000.0f);
		SimpleShader.setMat4("model", model);
		SimpleShader.setMat4("view", view);
		SimpleShader.setMat4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.sun);


		/* SUN */
		glm::mat4 model_sun;
		model_sun = glm::rotate(model_sun, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_sun = glm::rotate(model_sun, (GLfloat)glfwGetTime() * glm::radians(23.5f) * 0.25f, glm::vec3(0.0f, 0.0f, 1.f));
		model_sun = glm::translate(model_sun, point);
		SimpleShader.setMat4("model", model_sun);
		Sun.Draw();
		/* SUN */

		/* MERCURY */
		glm::mat4 model_mercury;
		double xx = sin(glfwGetTime() * PlanetSpeed) * 100.0f *2.0f *1.3f;
		double zz = cos(glfwGetTime() * PlanetSpeed) * 100.0f *2.0f *1.3f;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.mercury);
		model_mercury = glm::translate(model_mercury, point);
		model_mercury = glm::rotate(model_mercury, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		model_mercury = glm::rotate(model_mercury, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		model_mercury = glm::translate(model_mercury, glm::vec3(xx, 0.0f, zz));
		PlanetsPositions[0] = glm::vec3(xx, 0.0f, zz);
		model_mercury = glm::rotate(model_mercury, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_mercury = glm::rotate(model_mercury, (GLfloat)glfwGetTime() * glm::radians(-90.0f) * 0.05f, glm::vec3(0.0f, 0.0f, 1.f));
		SimpleShader.setMat4("model", model_mercury);
		Mercury.Draw();
		/* MERCURY */

		/* VENUS */
		glm::mat4 model_venus;
		xx = sin(glfwGetTime() * PlanetSpeed *0.75f) * 100.0f * 3.0f *1.3f;
		zz = cos(glfwGetTime() * PlanetSpeed * 0.75f) * 100.0f * 3.0f *1.3f;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.venus);
		model_venus = glm::translate(model_venus, point);
		model_venus = glm::rotate(model_venus, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		model_venus = glm::rotate(model_venus, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		model_venus = glm::translate(model_venus, glm::vec3(xx, 0.0f, zz));
		PlanetsPositions[1] = glm::vec3(xx, 0.0f, zz);
		model_venus = glm::rotate(model_venus, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_venus = glm::rotate(model_venus, glm::radians(-132.5f), glm::vec3(0.0f, 1.0f, 0.f));
		model_venus = glm::rotate(model_venus, (GLfloat)glfwGetTime() * glm::radians(-132.5f) * 0.012f, glm::vec3(0.0f, 0.0f, 1.f));
		SimpleShader.setMat4("model", model_venus);
		Venus.Draw();
		/* VENUS */

		/* EARTH */
		glm::mat4 model_earth;
		xx = sin(glfwGetTime() * PlanetSpeed * 0.55f) * 100.0f * 4.0f *1.3f;
		zz = cos(glfwGetTime() * PlanetSpeed * 0.55f) * 100.0f * 4.0f *1.3f;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.earth);
		//glBindTexture(GL_TEXTURE_2D, planetaryTextures.earth_clouds);
		model_earth = glm::translate(model_earth, point);
		model_earth = glm::rotate(model_earth, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		model_earth = glm::rotate(model_earth, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		model_earth = glm::translate(model_earth, glm::vec3(xx, 0.0f, zz));
		glm::vec3 EarthPoint = glm::vec3(xx, 0.0f, zz);
		PlanetsPositions[2] = glm::vec3(xx, 0.0f, zz);
		model_earth = glm::rotate(model_earth, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_earth = glm::rotate(model_earth, glm::radians(-33.25f), glm::vec3(0.0f, 1.0f, 0.f));
		model_earth = glm::rotate(model_earth, (GLfloat)glfwGetTime() * glm::radians(-33.25f) * 2.0f, glm::vec3(0.0f, 0.0f, 1.f));
		camera.LookAtPos = glm::vec3(model_earth[3][0], model_earth[3][1], model_earth[3][2]);
		SimpleShader.setMat4("model", model_earth);
		Earth.Draw();  
		/* EARTH */
		
		/* MOON */
		glm::mat4 model_moon;
		xx = sin(glfwGetTime() * PlanetSpeed * 67.55f) * 100.0f * 0.5f *1.3f;
		zz = cos(glfwGetTime() * PlanetSpeed * 67.55f) * 100.0f * 0.5f *1.3f;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.moon);
		model_moon = glm::rotate(model_moon, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		model_moon = glm::rotate(model_moon, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		model_moon = glm::translate(model_moon, EarthPoint);
		model_moon = glm::translate(model_moon, glm::vec3(xx, 0.0f, zz));
		model_moon = glm::rotate(model_moon, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_moon = glm::rotate(model_moon, glm::radians(-32.4f), glm::vec3(0.0f, 1.0f, 0.f));
		model_moon = glm::rotate(model_moon, (GLfloat)glfwGetTime() * glm::radians(-32.4f) * 3.1f, glm::vec3(0.0f, 0.0f, 1.f));
		SimpleShader.setMat4("model", model_moon);
		Moon.Draw();
		/* MOON */


		/* MARS */
		glm::mat4 model_mars;
		xx = sin(glfwGetTime() * PlanetSpeed * 0.35f) * 100.0f * 5.0f *1.3f;
		zz = cos(glfwGetTime() * PlanetSpeed * 0.35f) * 100.0f * 5.0f *1.3f;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.mars);
		model_mars = glm::translate(model_mars, point);
		model_mars = glm::rotate(model_mars, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		model_mars = glm::rotate(model_mars, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		model_mars = glm::translate(model_mars, glm::vec3(xx, 0.0f, zz));
		PlanetsPositions[3] = glm::vec3(xx, 0.0f, zz);
		model_mars = glm::rotate(model_mars, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_mars = glm::rotate(model_mars, glm::radians(-32.4f), glm::vec3(0.0f, 1.0f, 0.f));
		model_mars = glm::rotate(model_mars, (GLfloat)glfwGetTime() * glm::radians(-32.4f) * 2.1f, glm::vec3(0.0f, 0.0f, 1.f));
		SimpleShader.setMat4("model", model_mars);
		Mars.Draw();
		/* MARS */

		/* JUPITER */
		glm::mat4 model_jupiter;
		xx = sin(glfwGetTime() * PlanetSpeed * 0.2f) * 100.0f * 6.0f *1.3f;
		zz = cos(glfwGetTime() * PlanetSpeed * 0.2f) * 100.0f * 6.0f *1.3f;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.jupiter);
		model_jupiter = glm::translate(model_jupiter, point);
		model_jupiter = glm::rotate(model_jupiter, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		model_jupiter = glm::rotate(model_jupiter, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		model_jupiter = glm::translate(model_jupiter, glm::vec3(xx, 0.0f, zz));
		PlanetsPositions[4] = glm::vec3(xx, 0.0f, zz);
		model_jupiter = glm::rotate(model_jupiter, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_jupiter = glm::rotate(model_jupiter, glm::radians(-23.5f), glm::vec3(0.0f, 1.0f, 0.f));
		model_jupiter = glm::rotate(model_jupiter, (GLfloat)glfwGetTime() * glm::radians(-23.5f) * 4.5f, glm::vec3(0.0f, 0.0f, 1.f));
		SimpleShader.setMat4("model", model_jupiter);
		Jupiter.Draw();
		/* JUPITER */

		/* SATURN */
		glm::mat4 model_saturn;
		xx = sin(glfwGetTime() * PlanetSpeed * 0.15f) * 100.0f * 7.0f *1.3f;
		zz = cos(glfwGetTime() * PlanetSpeed * 0.15f) * 100.0f * 7.0f *1.3f;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.saturn);
		model_saturn = glm::translate(model_saturn, point);
		model_saturn = glm::rotate(model_saturn, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		model_saturn = glm::rotate(model_saturn, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		model_saturn = glm::translate(model_saturn, glm::vec3(xx, 0.0f, zz));
		glm::vec3 SatrunPoint = glm::vec3(xx, 0.0f, zz);
		PlanetsPositions[5] = glm::vec3(xx, 0.0f, zz);
		model_saturn = glm::rotate(model_saturn, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_saturn = glm::rotate(model_saturn, glm::radians(-34.7f), glm::vec3(0.0f, 1.0f, 0.f));
		model_saturn = glm::rotate(model_saturn, (GLfloat)glfwGetTime() * glm::radians(-34.7f) * 4.48f, glm::vec3(0.0f, 0.0f, 1.f));
		SimpleShader.setMat4("model", model_saturn);
		Saturn.Draw();
		/* SATURN */

		/* URANUS */
		glm::mat4 model_uranus;
		xx = sin(glfwGetTime() * PlanetSpeed * 0.1f) * 100.0f * 8.0f *1.3f;
		zz = cos(glfwGetTime() * PlanetSpeed * 0.1f) * 100.0f * 8.0f *1.3f;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.uranus);
		model_uranus = glm::translate(model_uranus, point);
		model_uranus = glm::rotate(model_uranus, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		model_uranus = glm::rotate(model_uranus, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		model_uranus = glm::translate(model_uranus, glm::vec3(xx, 0.0f, zz));
		PlanetsPositions[6] = glm::vec3(xx, 0.0f, zz);
		model_uranus = glm::rotate(model_uranus, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_uranus = glm::rotate(model_uranus, glm::radians(-99.0f), glm::vec3(0.0f, 1.0f, 0.f));
		model_uranus = glm::rotate(model_uranus, (GLfloat)glfwGetTime() * glm::radians(-99.0f) * 4.5f, glm::vec3(0.0f, 0.0f, 1.f));
		SimpleShader.setMat4("model", model_uranus);
		Uranus.Draw();
		/* URANUS */

		/* NEPTUNE */
		glm::mat4 model_neptune;
		xx = sin(glfwGetTime() * PlanetSpeed * 0.08f) * 100.0f * 9.0f *1.3f;
		zz = cos(glfwGetTime() * PlanetSpeed * 0.08f) * 100.0f * 9.0f *1.3f;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.neptune);

		model_neptune = glm::translate(model_neptune, point);
		model_neptune = glm::rotate(model_neptune, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		model_neptune = glm::rotate(model_neptune, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		model_neptune = glm::translate(model_neptune, glm::vec3(xx, 0.0f, zz));
		PlanetsPositions[7] = glm::vec3(xx, 0.0f, zz);
		model_neptune = glm::rotate(model_neptune, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.f));
		model_neptune = glm::rotate(model_neptune, glm::radians(-30.2f), glm::vec3(0.0f, 1.0f, 0.f));
		model_neptune = glm::rotate(model_neptune, (GLfloat)glfwGetTime() * glm::radians(-30.2f) * 4.0f, glm::vec3(0.0f, 0.0f, 1.f));
		
		SimpleShader.setMat4("model", model_neptune);
		Neptune.Draw();
		/* NEPTUNE */

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.venus);

		/* ORBITS */
		glBindVertexArray(VAO_t);
		glLineWidth(1.0f);
		glm::mat4 modelorb;
		for (float i = 2; i < 10; i++)
		{
			modelorb = glm::mat4(1);
			modelorb = glm::translate(modelorb, point);
			modelorb = glm::rotate(modelorb, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
			modelorb = glm::rotate(modelorb, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
			modelorb = glm::scale(modelorb, glm::vec3(i *1.3f, i*1.3f, i*1.3f));
			SimpleShader.setMat4("model", modelorb);
			glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)orbVert.size() / 3);

		}
		modelorb = glm::mat4(1);
		modelorb = glm::rotate(modelorb, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
		modelorb = glm::rotate(modelorb, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
		modelorb = glm::translate(modelorb, EarthPoint);
		modelorb = glm::scale(modelorb, glm::vec3(0.5f *1.3f , 0.5f *1.3f, 0.5f *1.3f));
		SimpleShader.setMat4("model", modelorb);
		glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)orbVert.size() / 3);
		/* ORBITS */

		/* SATURN RINGS */
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.saturn_ring);
		glLineWidth(2.0f);
		GLfloat rr = 0.55f;
		for (int i = 0; i < 25; i++)
		{
			modelorb = glm::mat4(1);
			
			modelorb = glm::rotate(modelorb, glm::radians(SceneRotateY), glm::vec3(1.0f, 0.0f, 0.0f));
			modelorb = glm::rotate(modelorb, glm::radians(SceneRotateX), glm::vec3(0.0f, 0.0f, 1.0f));
			modelorb = glm::translate(modelorb, SatrunPoint);
			modelorb = glm::rotate(modelorb, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			modelorb = glm::scale(modelorb, glm::vec3(rr, rr, rr));
			SimpleShader.setMat4("model", modelorb);
			glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)orbVert.size() / 3);
			if (i == 15)
				rr += 0.030f;
			else
				rr += 0.01f;
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, planetaryTextures.venus);
		glBindVertexArray(0);
		/* SATURN RINGS */


		/* DRAW SKYBOX */
		glDepthFunc(GL_LEQUAL);  
		SkyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		SkyboxShader.setMat4("view", view);
		SkyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		if (SkyBoxExtra)
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureExtra);
		else
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);  
		/* DRAW SKYBOX */

		/* PLANET TRACKING + SHOW INFO OF PLANET */
		switch (PlanetView)
		{
		case 1:
			viewX = sin(glfwGetTime() * PlanetSpeed) * 100.0f *3.5f * 1.3f;
			viewZ = cos(glfwGetTime() * PlanetSpeed) * 100.0f *3.5f * 1.3f;
			viewPos = glm::vec3(viewX, 50.0f, viewZ);
			view = glm::lookAt(viewPos, PlanetsPositions[0], glm::vec3(0.0f, 1.0f, 0.0f));
			//ShowInfo(TextShader);
			break;

		case 2:
			viewX = sin(glfwGetTime() * PlanetSpeed * 0.75f) * 100.0f *4.5f * 1.2f;
			viewZ = cos(glfwGetTime() * PlanetSpeed * 0.75f) * 100.0f *4.5f * 1.2f;
			viewPos = glm::vec3(viewX, 50.0f, viewZ);
			view = glm::lookAt(viewPos, PlanetsPositions[1], glm::vec3(0.0f, 1.0f, 0.0f));
			//ShowInfo(TextShader);
			break;

		case 3:
			viewX = sin(glfwGetTime() * PlanetSpeed * 0.55f) * 100.0f *5.5f * 1.2f;
			viewZ = cos(glfwGetTime() * PlanetSpeed * 0.55f) * 100.0f *5.5f * 1.2f;
			viewPos = glm::vec3(viewX, 50.0f, viewZ);
			view = glm::lookAt(viewPos, PlanetsPositions[2], glm::vec3(0.0f, 1.0f, 0.0f));
			//ShowInfo(TextShader);
			break;

		case 4:
			viewX = sin(glfwGetTime() * PlanetSpeed * 0.35f) * 100.0f *6.0f * 1.2f;
			viewZ = cos(glfwGetTime() * PlanetSpeed * 0.35f) * 100.0f *6.0f * 1.2f;
			viewPos = glm::vec3(viewX, 20.0f, viewZ);
			view = glm::lookAt(viewPos, PlanetsPositions[3], glm::vec3(0.0f, 1.0f, 0.0f));
			//ShowInfo(TextShader);
			break;

		case 5:
			viewX = sin(glfwGetTime() * PlanetSpeed * 0.2f) * 100.0f *7.5f * 1.3f;
			viewZ = cos(glfwGetTime() * PlanetSpeed * 0.2f) * 100.0f *7.5f * 1.3f;
			viewPos = glm::vec3(viewX, 50.0f, viewZ);
			view = glm::lookAt(viewPos, PlanetsPositions[4], glm::vec3(0.0f, 1.0f, 0.0f));
			//ShowInfo(TextShader);
			break;

		case 6:
			viewX = sin(glfwGetTime() * PlanetSpeed * 0.15f) * 100.0f *8.5f * 1.3f;
			viewZ = cos(glfwGetTime() * PlanetSpeed * 0.15f) * 100.0f *8.5f * 1.3f;
			viewPos = glm::vec3(viewX, 50.0f, viewZ);
			view = glm::lookAt(viewPos, PlanetsPositions[5], glm::vec3(0.0f, 1.0f, 0.0f));
			//ShowInfo(TextShader);
			break;

		case 7:
			viewX = sin(glfwGetTime() * PlanetSpeed * 0.1f) * 100.0f *9.5f * 1.3f;
			viewZ = cos(glfwGetTime() * PlanetSpeed * 0.1f) * 100.0f *9.5f * 1.3f;
			viewPos = glm::vec3(viewX, 50.0f, viewZ);
			view = glm::lookAt(viewPos, PlanetsPositions[6], glm::vec3(0.0f, 1.0f, 0.0f));
			//ShowInfo(TextShader);
			break;

		case 8:
			viewX = sin(glfwGetTime() * PlanetSpeed * 0.08f) * 100.0f *10.5f * 1.3f;
			viewZ = cos(glfwGetTime() * PlanetSpeed * 0.08f) * 100.0f *10.5f * 1.3f;
			viewPos = glm::vec3(viewX, 50.0f, viewZ);
			view = glm::lookAt(viewPos, PlanetsPositions[7], glm::vec3(0.0f, 1.0f, 0.0f));
			//ShowInfo(TextShader);
			break;

		case 0:
			view = camera.GetViewMatrix();

			break;
		}



		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO_t);
	glDeleteBuffers(1, &VBO_t);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		SkyBoxExtra = true;

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		PlanetView = 0;
		onFreeCam = false;
		camera.FreeCam = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		PlanetView = 0;
		onFreeCam = true;
		camera.FreeCam = false;
		camera.Position = glm::vec3(0.0f, 250.0f, -450.0f);
		camera.Yaw = 90.0f;
		camera.Pitch = -40.0f;
		camera.GetViewMatrix();
		camera.ProcessMouseMovement(xoff,yoff);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		PlanetView = 1;
	
		onFreeCam = false;
		camera.FreeCam = false;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		PlanetView = 2;
		onFreeCam = false;
		camera.FreeCam = false;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		PlanetView = 3;

		onFreeCam = false;
		camera.FreeCam = false;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
	{
		PlanetView = 4;
	
		onFreeCam = false;
		camera.FreeCam = false;
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
	{
		PlanetView = 5;
	
		onFreeCam = false;
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
	{
		PlanetView = 6;
		
		onFreeCam = false;
		camera.FreeCam = false;
	}
	if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
	{
		PlanetView = 7;
	
		onFreeCam = false;
		camera.FreeCam = false;
	}
	if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
	{
		PlanetView = 8;

		onFreeCam = false;
		camera.FreeCam = false;
	}

} 
