#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <fstream>
#include <list>
#include <math.h>

/*LIBRERIAS DE OPENGL*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shaders.hpp"

/*LIBRERIAS MATEMATICAS DE OPENGL*/
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> //glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

/*OBJETO DEL MODELO Y LA LECTURA*/
#include "modelo.h"
#include "lectorObj.h"
#include "raySphere.h"
#include "rotaciones.h"

using namespace std;

int InitWindow(void);
GLFWwindow* window;
GLuint programId;

glm::mat4x4 model, view, projection;
glm::vec3 lightPos = glm::vec3(0, 300, -500);
glm::vec3 mov(0.0f, -0.5f, -5.0f); //matriz que nos permitirá ver en el mundo
glm::vec3 posCamara(0.0f, -0.5f, -5.0f); //posicion en la que se encuentra la camara originalmente
glm::vec3 avance; 	//matriz que nos permitirá movernos por el mundo
glm::vec3 strafe;		//matriz que nos permitira caminar de forma lateral
glm::vec3 rayo;			//vector que definira el rayo para la interseccion

int ancho = 800, alto = 600; //MEDIDAS DEL TAMAÑO DE LA VENTANA
float velocidad = 0.3f; //velocidad a la que se moverá la camara
float angle = 0.0f;
float pitch = 0.0f;		//rotacion en el eje de las X
float yaw = 0.0f;			//rotacion en el eje de las Y
int camara = 0; 			//varaible para cambiar de camara, 0 = FPS, 1 = TPS = 2 TOP-VIEW

float zoom = 10.0;			//variable que permite controlar el zoom de la camara top-view
bool click;							//variable para detectar que el boton izquierdo del mouse esta siendo presionado
int colision = 0;				//variable que detectará la colisión en los objetos

vector<modelo> listaModelos;	//lista para almacenar los modelos que se usaran en el juego


float angleIncrement = 0;
bool rotA = true;

void teclado(GLFWwindow* window, int key, int scancode, int action, int mods);
void Matrices();
void rotaluz(float angleX, float angleY);
glm::mat4 FPSview(glm::vec3 mov, float pitch, float yaw);
glm::mat4 TPSview(glm::vec3 mov, float pitch, float yaw);
glm::mat4 vistaTopView(float zoom);

void funcionMouse(GLFWwindow* window, double xPos, double yPos);
void botonesMouse(GLFWwindow* window, int button, int action, int mods);



int main(int argc, char **argv)
{
	int fc = 0, unifLoc;
	double t = 0.0f;
	unsigned int cont;

	if (InitWindow() < 0) return -1;
	
	//programId = LoadShaders("C:\\Users\\Roa\\source\\repos\\ShaderLec\\ShaderLec\\vs_basico.glsl", "C:\\Users\\Roa\\source\\repos\\ShaderLec\\ShaderLec\\fs_basico.glsl");

	programId = LoadShaders("C:\\Users\\saul_\\Documents\\proyectos\\lectorShaders2\\vs_phongTexturas.glsl", 
													"C:\\Users\\saul_\\Documents\\proyectos\\lectorShaders2\\fs_phongTexturas.glsl");													
		

	modelo objeto = cargarObj("pruebaCubo.obj", "texturas.jpg");
	objeto.colision = 1;			//asignamos una bandera para las colisiones
	modelo objeto1 = cargarObj("cubo1.obj", "texturas.jpg");
	objeto1.colision = 1;			//asignamos una bandera para las colisiones
	modelo objeto2 = cargarObj("cubo2.obj", "texturas.jpg");
	objeto2.colision = 1;		//asignamos una bandera para las colisiones
	modelo objeto3 = cargarObj("cubo3.obj", "texturas.jpg");
	objeto3.colision = 1;		//asignamos una bandera para las colisiones
	
	modelo herramienta = cargarObj("herramienta.obj", "texturaAcero.jpg");
	modelo prueba = generarPlano("plano.obj", "texturaPasto.jpg", "heightmap3.jpg");
	modelo skybox = cargarObj("skybox.obj", "desertSkybox.jpg");

	
	listaModelos.push_back(herramienta);	
	listaModelos.push_back(objeto);
	listaModelos.push_back(objeto1);
	listaModelos.push_back(objeto2);
	listaModelos.push_back(objeto3);
	listaModelos.push_back(prueba);	
	listaModelos.push_back(skybox);
		
	/*COMIENZA EL CICLO*/
	do
	{
		double initialTime = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Matrices uniformes
		
		rayo = glm::vec3(mov.x, mov.y, mov.z + 1);	//posición actual del rayo
		
		glfwSetKeyCallback(window, teclado);
		glfwSetCursorPosCallback(window, funcionMouse);
		glfwSetMouseButtonCallback(window, botonesMouse);
		
		
		Matrices();
		for(cont = 0; cont < listaModelos.size(); cont++)
		{
			modelo mod = listaModelos[cont];
			glUseProgram(programId);
			
			glBindTexture(GL_TEXTURE_2D, listaModelos[cont].idTexture);
			
			if(click == 1)
			{
				//rayo = glm::vec3(posCamara.x, posCamara.y, posCamara.z + 1);	//posición actual del rayo
				colision = interseccionRaySphere(posCamara, rayo, mod.centroEsfera); //llamada a la función raySphere
				if(colision == 0 && listaModelos[cont].colision == 1)
					cout << "toca el objeto" << endl;
				if(colision > 0  && listaModelos[cont].colision == 1)
				{
					listaModelos[cont].matrizModelo = glm::mat4(0.0f);
					cout << "intersecta al objeto" << endl;
				}
			}
			
			unifLoc = glGetUniformLocation(programId, "model");
			glUniformMatrix4fv(unifLoc, 1, GL_FALSE, glm::value_ptr(mod.matrizModelo));
			unifLoc = glGetUniformLocation(programId, "view");
			glUniformMatrix4fv(unifLoc, 1, GL_FALSE, glm::value_ptr(view));
			unifLoc = glGetUniformLocation(programId, "projection");
			glUniformMatrix4fv(unifLoc, 1, GL_FALSE, glm::value_ptr(projection));
			unifLoc = glGetUniformLocation(programId, "lightPos");
			glUniform3f(unifLoc, lightPos[0], lightPos[1], lightPos[2]);

			glBindVertexArray(mod.vertexArrayId);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		
			glDrawArrays(GL_TRIANGLES, 0, mod.numeroVertices);
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		////////////////////FPS///////////////////////
		double ts = 1.0f/60.0f;
		double diff;
		double finalTime = glfwGetTime();
		
		diff = finalTime - initialTime;
		
		
		while(true)
		{
			if(diff>= ts) break;
			finalTime = glfwGetTime();
			
			diff = finalTime - initialTime;
		}
		
		fc++;
		t += diff;
		if(t >= 1.0f)
		{
			printf("%d\n", fc);
			fc = 0;
			t = 0.0f;
		}
		////////////////////FPS///////////////////////


	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	glfwTerminate();

	return 1;
}





int InitWindow(void)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(ancho, alto, "Test", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	//Glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//glfwSetMouseButtonCallback(window, mouse_button_callback);

	//OpenGl
	glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
	//glViewport(0, 0, 1000, 1000);
	glEnable(GL_DEPTH_TEST);

	return 1;
}



/*FUNCION DE LAS MATRICES DEL MUNDO*/
void Matrices() {
	float tx = 0, ty = 0, tz = 0,
	s = 2,
	rx = 0, ry = 1, rz = 0;
	model = glm::rotate(glm::scale(glm::translate(glm::mat4(1.0), glm::vec3(tx, ty, tz)), glm::vec3(s, s, s)), glm::radians(angle), glm::vec3(rx, ry, rz));
	
	view = FPSview(mov, pitch, yaw); 	//funcion para la vista FPS
	projection = glm::perspective(glm::radians(45.0f), 1080.0f / 960.0f, 0.1f, 300.0f);
}


glm::mat4 FPSview(glm::vec3 mov, float pitch, float yaw)
{

	glm::mat4 objeto = listaModelos[0].matrizModelo; //tomamos la matriz del personaje
	pitch = glm::radians(pitch);
	yaw = glm::radians(yaw);
	
	glm::mat4 m = glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f));	
	m = glm::rotate(m, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	
	glm::mat4 mo = glm::rotate(glm::mat4(1.0f), -pitch, glm::vec3(1.0f, 0.0f, 0.0f)); //matriz 
	mo = glm::rotate(mo, -yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	
	glm::vec3 posicion(-mov[0] +0.0f, -mov[1] -1.0f, -mov[2] -2.0f);
	
	glm::mat4 aPivote= glm::translate(glm::mat4(1.0f), mov);
	glm::mat4 desdePivote=  glm::translate(glm::mat4(1.0f), -mov);
	
	glm::mat4 rotar = desdePivote * mo * aPivote;
	
	avance = glm::vec3(-m[2].x, 0.0f, m[2].z);
	strafe = glm::vec3(-m[2].z, 0.0f, -m[2].x);

	rayo = girarX(pitch, rayo);
	rayo = girarZ(yaw, rayo);
	
	m = glm::translate(m, mov);
	objeto = glm::translate(objeto, mov);
		
	objeto = rotar * glm::translate(glm::mat4(1.0f), posicion) * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));	
		
	listaModelos[0].matrizModelo = objeto;
	
	return m;	
}



void rotaluz(float angleX, float angleY)
{
	using namespace glm;
	float x, y, z, x1, y1, z1;
	x = lightPos[0];
	y = (lightPos[1] * cos(radians(angleX))) - (lightPos[2] * sin(radians(angleX)));
	z = (lightPos[2] * cos(radians(angleX))) + (lightPos[1] * sin(radians(angleX)));
	x1 = (x * cos(radians(angleY))) + (z * sin(radians(angleY)));
	y1 = y;
	z1 = (z * cos(radians(angleY))) - (x * sin(radians(angleY)));
	lightPos = vec3(x1, y1, z1);
}




/*FUNCION PARA HACER USO DEL TECLADO*/
void teclado(GLFWwindow* window, int key, int scancode, int action, int mods)
{
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			yaw -= 0.5f;
			
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			yaw += 0.5f;
		
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			if(pitch > -90.0f)
					pitch -= 0.5f;
		
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			if(pitch < 90.0f)
					pitch += 0.5f;
		
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			mov += avance * velocidad;
			rayo += avance * velocidad;
		}	
			
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			mov -= avance * velocidad;
			rayo -= avance * velocidad;
		}
		
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			mov -= strafe * velocidad;
			rayo -= strafe * velocidad;
		}
		
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{	
			mov += strafe * velocidad;
			rayo += strafe * velocidad;
		}

		
		if(camara == 2)
		{
			if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				if(zoom > 10)
					zoom -= 1.0f;
			
			if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)			
				if(zoom < 50)	
					zoom += 1.0f;
		}
		
}





void funcionMouse(GLFWwindow* window, double xPos, double yPos)
{
	
	glfwSetCursorPos(window, ancho/2, alto/2);
	double x = xPos;
	double y = yPos;


	/*COMPARACIONES PARA MOVER LOS ANGULOS DE LA CAMARA*/		
	if(x > (ancho/2))
		yaw += 1.0f;

	if(x < (ancho/2))
		yaw -= 1.0f;	
	
	if(y > (alto/2))
		if(pitch < 90.0f)
			pitch += 1.0f;

	
	if(y < (alto/2))
		if(pitch > -90.0f)
			pitch -= 1.0f;
		
}

void botonesMouse(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		click = 1;
	else
		click = 0;
}
