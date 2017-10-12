// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// C++ standard lib includes
#include <iostream>
#include <string>

// OpenGL lib includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Polygon Core includes
#include <polygon/app/Application.h>
#include <polygon/graphics/Shader.h>
#include <polygon/graphics/texture/Texture2D.h>
