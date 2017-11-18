#include <stdio.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shaderProgram.h"
#include "texture.h"
#include "baseModel.h"


void printMajorAndMinorGlVersion();
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void processInput(GLFWwindow * window, double deltaTime);


const int WIDTH = 1600;
const int HEIGHT = 900;
const int FPS = 120;
const float FOV = 60.0f;

// Camera creation
Camera freeLookCam;


void printMajorAndMinorGlVersion()
{
	gladGLversionStruct version;
	glGetIntegerv(GL_MAJOR_VERSION, &version.major);
	glGetIntegerv(GL_MINOR_VERSION, &version.minor);
	printf("Initializing OpenGL V%i.%i\n", version.major, version.minor);
}

float lastX = (float)WIDTH / 2.0f;
float lastY = (float)HEIGHT / 2.0f;
bool firstMouse = true;
void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOff = xpos - lastX;
	float yOff = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	freeLookCam.ProcessMouseMovement(xOff, yOff);
}

void processInput(GLFWwindow * window, double deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		freeLookCam.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		freeLookCam.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		freeLookCam.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		freeLookCam.ProcessKeyboard(RIGHT, deltaTime);
}


int main()
{
	// Window initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "MOONMAN", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to initialize a GLFW window\n");
		glfwTerminate();
		return -1;
	}
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(window, mode->width/2 - WIDTH/2, mode->height/2 - HEIGHT/2);
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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


	// Texture creation
	Texture container;
	textureInit(&container, "container.jpg", false, false);
	Texture moonman;
	textureInit(&moonman, "moonman.png", true, true);
	shaderProgramUse(&texProgram);
	glUniform1i(glGetUniformLocation(texProgram.id, "texture0"), 0);
	glUniform1i(glGetUniformLocation(texProgram.id, "texture1"), 1);


	// Model creation
	GLfloat CUBE_VERTICES[] = {
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
	modelInit(&cubeModel, CUBE_VERTICES, sizeof(CUBE_VERTICES) / sizeof(GLfloat));
	BaseModel anotherCubeModel;
	modelInit(&anotherCubeModel, CUBE_VERTICES, sizeof(CUBE_VERTICES) / sizeof(GLfloat));


	glm::mat4 projectionMat = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);


	// Game loop
	double deltaTime = 0.0f;
	double lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// Delta calculations
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		printf("\r delta time: %f", deltaTime * 1000.0);

		// Event handling
		processInput(window, deltaTime);


		// Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Uniforms & Rendering
		glm::mat4 modelMat;
		modelMat = glm::rotate(modelMat, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		glm::mat4 anotherModelMat;
		anotherModelMat = glm::translate(anotherModelMat, glm::vec3(-2.0f, 0.25f, 0.0f));

		float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 viewMat;
		viewMat = freeLookCam.GetViewMatrix();

		shaderProgramUse(&texProgram);
		textureUse(&container, 0);
		textureUse(&moonman, 1);

		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));
		modelRender(&cubeModel);

		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(anotherModelMat));
		modelRender(&anotherCubeModel);


		// Swap and poll
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Clean up
	modelFree(&cubeModel);
	modelFree(&anotherCubeModel);
	textureFree(&container);
	textureFree(&moonman);
	shaderProgramFree(&texProgram);
	glfwTerminate();
	return 0;
}