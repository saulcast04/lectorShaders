#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <fstream>
#include <list>
#include <math.h>
#include <vector>

#include "modelo.h"
#include "lectorTextura.h"
//#include "datos.h"
//#include "matrizPlano.h"

struct vertice
{
	float x;
	float y;
	float z;
	int indiceVer;
};

struct normalVer
{
	float nx;
	float ny;
	float nz;
	int indiceNormal;
};

struct textura
{
	float tx;
	float ty;
	int indiceTex;
};


using namespace std;


modelo cargarObj(string nombreArchivo, const char *nombreTextura)
{
	vector <vertice> listaVertices; 					//lista donde se almacenarán las coordenadas de los vertices
	vector <normalVer> listaNormalVer;				//lista donde se almacenaran las normales de los vertices
	vector <textura> listaTexturas;						//lista donde se almacenaran las coordenadas de las texturas

	vector <int> indicesVertices;							//Lista para almacenar los indices de los vertices
	vector <int> indicesNormales;							//Lista para almacenar los indices de las normales de los vertices
	vector <int> indicesTexturas;							//lista para almacenar los indices de las texturas

	vector <vertice> verticesAux;							//Lista que guardará los vertices conforme aparezca en los indices
	vector <normalVer> normalesAux;						//Lista que guardará las normales de los vertices conforme aparezca en los indices	
	vector <textura> texturaAux;							//lista que guardará las texturas conforme aparezcan en los indices

	int cuentaDatos, indiceVer[3], indiceNormal[3], indiceText[3];
	int idCoord, idNorm, idTex;
	char diagonales[6];
	float x, y, z; 			//ubicacion del vertice
	float vx, vy, vz; 	//Coordenadas de la normal del vertice
	float tu, tv;				//ubicacion de las texturas
	ifstream archivo;
	string nombre, texto, info, nomObj;
	
	modelo nuevoModelo;
	
	cuentaDatos = 0;
	
	archivo.open(nombreArchivo, ios::in);
	
	if(archivo.fail())
	{
		cout << "Error, no se encontro el archivo" << endl;
		exit(1);
	}
	
	idCoord = 0;
	idNorm = 0;
	idTex = 0;
	
	while(!archivo.eof())
	{
		getline(archivo, texto);
		
		if(texto[0] == 'v' && texto [1] == ' ')
		{
			idCoord++;
			stringstream coordenadas; //dato para guardar la linea actual del documento
			info = texto.substr(2);
			coordenadas << info;
			coordenadas >> x >> y >> z;
			vertice nuevoVertice; //se crea un dato de tipo vertice
			nuevoVertice.x = x;
			nuevoVertice.y = y;
			nuevoVertice.z = z;
			nuevoVertice.indiceVer = idCoord;
			
			listaVertices.push_back(nuevoVertice); //almacenamos el dato
			cuentaDatos++;
		}
		
		if(texto[0] == 'v' && texto[1] == 't')
		{
			idTex++;
			stringstream coord;
			info = texto.substr(3);
			coord << info;
			coord >> tu >> tv;
			textura nuevaTextura; //se crea un dato de tipo textura
			nuevaTextura.tx = tu;
			nuevaTextura.ty = tv * (-1);
			nuevaTextura.indiceTex = idTex;
			
			listaTexturas.push_back(nuevaTextura);
			cuentaDatos++;
		}
		
		if(texto[0] == 'v' && texto[1] == 'n')
		{
			idNorm ++;
			stringstream normales; //dato para guardar la linea actual del documento
			info = texto.substr(3);
			normales << info;
			normales >> vx >> vy >> vz;
			normalVer nuevaNormal; //se crea un dato de tipo normalVer
			nuevaNormal.nx = vx;
			nuevaNormal.ny = vy;
			nuevaNormal.nz = vz;
			nuevaNormal.indiceNormal = idNorm;
			
			listaNormalVer.push_back(nuevaNormal); //almacenamos el dato
			cuentaDatos++;
		}
		
		if(texto[0] == 'f')
		{
			stringstream indice;
			info = texto.substr(2);
			indice << info;
			while(!indice.eof())
			{
				/*EXPLICACION DE LA SIGUIENTE LINEA
				 * en nuestro archivo se encontraran los indices ordenados de la siguiente forma
				 * 
				 * indice//indice indice//indice indice//indice
				 * 
				 * por lo que el primer indice indica la coordenada del vertice y el segundo el de la normal
				 * lo que se hace en este caso es almacenar los indices de los vertices en un arreglo
				 * los caracteres de la diagonal en otro arreglo para extraerlos
				 * y los indices de las normales
				 * Al final se almacenaran los indices de los vertices y de las normales en diferentes listas*/
				indice >> indiceVer[0] >> diagonales[0] >> indiceText[0] >> diagonales[1] >> indiceNormal[0] >> indiceVer[1] >> diagonales[2] >> indiceText[1] >> diagonales[3] >> indiceNormal[1] >> indiceVer[2] >> diagonales[4] >> indiceText[2] >> diagonales[5] >> indiceNormal[2];
				
				indicesVertices.push_back(indiceVer[0]);
				indicesVertices.push_back(indiceVer[1]);
				indicesVertices.push_back(indiceVer[2]);
				indicesNormales.push_back(indiceNormal[0]);
				indicesNormales.push_back(indiceNormal[1]);
				indicesNormales.push_back(indiceNormal[2]);
				indicesTexturas.push_back(indiceText[0]);
				indicesTexturas.push_back(indiceText[1]);
				indicesTexturas.push_back(indiceText[2]);
			}
		}
	}
	
	archivo.close();
	
	/*Parte del codigo que ordena los vertices como aparecen en los indices*/
	vector <vertice> :: iterator verIt;
	vector <normalVer> :: iterator normalIt;
	vector <textura> :: iterator textureIt;
	vector <int> :: iterator indicesIt;


	indicesIt = indicesVertices.begin();
	
	/*procedemos a almacenar los vertices de forma en que aparecen en los indices*/
	while(indicesIt != indicesVertices.end())
	{
		verIt = listaVertices.begin();
		while(verIt != listaVertices.end())
		{
			if(verIt->indiceVer == *indicesIt)
			{
				vertice verticeAux;
				
				verticeAux.x = verIt->x;
				verticeAux.y = verIt->y;
				verticeAux.z = verIt->z;
				
				verticesAux.push_back(verticeAux);
			}
			verIt++;
		}
		indicesIt++;
	}
	
	
	indicesIt = indicesNormales.begin();
	/*procedemos a almacenar las normales de los vertices de forma en que aparecen en los indices*/
	while(indicesIt != indicesNormales.end())
	{
		normalIt = listaNormalVer.begin();
		while(normalIt != listaNormalVer.end())
		{
			if(normalIt->indiceNormal == *indicesIt)
			{
				normalVer normalAux;
				
				normalAux.nx = normalIt->nx;
				normalAux.ny = normalIt->ny;
				normalAux.nz = normalIt->nz;
				
				normalesAux.push_back(normalAux);
			}
			normalIt++;
		}
		indicesIt++;
	}
	
	indicesIt = indicesTexturas.begin();
	/*procedemos a almacenar las coordenadas de la textura de forma en la que aparecen en los indices*/
	while(indicesIt != indicesTexturas.end())
	{
		textureIt = listaTexturas.begin();
		while(textureIt != listaTexturas.end())
		{
			if(textureIt->indiceTex == *indicesIt)
			{
				textura textAux;
				textAux.tx = textureIt->tx;
				textAux.ty = textureIt->ty;
				
				texturaAux.push_back(textAux);
			}
			textureIt++;
		}
		indicesIt++;
	}

	float minX = 0;
	float minY = 0;
	float minZ = 0;
	float maxX = 0;
	float maxY = 0;
	float maxZ = 0;
	float centroX, centroY, centroZ;
	
	/*CICLO PARA ENCONTRAR EL VALOR MINIMO DE X*/
	verIt = verticesAux.begin();
	while(verIt != verticesAux.end())
	{
		if(verIt->x < minX)
			minX = verIt->x;
		
		verIt++;
	}
	/*CICLO PARA ENCONTRAR EL VALOR MINIMO DE Y*/
	verIt = verticesAux.begin();
	while(verIt != verticesAux.end())
	{
		if(verIt->y < minY)
			minY = verIt->y;
			
		verIt++;
	}
	/*CICLO PARA ENCONTRAR EL VALOR MINIMO DE Z*/
	verIt = verticesAux.begin();
	while(verIt != verticesAux.end())
	{
		if(verIt->z < minZ)
			minZ = verIt->z;
			
		verIt++;
	}
	/*CICLO PARA ENCONTRAR EL VALOR MAXIMO DE X*/
	verIt = verticesAux.begin();
	while(verIt != verticesAux.end())
	{
		if(verIt->x > maxX)
			maxX = verIt->x;
			
		verIt++;
	}
	/*CICLO PARA ENCONTRAR EL VALOR MAXIMO DE Y*/
	verIt = verticesAux.begin();
	while(verIt != verticesAux.end())
	{
		if(verIt->y > maxY)
			maxY = verIt->y;
			
		verIt++;
	}
	/*CICLO PARA ENCONTRAR EL VALOR MAXIMO DE Z*/
	verIt = verticesAux.begin();
	while(verIt != verticesAux.end())
	{
		if(verIt->z > maxZ)
			maxZ = verIt->z;
			
		verIt++;
	}
	
	centroX = (maxX + minX) / 2;
	centroY = (maxY + minY) / 2;
	centroZ = (maxZ + minZ) / 2;
		
	int tamTotal = verticesAux.size() * 8;//6; //calculamos el numero total de vertices del arreglo
	
	GLfloat* arreglo = (GLfloat*)malloc(tamTotal * sizeof(GLfloat)); //creamos el arreglo

	verIt = verticesAux.begin();
	normalIt = normalesAux.begin();
	textureIt = texturaAux.begin();
	int cont = 0;
	
	/*CICLO DONDE SE ALMACENARÁN LOS DATOS ORDENADOS AL ARREGLO */
	while(verIt != verticesAux.end() && normalIt != normalesAux.end() && textureIt != texturaAux.end())
	{
		arreglo[cont] = verIt->x;
		cont++;
		
		arreglo[cont] = verIt->y;
		cont++;
		
		arreglo[cont] = verIt->z;
		cont++;
		
		arreglo[cont] = normalIt->nx;
		cont++;
		
		arreglo[cont] = normalIt->ny;
		cont++;
		
		arreglo[cont] = normalIt->nz;
		cont++;
		
		arreglo[cont] = textureIt->tx;
		cont++;
		
		arreglo[cont] = textureIt->ty;
		cont++;
		
		verIt++;
		normalIt++;
		textureIt++;
	}	

	
	GLuint vao, vbo;
	const GLfloat* bufferconst = arreglo;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*VAO*/
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*VBO*/
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * tamTotal, bufferconst, GL_STATIC_DRAW); //carga los datos del arreglo en el buffer
	
	/*POSICION*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat*) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	
	/*NORMAL*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat*) * 8, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	
	/*TEXTURA*/
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat*) * 8, (void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);	
	
	
	/*GUARDAMOS TODOS LOS DATOS DE NUESTRO OBJETO CREADO*/
	nuevoModelo.matrizModelo = glm::mat4(1.0f);
	nuevoModelo.numeroVertices = tamTotal;
	nuevoModelo.vertexArrayId = vao;
	nuevoModelo.vertexBufferId = vbo;
	nuevoModelo.idTexture = cargarTextura(nombreTextura);
	nuevoModelo.centroEsfera = glm::vec3(centroX, centroY, centroZ);
	
	return nuevoModelo;
}










modelo generarPlano(string nombreArchivo, const char *nombreTextura, const char *nombreMapa)
{
	vector <vertice> listaVertices; 					//lista donde se almacenarán las coordenadas de los vertices
	vector <normalVer> listaNormalVer;				//lista donde se almacenaran las normales de los vertices
	vector <textura> listaTexturas;						//lista donde se almacenaran las coordenadas de las texturas

	vector <int> indicesVertices;							//Lista para almacenar los indices de los vertices
	vector <int> indicesNormales;							//Lista para almacenar los indices de las normales de los vertices
	vector <int> indicesTexturas;							//lista para almacenar los indices de las texturas

	vector <vertice> verticesAux;							//Lista que guardará los vertices conforme aparezca en los indices
	vector <normalVer> normalesAux;						//Lista que guardará las normales de los vertices conforme aparezca en los indices	
	vector <textura> texturaAux;							//lista que guardará las texturas conforme aparezcan en los indices
	

	int cuentaDatos, indiceVer[3], indiceNormal[3], indiceText[3];
	int idCoord, idNorm, idTex;
	char diagonales[6];
	float x, y, z; 			//ubicacion del vertice
	float vx, vy, vz; 	//Coordenadas de la normal del vertice
	float tu, tv;				//ubicacion de las texturas
		
	vector <GLubyte> alturas = cargarAlturas(nombreMapa);
	int contador = 0;
	
	float elevacion;			//cuanto se elevara el terreno, esto se define de la siguiente forma
												//blanco = 0, gris = 1 y blanco  = 2
	
	ifstream archivo;
	string nombre, texto, info, nomObj;
		
	cuentaDatos = 0;
	
	archivo.open(nombreArchivo, ios::in);
	
	if(archivo.fail())
	{
		cout << "Error, no se encontro el archivo" << endl;
		exit(1);
	}
	
	idCoord = 0;
	idNorm = 0;
	idTex = 0;
	
	while(!archivo.eof())
	{
		getline(archivo, texto);
		
		if(texto[0] == 'v' && texto [1] == ' ')
		{
			if(alturas[contador]/255.0f == 0)
				{
					elevacion = -2;
				}
			else
			{
				if(alturas[contador]/255.0f == 0.5)
				{
					elevacion = 0;
				}
				else
				{
					if(alturas[contador]/255.0f == 1)
						elevacion = 2;
				}
			}
			
			idCoord++;
			stringstream coordenadas; //dato para guardar la linea actual del documento
			info = texto.substr(2);
			coordenadas << info;
			coordenadas >> x >> y >> z;
			vertice nuevoVertice; //se crea un dato de tipo vertice
			nuevoVertice.x = x;
			nuevoVertice.y = y + elevacion;
			nuevoVertice.z = z;
			nuevoVertice.indiceVer = idCoord;
			
			listaVertices.push_back(nuevoVertice); //almacenamos el dato
			cuentaDatos++;
			contador++;
		}
		
		if(texto[0] == 'v' && texto[1] == 't')
		{
			idTex++;
			stringstream coord;
			info = texto.substr(3);
			coord << info;
			coord >> tu >> tv;
			textura nuevaTextura; //se crea un dato de tipo textura
			nuevaTextura.tx = tu;
			nuevaTextura.ty = tv * (-1);
			nuevaTextura.indiceTex = idTex;
			
			listaTexturas.push_back(nuevaTextura);
			cuentaDatos++;
		}
		
		if(texto[0] == 'v' && texto[1] == 'n')
		{
			idNorm ++;
			stringstream normales; //dato para guardar la linea actual del documento
			info = texto.substr(3);
			normales << info;
			normales >> vx >> vy >> vz;
			normalVer nuevaNormal; //se crea un dato de tipo normalVer
			nuevaNormal.nx = vx;
			nuevaNormal.ny = vy;
			nuevaNormal.nz = vz;
			nuevaNormal.indiceNormal = idNorm;
			
			listaNormalVer.push_back(nuevaNormal); //almacenamos el dato
			cuentaDatos++;
		}
		
		if(texto[0] == 'f')
		{
			stringstream indice;
			info = texto.substr(2);
			indice << info;
			while(!indice.eof())
			{
				/*EXPLICACION DE LA SIGUIENTE LINEA
				 * en nuestro archivo se encontraran los indices ordenados de la siguiente forma
				 * 
				 * indice//indice indice//indice indice//indice
				 * 
				 * por lo que el primer indice indica la coordenada del vertice y el segundo el de la normal
				 * lo que se hace en este caso es almacenar los indices de los vertices en un arreglo
				 * los caracteres de la diagonal en otro arreglo para extraerlos
				 * y los indices de las normales
				 * Al final se almacenaran los indices de los vertices y de las normales en diferentes listas*/
				indice >> indiceVer[0] >> diagonales[0] >> indiceText[0] >> diagonales[1] >> indiceNormal[0] >> indiceVer[1] >> diagonales[2] >> indiceText[1] >> diagonales[3] >> indiceNormal[1] >> indiceVer[2] >> diagonales[4] >> indiceText[2] >> diagonales[5] >> indiceNormal[2];
				
				indicesVertices.push_back(indiceVer[0]);
				indicesVertices.push_back(indiceVer[1]);
				indicesVertices.push_back(indiceVer[2]);
				indicesNormales.push_back(indiceNormal[0]);
				indicesNormales.push_back(indiceNormal[1]);
				indicesNormales.push_back(indiceNormal[2]);
				indicesTexturas.push_back(indiceText[0]);
				indicesTexturas.push_back(indiceText[1]);
				indicesTexturas.push_back(indiceText[2]);
			}
		}
	}
	
	archivo.close();
	
	/*Parte del codigo que ordena los vertices como aparecen en los indices*/
	vector <vertice> :: iterator verIt;
	vector <normalVer> :: iterator normalIt;
	vector <textura> :: iterator textureIt;
	vector <int> :: iterator indicesIt;
		
	indicesIt = indicesVertices.begin();
	
	/*procedemos a almacenar los vertices de forma en que aparecen en los indices*/
	while(indicesIt != indicesVertices.end())
	{
		verIt = listaVertices.begin();
		while(verIt != listaVertices.end())
		{
			if(verIt->indiceVer == *indicesIt)
			{
				vertice verticeAux;
				
				verticeAux.x = verIt->x;
				verticeAux.y = verIt->y;
				verticeAux.z = verIt->z;
				
				verticesAux.push_back(verticeAux);
			}
			verIt++;
		}
		indicesIt++;
	}
	
	
	indicesIt = indicesNormales.begin();
	/*procedemos a almacenar las normales de los vertices de forma en que aparecen en los indices*/
	while(indicesIt != indicesNormales.end())
	{
		normalIt = listaNormalVer.begin();
		while(normalIt != listaNormalVer.end())
		{
			if(normalIt->indiceNormal == *indicesIt)
			{
				normalVer normalAux;
				
				normalAux.nx = normalIt->nx;
				normalAux.ny = normalIt->ny;
				normalAux.nz = normalIt->nz;
				
				normalesAux.push_back(normalAux);
			}
			normalIt++;
		}
		indicesIt++;
	}
	
	indicesIt = indicesTexturas.begin();
	/*procedemos a almacenar las coordenadas de la textura de forma en la que aparecen en los indices*/
	while(indicesIt != indicesTexturas.end())
	{
		textureIt = listaTexturas.begin();
		while(textureIt != listaTexturas.end())
		{
			if(textureIt->indiceTex == *indicesIt)
			{
				textura textAux;
				textAux.tx = textureIt->tx;
				textAux.ty = textureIt->ty;
				
				texturaAux.push_back(textAux);
			}
			textureIt++;
		}
		indicesIt++;
	}
	
	
	int tamTotal = verticesAux.size() * 8;//6; //calculamos el numero total de vertices del arreglo
	
	GLfloat* arreglo = (GLfloat*)malloc(tamTotal * sizeof(GLfloat)); //creamos el arreglo

	verIt = verticesAux.begin();
	normalIt = normalesAux.begin();
	textureIt = texturaAux.begin();
	int cont = 0;
	
	/*CICLO DONDE SE ALMACENARÁN LOS DATOS ORDENADOS AL ARREGLO */
	while(verIt != verticesAux.end() && normalIt != normalesAux.end() && textureIt != texturaAux.end())
	{
		arreglo[cont] = verIt->x;
		cont++;
		
		arreglo[cont] = verIt->y;
		cont++;
		
		arreglo[cont] = verIt->z;
		cont++;
		
		arreglo[cont] = normalIt->nx;
		cont++;
		
		arreglo[cont] = normalIt->ny;
		cont++;
		
		arreglo[cont] = normalIt->nz;
		cont++;
		
		arreglo[cont] = textureIt->tx;
		cont++;
		
		arreglo[cont] = textureIt->ty;
		cont++;
		
		verIt++;
		normalIt++;
		textureIt++;
	}
	
	
	GLuint vao, vbo;
	const GLfloat* bufferconst = arreglo;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*VAO*/
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*VBO*/
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * tamTotal, bufferconst, GL_STATIC_DRAW); //carga los datos del arreglo en el buffer
	
	/*POSICION*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat*) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	
	/*NORMAL*/
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat*) * 8, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	
	/*TEXTURA*/
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat*) * 8, (void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);	
	
	
	/*GUARDAMOS TODOS LOS DATOS DE NUESTRO OBJETO CREADO*/
	modelo nuevoPlano;
	nuevoPlano.matrizModelo = glm::mat4(1.0f);
	nuevoPlano.numeroVertices = tamTotal;
	nuevoPlano.vertexArrayId = vao;
	nuevoPlano.vertexBufferId = vbo;
	nuevoPlano.idTexture = cargarTextura(nombreTextura);

	return nuevoPlano;
}
