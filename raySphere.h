#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <fstream>
#include <list>
#include <math.h>

/*LIBRERIAS MATEMATICAS DE OPENGL*/
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> //glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>


int interseccionRaySphere(glm::vec3 Ro, glm::vec3 Rf, glm::vec3 esfera)
{
	float a, b, c, r = 1.0;
	float delta;
	
	a = ((Rf.x - Ro.x) * (Rf.x - Ro.x)) + ((Rf.y - Ro.y) * (Rf.y - Ro.y)) + ((Rf.z - Ro.z) * (Rf.z - Ro.z));
	b = 2 * (((Rf.x - Ro.x) * (Ro.x - esfera.x)) + ((Rf.y - Ro.y) * (Ro.y - esfera.y)) + ((Rf.z - Ro.z) * (Ro.z - esfera.z)));
	c = ((Ro.x - esfera.x) * (Ro.x - esfera.x)) + ((Ro.y - esfera.y) * (Ro.y - esfera.y)) + ((Ro.z - esfera.z) * (Ro.z - esfera.z)) - (r * r);
	
	delta = (b * b) - 4*(a * c);
	
	return delta;
}
