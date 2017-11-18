#include <stdio.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "shaderProgram.h"
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
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello, World!", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to initialize a GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize a GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	printMajorAndMinorGlVersion();

	ShaderProgram programOne, programTwo;
	shaderProgramBuild(&programOne, "basic_vertex.glsl", "basic_frag.glsl");
	shaderProgramBuild(&programTwo, "basic_vertex.glsl", "basic_frag_two.glsl");

	GLfloat VERTICES_ONE[] = {
		-1.0f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.0f, -0.5f, 0.0f,
	};
	GLfloat VERTICES_TWO[] = {
		 0.0f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		 1.0f, -0.5f, 0.0f,
	};
	BaseModel modelOne, modelTwo;
	modelInit(&modelOne, VERTICES_ONE, sizeof(VERTICES_ONE) / sizeof(GLfloat));
	modelInit(&modelTwo, VERTICES_TWO, sizeof(VERTICES_TWO) / sizeof(GLfloat));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgramUse(&programOne);
		modelRender(&modelOne);

		shaderProgramUse(&programTwo);
		modelRender(&modelTwo);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	modelFree(&modelOne);
	modelFree(&modelTwo);
	shaderProgramFree(&programOne);
	shaderProgramFree(&programTwo);

	glfwTerminate();
	return 0;
}