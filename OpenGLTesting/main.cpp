#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "screen.h"
#include "camera.h"
#include "shaderProgram.h"
#include "texture.h"
#include "cubeData.h"
#include "planeData.h"
#include "baseModel.h"


// TODO: can i get rid of this at some point? is it even worth it?
// Global state
Camera freeLookCam;
Screen screen;
ShaderProgram lampProgram, texProgram;
Texture container, containerTwo, containerTwoSpecular, moonman;
BaseModel planeModel, cubeModel, lampCubeModel;


void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void processInput(GLFWwindow * window, double deltaTime);
void cleanUp();


void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (!screen.mouseCaptured)
	{
		screen.lastX = xpos;
		screen.lastY = ypos;
		screen.mouseCaptured = true;
	}

	float xOff = xpos - screen.lastX;
	float yOff = screen.lastY - ypos;
	screen.lastX = xpos;
	screen.lastY = ypos;

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
	// Screen initialization
	if (screenInit(&screen, mouse_callback) < 0)
	{
		printf("Error initialization the screen!");
		return -1;
	}

	shaderProgramBuild(&lampProgram, "basic_vertex.glsl", "lamp_frag.glsl");
	shaderProgramBuild(&texProgram, "basic_vertex.glsl", "texture_phong_frag.glsl");

	textureInit(&container, "container.jpg", false, false);
	textureInit(&containerTwo, "container2.png", true, true);
	textureInit(&containerTwoSpecular, "container2_specular.png", true, true);
	textureInit(&moonman, "moonman.png", false, true);

	float * planeVertices;
	planeVertices = (GLfloat *)malloc(10 * 10 * (6 * 8) * sizeof(float));
	generatePlaneVertices(planeVertices, 10, 10);
	modelInit(&planeModel, planeVertices, 10 * 10 * (6 * 8));
	free(planeVertices);
	modelInit(&cubeModel, CUBE_VERTICES, sizeof(CUBE_VERTICES) / sizeof(GLfloat));
	modelInit(&lampCubeModel, CUBE_VERTICES, sizeof(CUBE_VERTICES) / sizeof(GLfloat));

	glm::mat4 projectionMat = glm::perspective(glm::radians(freeLookCam.GetFov()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	double deltaTime = 0.0f;
	double lastFrame = 0.0f;
	while (!screenShouldClose(&screen))
	{
		// Delta calculations
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		printf("\rFPS: %f", 1.0 / deltaTime);

		// Event handling
		processInput(screen.window, deltaTime);

		// Clear
		screenClear(&screen);

		// Uniforms & Rendering
		float movementAmt = sin((float)glfwGetTime()) * 2.0f + 1;
		float rotAmt = sin((float)glfwGetTime() * 0.5f + 1.0f);
		glm::mat4 modelMat;
		glm::mat4 viewMat = freeLookCam.GetViewMatrix();
		modelMat = glm::translate(modelMat, glm::vec3(movementAmt, movementAmt, -2.0f));
		modelMat = glm::rotate(modelMat, rotAmt, glm::vec3(1.0f, 1.0f, 0.0f));

		shaderProgramUse(&texProgram);
		textureUse(&containerTwo, 0);
		textureUse(&containerTwoSpecular, 1);
		shaderProgramSet(&texProgram, "material.diffuse", 0);
		shaderProgramSet(&texProgram, "material.specular", 1);
		shaderProgramSet(&texProgram, "material.shininess", 32.0f);
		shaderProgramSet(&texProgram, "light.position", lightPos);
		shaderProgramSet(&texProgram, "light.ambient", glm::vec3(0.2f));
		shaderProgramSet(&texProgram, "light.diffuse", glm::vec3(0.5f));
		shaderProgramSet(&texProgram, "light.specular", glm::vec3(1.0f));
		shaderProgramSet(&texProgram, "model", modelMat);
		shaderProgramSet(&texProgram, "view", viewMat);
		shaderProgramSet(&texProgram, "projection", projectionMat);
		modelRender(&cubeModel);

		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, -4.0f, 0.0f));
		textureUse(&moonman, 0);
		textureUse(&moonman, 1);
		shaderProgramSet(&texProgram, "model", modelMat);
		shaderProgramSet(&texProgram, "view", viewMat);
		shaderProgramSet(&texProgram, "projection", projectionMat);
		modelRender(&planeModel);

		shaderProgramUse(&lampProgram);
		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, lightPos);
		modelMat = glm::scale(modelMat, glm::vec3(0.2f));
		shaderProgramSet(&lampProgram, "model", modelMat);
		shaderProgramSet(&lampProgram, "view", viewMat);
		shaderProgramSet(&lampProgram, "projection", projectionMat);
		modelRender(&lampCubeModel);

		// Swap buffers
		screenSwapAndPoll(&screen);
	}

	cleanUp();
	return 0;
}


void cleanUp()
{
	modelFree(&planeModel);
	modelFree(&cubeModel);
	modelFree(&lampCubeModel);
	textureFree(&container);
	textureFree(&containerTwo);
	textureFree(&containerTwoSpecular);
	textureFree(&moonman);
	shaderProgramFree(&lampProgram);
	shaderProgramFree(&texProgram);
	screenFree(&screen);
}