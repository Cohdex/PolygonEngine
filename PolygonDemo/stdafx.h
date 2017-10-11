// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// C++ standard lib includes
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <memory>

// OpenGL lib includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Polygon Core lib includes
#include <polygon/app/Application.h>
#include <polygon/graphics/Shader.h>

// Polygon Demo includes
#include <include/demo/DemoApplication.h>
