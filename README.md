# Computer Graphics: Physically-Based Rendering

This repository hosts an implementation a physically-based rendering (PBR) algorithm for rendering an object in a dark room. The project is linked to a lecture offered by the [Visualization and MultiMedia Lab](https://www.ifi.uzh.ch/en/vmml/teaching.html) in the Department of Informatics at the University of Zurich, Switzerland.

---
## Project Description

The application builds upon a framework provided by the Visualization and MultiMedia Lab.

---

## Installation and Setup
This program has the following dependencies:

 1. OpenGL
 2. GLEW
 3. GLFW
 4. FreeImage
 5. Assimp

### Dependencies
OpenGL should already exist on the platform. The Assimp source code is included in this repository and will be built the first time when building this project.

**on Mac:**

Install the remaining three dependencies using homebrew:
```
brew install glew
brew install glfw3
brew install freeimage
```

**on Windows:**
All dependencies are included in this repository. When running CMake, choose "Visual Studio xx WIN64" or choose "Visual Studio xx" as the target platform. In the second case, make sure to definitely choose x64 in the next field.

**on Linux:**

Install the remaining three dependencies using apt-get:
```
sudo apt-get install libglew-dev
sudo apt-get install libglfw3-dev
sudo apt-get install libfreeimage3 libfreeimage-dev
```

### Build
After installing the dependencies, use CMake to build the project. Make sure that the build folder is inside the project folder:

```
[project root]
	|-- build
	|-- CMake
	|-- doc
	|-- external
	|-- res
	|-- shader
	|-- src
	
```
---
