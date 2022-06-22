#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <fstream>
#include <list>
#include <math.h>

#include "modelo.h"

/*BIBLIOTECA PARA LA LECTURA DE TEXTURAS*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



/*FUNCION PARA LEER LAS TEXTURAS*/
GLuint cargarTextura(const char *nombreTextura)
{
	GLuint text;
	
	glGenTextures(1, &text);
	glBindTexture(GL_TEXTURE_2D, text);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, numeroCanales;
	
	unsigned char *info = stbi_load(nombreTextura, &width, &height, &numeroCanales, 0);
	
	if(info)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, info);
	}
	else
	{
		cout << "error, no se encontro la textura" << endl;
	}
	stbi_image_free(info);
	
	return text;
}

/*FUNCION PARA CREAR LA LISTA CON LAS ALTURAS DEL HEIGHTMAP*/
vector <GLubyte> cargarAlturas(const char *nombreMapa)
{
	vector <GLubyte> heightmap;
	int width, height, numeroCanales;
	
	unsigned char *datos = stbi_load(nombreMapa, &width, &height, &numeroCanales, STBI_grey);
	
	if(!datos)
	{
		cout << "Error, no se ha encontrado el mapa de alturas";
	}
	else
	{
		heightmap.insert(heightmap.end(), datos, datos + (width * height));
	}
	
	stbi_image_free(datos);
	
	return heightmap;
}
