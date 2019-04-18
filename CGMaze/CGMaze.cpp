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
"uniform float faktor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"

"void main(){\n"
"gl_Position = projection*view*model*vec4(aPos*faktor, 1.0);\n"
"ourColor = aColor;\n"
"}";

//"gl_Position = vec4(aPos, 1.0);\n"
//"gl_Position=vec4(aPos.x/faktor,aPos.y/faktor,aPos.z/faktor,1.0);\n"

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

	
	Vertex a, b, c,d,e, f;
	a.x = -0.5; a.y = -0.5; a.z = 0; a.r = 1; a.b = 0; a.g=0;
	b.x = 0.5; b.y = -0.5; b.z = 0; b.r = 0; b.b = 1; b.g = 0;
	c.x = 0; c.y = 0.5; c.z = 0; c.r = 0; c.b = 0; c.g = 1;
	d.x = 0.65; d.y = 0.75; d.z = -0.5; d.r = 1; d.g = 1; d.b = 0;
	e.x = -0.75; e.y = 0.75;e.z = 0; e.r = 0; e.b = 1;e.g = 1;
	f.x = 0.65; f.y = 0.9; f.z = 0; f.r = 1;f.b = 1;f.g = 0;
	
	VertexList Vlist(vertexCoordinates|vertexColor, 3);
bool successful=	
successful = Vlist.addTriangle(d, e, f);
successful = Vlist.addTriangle(a,b,c);
Vlist.addTriangle(c,e,f);Vlist.addTriangle(a,f,c);successful = Vlist.addTriangle(a, c,e);

	//VertexList Vlist(vertexCoordinates,8);


//if (successful)
//{
//	puts("Verts added successfully");
//}
//else { puts("Vertex insertion failed"); }


	
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

	Vlist = theGame.getVertexList();

	//puts("triangle generated successfully");
	int icount = Vlist.getIndexCount();
	int vcount = Vlist.getVertexCount();
	//printf("IndexCount: %d, VertexCount: %d", icount, vcount);

	float* tverts = Vlist.getVertexData();
	unsigned int *tindices = Vlist.getIndizes();
	
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Vlist.getStride() * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	
	if (Vlist.getContainsVertexColor()) {
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, Vlist.getStride() * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	if (Vlist.getContainsUVCoordinates())
	{
		if (Vlist.getContainsVertexColor())
		{
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Vlist.getStride() * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
		}
		if (!Vlist.getContainsVertexColor())
		{
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Vlist.getStride() * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
		}
	}


	/* Lets play with EBO*/
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*Vlist.getIndexCount(),tindices,GL_STATIC_DRAW);


	//puts("indizes to render");
	//unsigned int * tin = Vlist.getIndizes();
	//for (int i = 0; i < Vlist.getIndexCount();i++)
	//{
	//	printf("%d ", *(tin + i));
	//}
	//puts("\ndone\n");
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*9, tindices, GL_STATIC_DRAW);

	/*and there*/



	glEnable(GL_DEPTH_TEST);



	/*Set up the matrices*/
	glm::mat4 model = glm::mat4(1.0f); // init to something clearly defined
	

	//generate the base view matrix
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	//translate the whole scene
	viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f,0.1f,-3.2f));
	model = glm::rotate(model, glm::radians(145.0f), glm::vec3(1,0,0));
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, -0.05f));
//for (int i = 0; i < 4;++i)
//	{
//		for (int j = 0; j < 4; ++j)
//		{
//			//if (i == j)
//			//{
//			//	model[i][j]= 1;
//			//}
//			printf("%f ", viewMatrix[j][i]);
//		}
//		puts("");
//	}
	//projection Matrix
	glm::mat4 projectionMatrix=glm::mat4(1.0f);
	projectionMatrix = glm::perspective(glm::radians(45.0f), 800/600.0f, 0.1f,100.0f);


	/**/

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgram);
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc,1, GL_FALSE, glm::value_ptr(model));
		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMatrix[0][0]);
		unsigned int project = glGetUniformLocation(shaderProgram,"projection");
		glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		unsigned int myFaktor = glGetUniformLocation(shaderProgram, "faktor");
		glUniform1f(myFaktor, 0.75f);

	
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0,3);
		glDrawElements(GL_TRIANGLES, Vlist.getIndexCount(), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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
