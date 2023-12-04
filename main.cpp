
// template based on material from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include "include/sphere.h"
#include "include/camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(char const* path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int cameraView = 0;
Camera ourCamera = Camera();

//glm::vec3 cameraPos(0.0f, 250.0f, -450.0f);
//glm::vec3 cameraDir(0.0f, 0.0f, -1.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "viewGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // glew: load all OpenGL function pointers
    glewInit();


     // build and compile our shader program
    // ------------------------------------
    Shader ourShader("shader/source.vs", "shader/source.fs"); // shader for object

    // lighting
    /*
    glm::vec3 lPos(1.0f, 1.0f, -1.5f);
    glm::vec3 lCol(1.0f, 1.0f, 1.0f);
    glm::vec3 objCol(1.0f, 0.5f, 0.31f);

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
    bool res = loadObject("objectfiles/cube.obj", vertices, normals);
    */

    /* LOAD TEXTURES */
    unsigned int texture_earth = loadTexture("planets/earth2k.jpg");
    unsigned int t_sun = loadTexture("planets/2k_sun.jpg");
    unsigned int texture_moon = loadTexture("planets/2k_moon.jpg");
    unsigned int texture_mercury = loadTexture("planets/2k_mercury.jpg");
    unsigned int texture_venus = loadTexture("planets/2k_mercury.jpg");
    unsigned int texture_mars = loadTexture("planets/2k_mars.jpg");
    unsigned int texture_jupiter = loadTexture("planets/2k_jupiter.jpg");
    unsigned int texture_saturn = loadTexture("planets/2k_saturn.jpg");
    unsigned int texture_uranus = loadTexture("planets/2k_uranus.jpg");
    unsigned int texture_neptune = loadTexture("planets/2k_neptune.jpg");
    unsigned int texture_saturn_ring = loadTexture("planets/r.jpg");
    unsigned int texture_earth_clouds = loadTexture("planets/2k_earth_clouds.jpg");

    /* CREATE PLANET SPHERES */
    Sphere Sun(100.0f, 36 * 5, 18 * 5);

    // View matrix
    glm::mat4 View = ourCamera.GetViewMatrix();
    /*
    glm::mat4 View = glm::lookAt(
        cameraPos, // camera position in world Space
        //glm::vec3(0, 0, 1), // where you want to look at in world space
        cameraDir,
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    */
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Projection matrix
    
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 10000.0f);
    //Projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, 0.1f, 100.0f);

    /*
    unsigned int numVertices = vertices.size();
    std::cout << "Number of verticies " << numVertices <<"\n";
    unsigned int numNormals = normals.size();
    std::cout << "Number of normals " << numNormals << "\n";


    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    //glEnableVertexAttribArray(1);

    // normal attribute
    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST); 
    */

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //update view
        glm::mat4 View = ourCamera.GetViewMatrix();

        // draw our first triangle
        ourShader.use();
        ourShader.setMat4("model", Model);
        ourShader.setMat4("view", View);
        ourShader.setMat4("projection", Projection);
        
        // Sends info to shader
        // MVP info
        /*
        GLint model = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(model, 1, GL_FALSE, &Model[0][0]);
        GLint view = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(view, 1, GL_FALSE, &View[0][0]);
        */

        /* SUN */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, t_sun);
        glm::mat4 model_sun;
        ourShader.setMat4("model", model_sun);
        Sun.Draw();

        //glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //GLint u_ModelViewLocation = glGetUniformLocation(ourShader.ID, "u_ModelView");
        //glDrawArrays(GL_TRIANGLES, 0, numVertices);
        // glBindVertexArray(0); // unbind our VA no need to unbind it every time 
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //update camera view
    //0 is free camera
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        cameraView = 0;
    }
    //planet view options

    if (cameraView == 0) {
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            //handle zoom
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                ourCamera.zoomIn();
            }
            else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                ourCamera.zoomOut();
            }
        }
        else {
            //handle camera movement
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                ourCamera.moveUp();
            }
            else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                ourCamera.moveDown();
            }
            else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                ourCamera.moveLeft();
            }
            else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                ourCamera.moveRight();
            }
        }

    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
