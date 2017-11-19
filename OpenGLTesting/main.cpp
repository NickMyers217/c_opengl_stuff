#include <stdio.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shaderProgram.h"
#include "texture.h"
#include "cubeData.h"
#include "baseModel.h"


// TODO: add a window abstraction at some point


void printMajorAndMinorGlVersion();
void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void processInput(GLFWwindow * window, double deltaTime);


const int WIDTH = 1600;
const int HEIGHT = 900;
const int FPS = 120;
const float FOV = 50.0f;

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
	glEnable(GL_CULL_FACE);
	printMajorAndMinorGlVersion();


	// Shader program creation
	ShaderProgram lampProgram;
	shaderProgramBuild(&lampProgram, "basic_vertex.glsl", "lamp_frag.glsl");
	ShaderProgram texProgram;
	shaderProgramBuild(&texProgram, "basic_vertex.glsl", "texture_phong_frag.glsl");


	// Texture creation
	Texture container;
	textureInit(&container, "container.jpg", false, false);
	Texture moonman;
	textureInit(&moonman, "moonman.png", true, true);
	shaderProgramUse(&texProgram);
	glUniform1i(glGetUniformLocation(texProgram.id, "texture0"), 0);
	glUniform1i(glGetUniformLocation(texProgram.id, "texture1"), 1);


	// Model creation
	BaseModel cubeModel;
	modelInit(&cubeModel, CUBE_VERTICES, sizeof(CUBE_VERTICES) / sizeof(GLfloat));
	BaseModel lampCubeModel;
	modelInit(&lampCubeModel, CUBE_VERTICES, sizeof(CUBE_VERTICES) / sizeof(GLfloat));


	glm::mat4 projectionMat = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);


	// Game loop
	double deltaTime = 0.0f;
	double lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// Delta calculations
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		printf("\rFPS: %f", 1.0 / deltaTime);

		// Event handling
		processInput(window, deltaTime);


		// Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Uniforms & Rendering
		shaderProgramUse(&texProgram);
		textureUse(&container, 0);
		textureUse(&moonman, 1);
		float movementAmt = sin((float)glfwGetTime()) * 2.0f + 1;
		float rotAmt = sin((float)glfwGetTime() * 0.5f + 1.0f);
		glm::mat4 modelMat;
		modelMat = glm::translate(modelMat, glm::vec3(movementAmt, movementAmt, 4.0f));
		modelMat = glm::rotate(modelMat, rotAmt, glm::vec3(1.0f, 1.0f, 0.0f));
		glm::mat4 viewMat = freeLookCam.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));
		glUniform3f(glGetUniformLocation(texProgram.id, "lightColor"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(texProgram.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		modelRender(&cubeModel);

		shaderProgramUse(&lampProgram);
		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, lightPos);
		modelMat = glm::scale(modelMat, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lampProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(glGetUniformLocation(lampProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(lampProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));
		modelRender(&lampCubeModel);

		// Swap and poll
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Clean up
	modelFree(&cubeModel);
	modelFree(&lampCubeModel);
	textureFree(&container);
	textureFree(&moonman);
	shaderProgramFree(&lampProgram);
	shaderProgramFree(&texProgram);
	glfwTerminate();
	return 0;
}