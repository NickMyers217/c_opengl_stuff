#include <stdio.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderProgram.h"
#include "texture.h"
#include "baseModel.h"


void printMajorAndMinorGlVersion();
void processInput(GLFWwindow * window);


const int WIDTH = 800;
const int HEIGHT = 600;


void printMajorAndMinorGlVersion()
{
	gladGLversionStruct version;
	glGetIntegerv(GL_MAJOR_VERSION, &version.major);
	glGetIntegerv(GL_MINOR_VERSION, &version.minor);
	printf("Initializing OpenGL V%i.%i\n", version.major, version.minor);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


int main()
{
	// Window initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MOONMAN", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to initialize a GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// OpenGL initialization
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize a GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
	printMajorAndMinorGlVersion();


	// Shader program creation
	ShaderProgram texProgram;
	shaderProgramBuild(&texProgram, "basic_vertex.glsl", "basic_frag.glsl");
	ShaderProgram blueProgram;
	shaderProgramBuild(&blueProgram, "basic_vertex.glsl", "basic_frag_two.glsl");


	// Texture creation
	Texture container;
	textureInit(&container, "container.jpg", false, false);
	Texture moonman;
	textureInit(&moonman, "moonman.png", true, true);
	shaderProgramUse(&texProgram);
	glUniform1i(glGetUniformLocation(texProgram.id, "texture0"), 0);
	glUniform1i(glGetUniformLocation(texProgram.id, "texture1"), 1);


	// Model creation
	GLfloat VERTICES_ONE[] = {
		// Positions         // Tex Coords
		-1.0f, -0.5f, 0.0f,  0.0f, 0.0f, // Bot Left
		-1.0f,  0.5f, 0.0f,  0.0f, 1.0f, // Top Left
		 0.0f,  0.5f, 0.0f,  1.0f, 1.0f, // Top Right
		 0.0f, -0.5f, 0.0f,  1.0f, 0.0f, // Bot Right

		-1.0f, -0.5f, 0.0f,  0.0f, 0.0f, // Bot Left
		 0.0f,  0.5f, 0.0f,  1.0f, 1.0f, // Top Right
		 0.0f, -0.5f, 0.0f,  1.0f, 0.0f, // Bot Right
	};
	BaseModel planeModel;
	modelInit(&planeModel, VERTICES_ONE, sizeof(VERTICES_ONE) / sizeof(GLfloat));

	GLfloat VERTICES_TWO[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	BaseModel cubeModel;
	modelInit(&cubeModel, VERTICES_TWO, sizeof(VERTICES_TWO) / sizeof(GLfloat));


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Event handling
		processInput(window);


		// Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Transformations
		glm::mat4 modelMat;
		modelMat = glm::rotate(modelMat, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 viewMat;
		viewMat = glm::translate(viewMat, glm::vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 projectionMat = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


		//Rendering
		shaderProgramUse(&texProgram);
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));
		textureUse(&container, 0);
		textureUse(&moonman, 1);
		modelRender(&cubeModel);

		//shaderProgramUse(&blueProgram);
		//glUniformMatrix4fv(glGetUniformLocation(blueProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
		//glUniformMatrix4fv(glGetUniformLocation(blueProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
		//glUniformMatrix4fv(glGetUniformLocation(blueProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));
		//modelRender(&planeModel);


		// Swap and poll
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Clean up
	modelFree(&planeModel);
	modelFree(&cubeModel);
	textureFree(&container);
	textureFree(&moonman);
	shaderProgramFree(&texProgram);
	shaderProgramFree(&blueProgram);
	glfwTerminate();
	return 0;
}