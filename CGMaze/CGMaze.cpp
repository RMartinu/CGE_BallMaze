// CGMaze.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "pch.h"
#include <iostream>
#include "ppm.h"
#include "CGMaze.h"
#include "Maze.h"
#include <glad\glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.1f, 0.75f, 0.9f, 1.0f);\n"
"}\n\0";


const char *vertexShaderGradient = "#version 330 core\n"
"layout (location=0) in vec3 aPos;\n"
"layout (location=1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main(){\n"
"gl_Position = vec4(aPos, 1.0);\n"
"ourColor = aColor;\n"
"}";


const char *fragmentShaderGradient = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main(){\n"
"FragColor = vec4(ourColor, 1.0);\n"
"}";


//"FragColor = vec4(ourColor, 1.0);\n"

float vertices[] = {-0.5f,-0.5f, 0.0f,
					0.5f, -0.5f, 0.0f,
					0.0f, 0.5f, 0.0f};
float v2[] = {	0.5f, 0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				-0.5f, -0.5f, 0.0f,
				-0.5, 0.5, 0.0f}
;

float vGradient[] = { -0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
					0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f };


unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0,0, w,h);
}

int main()
{
    std::cout << "Hello World!\n"; 

	//ppmImage thePlan("insert Path here");
	//Maze theGame(thePlan);
	
	ppmImage myImage("Resource\\smiley.ppm");
	pixel p = myImage.getPixel(155,190);

	
	Vertex a, b, c,d;
	a.x = -0.5; a.y = -0.5; a.z = 0; a.r = 1; a.b = 0; a.g;
	b.x = 0.5; b.y = -0.5; b.z = 0; b.r = 0; b.b = 1; b.g = 0;
	c.x = 0; c.y = 0.5; c.z = 0; c.r = 0; c.b = 0; c.g = 1;
	d.x = 0.5; d.y = 0.75; d.z = 0; d.r = 1; d.g = 1; d.b = 0;
	
	VertexList Vlist(vertexCoordinates|vertexColor, 4);
bool successful=	Vlist.addTriangle(a,b,c);
successful = Vlist.addTriangle(b, c, d);
//if (successful)
//{
//	puts("Verts added successfully");
//}
//else { puts("Vertex insertion failed"); }

	//puts("triangle generated successfully");
	int icount = Vlist.getIndexCount();
	int vcount = Vlist.getVertexCount();
	//printf("IndexCount: %d, VertexCount: %d", icount, vcount);

	float* tverts = Vlist.getVertexData();
	unsigned int *tindices = Vlist.getIndizes();
	
	//puts("My indices: ");
	//for (int i = 0; i < icount; ++i)
	//{
	//	printf("%d", *(tindices+i));
	//}
	//puts("\nMy vertices\n");
	//for (int i =0; i<vcount*Vlist.getStride(); i+=Vlist.getStride())
	//{
	//	printf("Coords: %f, %f, %f\tColor: %f, %f, %f\n", *(tverts +i+0), *(tverts + i + 1), *(tverts + i + 2), *(tverts + i + 3), *(tverts + i + 4), *(tverts + i + 5) );
	//}

	//puts("Is the data fine?");


	//printf("r: %d, g: %d, b: %d\n", p.r,p.g,p.b);

	 p = myImage.getPixel(64, 144);
	//printf("r: %d, g: %d, b: %d\n", p.r, p.g, p.b);
	 p = myImage.getPixel(155, 190);
	//printf("r: %d, g: %d, b: %d\n", p.r, p.g, p.b);

	//Level Load Test
	ppmImage theLevel("Resource//minLevel.ppm");
	Maze theGame(theLevel);

	VertexList VxXx = theGame.getVertexList();


	puts("level loaded");


	//done

	//get a vertex buffer
	VertexList VL(vertexCoordinates|UVCoordinates|vertexColor,12);
	//puts("Vertex list built");

	VertexList VL1(vertexCoordinates | UVCoordinates , 12);
	//puts("Vertex list built");

	VertexList VL2(vertexCoordinates | vertexColor, 12);
	//puts("Vertex list built");
	//VL.addVertex(4, 5, 6);
	//VL.extendVertexData();
	//VL.addVertex(1,2,3);
	//float *t = VL.getVertexData();
	//int vcount = VL.getVertexCount();
	//printf("Our list has %d elements\n",vcount);
	//for (int i = 0; i < VL.getStride()*VL.getVertexCount(); ++i)
	//{
	//	printf("val: %f, ", t[i]);
	//}
	//puts("");


	//get list from game

	//VertexList gvl = theGame.getVertexList();
	//printf("+++Our list has %d elements\n", gvl.getVertexCount());

	/*Vector Excersise here; remove at appropriete time*/
	glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f,1.0f, 0.0f));
	vec = trans * vec;
	//std::cout << vec.x <<" "<< vec.y <<" "<< vec.z << std::endl;
	/**/

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800,600,"Our first Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Something went wrong" << std::endl;
		glfwTerminate();
		return -11;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -13;
	}

	glViewport(0,0,800,600);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);



	/*Lets put our OGL stuff btween here*/
	unsigned int VBO;
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*Vlist.getStride()*Vlist.getVertexCount(), tverts, GL_STATIC_DRAW);

	//simple Vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderGradient, NULL);
	glCompileShader(vertexShader);

	//check for compile errors
	int success; char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::CompFail" << infoLog << std::endl;
	}



	//Fragment shader

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderGradient, NULL);
	glCompileShader(fragmentShader);


	//check for compile errors
	int successFS; char infoLogFS[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successFS);
	if (!successFS)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFS);
		std::cout << "ERROR::SHADER::FRAGMENT::CompFail" << infoLogFS << std::endl;
	}


	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//shaders are ready for action

	//Linking vertices
	//glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	//glEnableVertexAttribArray(0);
	



	//make a vertex array object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*Vlist.getStride()*Vlist.getVertexCount(), tverts, GL_STATIC_DRAW);
	//vertex attrib pointer
	//glVertexAttribPointer(0,4,GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	/* Lets play with EBO*/
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*Vlist.getIndexCount(),tindices,GL_STATIC_DRAW);

	/*and there*/


	while (!glfwWindowShouldClose(window))
	{

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0,3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 7;
	 

	
}

// Programm ausführen: STRG+F5 oder "Debuggen" > Menü "Ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
