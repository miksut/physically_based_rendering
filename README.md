# Computer Graphics: Physically-Based Rendering

This repository hosts an implementation of a physically-based rendering (PBR) algorithm for rendering an object in a dark room. The project is linked to a lecture offered by the [Visualization and MultiMedia Lab](https://www.ifi.uzh.ch/en/vmml/teaching.html) in the Department of Informatics at the University of Zurich, Switzerland.

---
## Project Description
In a dark room, the ambient light is close to zero. Therefore, four lights ensure that the object is visible from every angle. This project implements a [bidirectional reflectance distribution function (BRDF)](https://en.wikipedia.org/wiki/Bidirectional_reflectance_distribution_function). This function defines how light is reflected on an opaque surface. The implemented BRDF includes both a specular and a diffuse term. The specular term is based on microfacet theory and assumes that each microfacet acts as a perfect mirror. Concretely, the [GGX/Trowbridge-Reitz distribution](https://learnopengl.com/PBR/Theory) (as a NDF), the [Smith function](https://learnopengl.com/PBR/Theory) (for the joint masking-shadowing function), and [Schlick's approximation](https://learnopengl.com/PBR/Theory) (for modeling the Fresnel reflectance) are considered. For the diffuse term, the [Lambertian BRDF](https://learnopengl.com/PBR/Theory) is applied. Furthermore, the energy conservation rule is met. 

The properties of the surface are described using the *metalness* and *roughness* parameters. Their values are stored as textures. The implementation is done in the [PBR fragment shader](https://github.com/miksut/physically_based_rendering/blob/main/shader/pbr.frag). The application itself builds upon a framework provided by the Visualization and MultiMedia Lab.

The following images show two exemplary renderings.

<img
    src="/res/tracedImage1.png"
    height="250"
    align="left">
    
<img
    src="/res/tracedImage2.png"
    height="250"> 


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
