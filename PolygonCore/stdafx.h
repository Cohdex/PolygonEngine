// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// C++ standard lib includes
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

// OpenGL lib includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Assimp lib includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Polygon Core includes
#include <polygon/app/Application.h>
#include <polygon/graphics/buffer/VertexArray.h>
#include <polygon/graphics/buffer/VertexBuffer.h>
#include <polygon/graphics/buffer/IndexBuffer.h>
#include <polygon/graphics/Shader.h>
#include <polygon/graphics/texture/Texture.h>
#include <polygon/graphics/texture/Texture2D.h>
#include <polygon/graphics/model/Mesh.h>
#include <polygon/graphics/model/Model.h>
#include <polygon/util/Camera.h>
#include <polygon/util/MeshUtil.h>
#include <polygon/util/ObjLoader.h>
