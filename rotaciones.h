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


//funcion para rotar la columna en X
glm::vec3 girarX(float grados, glm::vec3 vertex)
{
	float x, y, z, w =1;
	string nombre;
	float matrizX[4][4] = {{1, 0, 0, 0}, {0, cos(grados), sin(-grados), 0}, {0, sin(grados), cos(grados), 0}, {0, 0, 0, 1}};
	float resultado[4];
	float mover[4];

	x = vertex.x;
	y = vertex.y;
	z = vertex.z;
	
	float vector[4] = {x, y, z, w};
	float centro[4][4] = {{1, 0, 0, (-x/2)}, {0, 1, 0, (-y/2)}, {0, 0, 1, (-z/2)}, {0, 0, 0, 1}};  //creamos la matriz para llevar el vertice al centro
	float regresar[4][4] = { {1, 0, 0, (x/2)}, {0, 1, 0, (y/2)}, {0, 0, 1, (z/2)}, {0, 0, 0, 1} }; //creamos la matriz para regresar el vertice a su posicion original
		
	mover[0] = ( centro[0][0]* vector[0] + centro[0][1] * vector[1] + centro[0][2] * vector[2] + centro[0][3] * vector[3] ); //movemos los valores
	mover[1] = ( centro[1][0]* vector[0] + centro[1][1] * vector[1] + centro[1][2] * vector[2] + centro[1][3] * vector[3] ); //de X, Y, Z al centro
	mover[2] = ( centro[2][0]* vector[0] + centro[2][1] * vector[1] + centro[2][2] * vector[2] + centro[2][3] * vector[3] ); 
	mover[3] = ( centro[3][0]* vector[0] + centro[3][1] * vector[1] + centro[3][2] * vector[2] + centro[3][3] * vector[3] );
		
	resultado[0] = ( (matrizX[0][0] * mover[0]) + (matrizX[0][1] * mover[1]) + (matrizX[0][2] * mover[2]) + (matrizX[0][3] * mover[3]) ); //realizamos la rotacion
	resultado[1] = ( (matrizX[1][0] * mover[0]) + (matrizX[1][1] * mover[1]) + (matrizX[1][2] * mover[2]) + (matrizX[1][3] * mover[3]) ); //del vertice desde el 
	resultado[2] = ( (matrizX[2][0] * mover[0]) + (matrizX[2][1] * mover[1]) + (matrizX[2][2] * mover[2]) + (matrizX[2][3] * mover[3]) ); //punto origen o el centro
	resultado[3] = ( (matrizX[3][0] * mover[0]) + (matrizX[3][1] * mover[1]) + (matrizX[3][2] * mover[2]) + (matrizX[3][3] * mover[3]) ); 
		
  mover[0] = ( regresar[0][0]* resultado[0] + regresar[0][1] * resultado[1] + regresar[0][2] * resultado[2] + regresar[0][3] * resultado[3] ); //regresamos el vertice
	mover[1] = ( regresar[1][0]* resultado[0] + regresar[1][1] * resultado[1] + regresar[1][2] * resultado[2] + regresar[1][3] * resultado[3] ); //rotado ya a su posicion
	mover[2] = ( regresar[2][0]* resultado[0] + regresar[2][1] * resultado[1] + regresar[2][2] * resultado[2] + regresar[2][3] * resultado[3] ); //original
	mover[3] = ( regresar[3][0]* resultado[0] + regresar[3][1] * resultado[1] + regresar[3][2] * resultado[2] + regresar[3][3] * resultado[3] ); 
	
	glm::vec3 res = glm::vec3(mover[0], mover[1], mover[2]);
	return res;
}




//funcion para girar la columna en Y
glm::vec3 girarY(float grados, glm::vec3 vertex)
{
	float x, y, z, w =1;
	string nombre;
	float matrizY[4][4] = {{cos(grados), 0, sin(grados), 0}, {0, 1, 0, 0}, {sin(-grados), 0, cos(grados), 0}, {0, 0, 0, 1}};
	float resultado[4];
	float mover[4];

	x = vertex.x;
	y = vertex.y;
	z = vertex.z;
	
	float vector[4] = {x, y, z, w};
	float centro[4][4] = {{1, 0, 0, (-x/2)}, {0, 1, 0, (-y/2)}, {0, 0, 1, (-z/2)}, {0, 0, 0, 1}};  //creamos la matriz para llevar el vertice al centro
	float regresar[4][4] = { {1, 0, 0, (x/2)}, {0, 1, 0, (y/2)}, {0, 0, 1, (z/2)}, {0, 0, 0, 1} }; //creamos la matriz para regresar el vertice a su posicion original
		
	mover[0] = ( centro[0][0]* vector[0] + centro[0][1] * vector[1] + centro[0][2] * vector[2] + centro[0][3] * vector[3] ); //movemos los valores
	mover[1] = ( centro[1][0]* vector[0] + centro[1][1] * vector[1] + centro[1][2] * vector[2] + centro[1][3] * vector[3] ); //de X, Y, Z al centro
	mover[2] = ( centro[2][0]* vector[0] + centro[2][1] * vector[1] + centro[2][2] * vector[2] + centro[2][3] * vector[3] ); 
	mover[3] = ( centro[3][0]* vector[0] + centro[3][1] * vector[1] + centro[3][2] * vector[2] + centro[3][3] * vector[3] );

	resultado[0] = ( (matrizY[0][0] * mover[0]) + (matrizY[0][1] * mover[1]) + (matrizY[0][2] * mover[2]) + (matrizY[0][3] * mover[3]) );
	resultado[1] = ( (matrizY[1][0] * mover[0]) + (matrizY[1][1] * mover[1]) + (matrizY[1][2] * mover[2]) + (matrizY[1][3] * mover[3]) );
	resultado[2] = ( (matrizY[2][0] * mover[0]) + (matrizY[2][1] * mover[1]) + (matrizY[2][2] * mover[2]) + (matrizY[2][3] * mover[3]) );
	resultado[3] = ( (matrizY[3][0] * mover[0]) + (matrizY[3][1] * mover[1]) + (matrizY[3][2] * mover[2]) + (matrizY[3][3] * mover[3]) );

	mover[0] = ( regresar[0][0]* resultado[0] + regresar[0][1] * resultado[1] + regresar[0][2] * resultado[2] + regresar[0][3] * resultado[3] ); //regresamos el vertice
	mover[1] = ( regresar[1][0]* resultado[0] + regresar[1][1] * resultado[1] + regresar[1][2] * resultado[2] + regresar[1][3] * resultado[3] ); //rotado ya a su posicion
	mover[2] = ( regresar[2][0]* resultado[0] + regresar[2][1] * resultado[1] + regresar[2][2] * resultado[2] + regresar[2][3] * resultado[3] ); //original
	mover[3] = ( regresar[3][0]* resultado[0] + regresar[3][1] * resultado[1] + regresar[3][2] * resultado[2] + regresar[3][3] * resultado[3] ); 
	
	glm::vec3 res = glm::vec3(mover[0], mover[1], mover[2]);
	return res;
}




//funcion para girar la columna en Z
glm::vec3 girarZ(float grados, glm::vec3 vertex)
{
	float x, y, z, w =1;
	string nombre;
	float matrizZ[4][4] = {{cos (grados), sin (-grados), 0, 0}, {sin(grados), cos(grados), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	float resultado[4];
	float mover[4];

	x = vertex.x;
	y = vertex.y;
	z = vertex.z;
	
	float vector[4] = {x, y, z, w};
	float centro[4][4] = {{1, 0, 0, (-x/2)}, {0, 1, 0, (-y/2)}, {0, 0, 1, (-z/2)}, {0, 0, 0, 1}};  //creamos la matriz para llevar el vertice al centro
	float regresar[4][4] = { {1, 0, 0, (x/2)}, {0, 1, 0, (y/2)}, {0, 0, 1, (z/2)}, {0, 0, 0, 1} }; //creamos la matriz para regresar el vertice a su posicion original
		
	mover[0] = ( centro[0][0]* vector[0] + centro[0][1] * vector[1] + centro[0][2] * vector[2] + centro[0][3] * vector[3] ); //movemos los valores
	mover[1] = ( centro[1][0]* vector[0] + centro[1][1] * vector[1] + centro[1][2] * vector[2] + centro[1][3] * vector[3] ); //de X, Y, Z al centro
	mover[2] = ( centro[2][0]* vector[0] + centro[2][1] * vector[1] + centro[2][2] * vector[2] + centro[2][3] * vector[3] ); 
	mover[3] = ( centro[3][0]* vector[0] + centro[3][1] * vector[1] + centro[3][2] * vector[2] + centro[3][3] * vector[3] );


	resultado[0] = ( (matrizZ[0][0] * mover[0]) + (matrizZ[0][1] * mover[1]) + (matrizZ[0][2] * mover[2]) + (matrizZ[0][3] * mover[3]) );
	resultado[1] = ( (matrizZ[1][0] * mover[0]) + (matrizZ[1][1] * mover[1]) + (matrizZ[1][2] * mover[2]) + (matrizZ[1][3] * mover[3]) );
	resultado[2] = ( (matrizZ[2][0] * mover[0]) + (matrizZ[2][1] * mover[1]) + (matrizZ[2][2] * mover[2]) + (matrizZ[2][3] * mover[3]) );
	resultado[3] = ( (matrizZ[3][0] * mover[0]) + (matrizZ[3][1] * mover[1]) + (matrizZ[3][2] * mover[2]) + (matrizZ[3][3] * mover[3]) );

	mover[0] = ( regresar[0][0]* resultado[0] + regresar[0][1] * resultado[1] + regresar[0][2] * resultado[2] + regresar[0][3] * resultado[3] ); //regresamos el vertice
	mover[1] = ( regresar[1][0]* resultado[0] + regresar[1][1] * resultado[1] + regresar[1][2] * resultado[2] + regresar[1][3] * resultado[3] ); //rotado ya a su posicion
	mover[2] = ( regresar[2][0]* resultado[0] + regresar[2][1] * resultado[1] + regresar[2][2] * resultado[2] + regresar[2][3] * resultado[3] ); //original
	mover[3] = ( regresar[3][0]* resultado[0] + regresar[3][1] * resultado[1] + regresar[3][2] * resultado[2] + regresar[3][3] * resultado[3] ); 

	glm::vec3 res = glm::vec3(mover[0], mover[1], mover[2]);
	return res;
}
