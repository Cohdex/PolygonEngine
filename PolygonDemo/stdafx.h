// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// C++ standard lib includes
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <memory>

// OpenGL lib includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// Polygon Core lib includes
#include <polygon/app/Application.h>
#include <polygon/graphics/Shader.h>
#include <polygon/graphics/texture/Texture2D.h>
#include <polygon/util/MeshUtil.h>
#include <polygon/util/ObjLoader.h>

// Polygon Demo includes
