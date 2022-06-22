#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> //glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

class modelo
{
	public:
		int numeroVertices;
		
		GLuint idShaders;
		GLuint vertexBufferId;
		GLuint vertexArrayId;
		GLuint idTexture;
		glm::vec3 centroEsfera;
		bool colision;
		glm::mat4 matrizModelo;
};
