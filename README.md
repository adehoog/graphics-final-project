# Final Project
# Model Solar System with OpenGL

## Prerequisites

Before you can run this program, you need to have the following dependencies installed:

- GLFW (OpenGL Framework)
- GLEW (OpenGL Extension Wrangler Library)
- GLM (OpenGL Mathematics Library)
- libstb
- CMake (for building the project)
- A C++ compiler

## Building and Running

1. Clone or download this repository.

2. Open a terminal and navigate to the project directory.

3. Create a build directory for the project:

   ```bash
   make
   make run
   ```
(If the GLFW window fails to open, you may need to run the command ```export DISPLAY=:0``` before ```make run```)

## Camera Options
### Free Camera
By default, free camera is turned on. You can switch from a planet view to free camera by pressing 0. Press the arrow keys (⬅➡⬆⬇) to move the camera in that direction. Hold the Z key and press up or down arrow keys (⬆⬇) to zoom in or out. 

