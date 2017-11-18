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
	printMajorAndMinorGlVersion();


	// Shader program creation
	ShaderProgram programOne;
	shaderProgramBuild(&programOne, "basic_vertex.glsl", "basic_frag.glsl");
	ShaderProgram programTwo;
	shaderProgramBuild(&programTwo, "basic_vertex.glsl", "basic_frag_two.glsl");


	// Texture creation
	Texture container;
	textureInit(&container, "container.jpg", false, false);
	Texture moonman;
	textureInit(&moonman, "moonman.png", true, true);
	shaderProgramUse(&programOne);
	glUniform1i(glGetUniformLocation(programOne.id, "texture0"), 0);
	glUniform1i(glGetUniformLocation(programOne.id, "texture1"), 1);


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
	BaseModel modelOne;
	modelInit(&modelOne, VERTICES_ONE, sizeof(VERTICES_ONE) / sizeof(GLfloat));

	GLfloat VERTICES_TWO[] = {
		 0.0f, -0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,  0.5f, 1.0f,
		 1.0f, -0.5f, 0.0f,  1.0f, 0.0f,
	};
	BaseModel modelTwo;
	modelInit(&modelTwo, VERTICES_TWO, sizeof(VERTICES_TWO) / sizeof(GLfloat));


	// Transformation and Uniform Stuff
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3());
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.5f));
	shaderProgramUse(&programOne);
	glUniformMatrix4fv(glGetUniformLocation(programOne.id, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
	shaderProgramUse(&programTwo);
	glUniformMatrix4fv(glGetUniformLocation(programTwo.id, "transform"), 1, GL_FALSE, glm::value_ptr(trans));


	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Event handling
		processInput(window);


		// Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//Rendering
		shaderProgramUse(&programOne);
		textureUse(&container, 0);
		textureUse(&moonman, 1);
		modelRender(&modelOne);

		shaderProgramUse(&programTwo);
		modelRender(&modelTwo);


		// Swap and poll
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Clean up
	modelFree(&modelOne);
	modelFree(&modelTwo);
	textureFree(&container);
	textureFree(&moonman);
	shaderProgramFree(&programOne);
	shaderProgramFree(&programTwo);
	glfwTerminate();
	return 0;
}