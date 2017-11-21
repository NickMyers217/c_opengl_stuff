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
BaseModel planeModel, cubeModel;
#define NUM_LIGHTS 5
BaseModel lampModels[NUM_LIGHTS];


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
	textureInit(&moonman, "moonman.png", true, true);

	float * planeVertices;
	planeVertices = (GLfloat *)malloc(20 * 20 * (6 * 8) * sizeof(float));
	generatePlaneVertices(planeVertices, 20, 20);
	modelInit(&planeModel, planeVertices, 20 * 20 * (6 * 8));
	free(planeVertices);
	modelInit(&cubeModel, CUBE_VERTICES, sizeof(CUBE_VERTICES) / sizeof(GLfloat));
	for (int i = 0; i < NUM_LIGHTS; i++)
		modelInit(&lampModels[i], CUBE_VERTICES, sizeof(CUBE_VERTICES) / sizeof(GLfloat));

	glm::mat4 projectionMat = glm::perspective(glm::radians(freeLookCam.GetFov()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	glm::vec3 lightPositions[NUM_LIGHTS] = {
		glm::vec3(-8.0f, 1.0f,  8.0f),
		glm::vec3(-8.0f, 1.0f, -8.0f),
		glm::vec3( 8.0f, 1.0f, -8.0f),
		glm::vec3( 8.0f, 1.0f,  8.0f),
		glm::vec3( 0.0f, 0.0f,  0.0f),
	};
	glm::vec3 lightColors[NUM_LIGHTS] = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
	};

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
		float movementAmt = sin((float)glfwGetTime()) * 2.0f + 0.5f;
		float rotAmt = sin((float)glfwGetTime() * 0.5f + 1.0f);
		glm::mat4 modelMat;
		glm::mat4 viewMat = freeLookCam.GetViewMatrix();
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, movementAmt, 0.0f));
		modelMat = glm::rotate(modelMat, rotAmt, glm::vec3(1.0f, 1.0f, 0.0f));

		shaderProgramUse(&texProgram);
		textureUse(&moonman, 0);
		textureUse(&moonman, 1);
		shaderProgramSet(&texProgram, "model", modelMat);
		shaderProgramSet(&texProgram, "view", viewMat);
		shaderProgramSet(&texProgram, "projection", projectionMat);
		shaderProgramSet(&texProgram, "material.diffuse", 0);
		shaderProgramSet(&texProgram, "material.specular", 1);
		shaderProgramSet(&texProgram, "material.shininess", 32.0f);

		shaderProgramSet(&texProgram, "dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		shaderProgramSet(&texProgram, "dirLight.ambient", glm::vec3(0.1f));
		shaderProgramSet(&texProgram, "dirLight.diffuse", glm::vec3(0.1f));
		shaderProgramSet(&texProgram, "dirLight.specular", glm::vec3(0.1f));

		shaderProgramSet(&texProgram, "pointLights[0].position", lightPositions[0] + glm::vec3(0.0f, movementAmt, 0.0f));
		shaderProgramSet(&texProgram, "pointLights[0].color", lightColors[0]);
		shaderProgramSet(&texProgram, "pointLights[0].ambient", glm::vec3(0.1f));
		shaderProgramSet(&texProgram, "pointLights[0].diffuse", glm::vec3(0.4f));
		shaderProgramSet(&texProgram, "pointLights[0].specular", glm::vec3(1.0f));
		shaderProgramSet(&texProgram, "pointLights[0].constant", 1.0f);
		shaderProgramSet(&texProgram, "pointLights[0].linear", 0.09f);
		shaderProgramSet(&texProgram, "pointLights[0].quadratic", 0.032f);
		shaderProgramSet(&texProgram, "pointLights[1].position", lightPositions[1] + glm::vec3(0.0f, movementAmt, 0.0f));
		shaderProgramSet(&texProgram, "pointLights[1].color", glm::vec3(0.0f, 1.0f, 0.0f));
		shaderProgramSet(&texProgram, "pointLights[1].color", lightColors[1]);
		shaderProgramSet(&texProgram, "pointLights[1].ambient", glm::vec3(0.1f));
		shaderProgramSet(&texProgram, "pointLights[1].diffuse", glm::vec3(0.4f));
		shaderProgramSet(&texProgram, "pointLights[1].specular", glm::vec3(1.0f));
		shaderProgramSet(&texProgram, "pointLights[1].constant", 1.0f);
		shaderProgramSet(&texProgram, "pointLights[1].linear", 0.09f);
		shaderProgramSet(&texProgram, "pointLights[1].quadratic", 0.032f);
		shaderProgramSet(&texProgram, "pointLights[2].position", lightPositions[2] + glm::vec3(0.0f, movementAmt, 0.0f));
		shaderProgramSet(&texProgram, "pointLights[2].color", glm::vec3(0.0f, 0.0f, 1.0f));
		shaderProgramSet(&texProgram, "pointLights[2].color", lightColors[2]);
		shaderProgramSet(&texProgram, "pointLights[2].ambient", glm::vec3(0.1f));
		shaderProgramSet(&texProgram, "pointLights[2].diffuse", glm::vec3(0.4f));
		shaderProgramSet(&texProgram, "pointLights[2].specular", glm::vec3(1.0f));
		shaderProgramSet(&texProgram, "pointLights[2].constant", 1.0f);
		shaderProgramSet(&texProgram, "pointLights[2].linear", 0.09f);
		shaderProgramSet(&texProgram, "pointLights[2].quadratic", 0.032f);
		shaderProgramSet(&texProgram, "pointLights[3].position", lightPositions[3] + glm::vec3(0.0f, movementAmt, 0.0f));
		shaderProgramSet(&texProgram, "pointLights[3].color", lightColors[3]);
		shaderProgramSet(&texProgram, "pointLights[3].ambient", glm::vec3(0.1f));
		shaderProgramSet(&texProgram, "pointLights[3].diffuse", glm::vec3(0.4f));
		shaderProgramSet(&texProgram, "pointLights[3].specular", glm::vec3(1.0f));
		shaderProgramSet(&texProgram, "pointLights[3].constant", 1.0f);
		shaderProgramSet(&texProgram, "pointLights[3].linear", 0.009f);
		shaderProgramSet(&texProgram, "pointLights[3].quadratic", 0.0032f);
		shaderProgramSet(&texProgram, "pointLights[4].position", lightPositions[4]);
		shaderProgramSet(&texProgram, "pointLights[4].color", lightColors[4]);
		shaderProgramSet(&texProgram, "pointLights[4].ambient", glm::vec3(0.1f));
		shaderProgramSet(&texProgram, "pointLights[4].diffuse", glm::vec3(0.4f));
		shaderProgramSet(&texProgram, "pointLights[4].specular", glm::vec3(1.0f));
		shaderProgramSet(&texProgram, "pointLights[4].constant", 1.0f);
		shaderProgramSet(&texProgram, "pointLights[4].linear", 0.009f);
		shaderProgramSet(&texProgram, "pointLights[4].quadratic", 0.0032f);

		shaderProgramSet(&texProgram, "spotLight.position", freeLookCam.Position);
		shaderProgramSet(&texProgram, "spotLight.direction", freeLookCam.Front);
		shaderProgramSet(&texProgram, "spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		shaderProgramSet(&texProgram, "spotLight.color", glm::vec3(1.0f));
		shaderProgramSet(&texProgram, "spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
		shaderProgramSet(&texProgram, "spotLight.ambient", glm::vec3(0.3f));
		shaderProgramSet(&texProgram, "spotLight.diffuse", glm::vec3(0.5f));
		shaderProgramSet(&texProgram, "spotLight.specular", glm::vec3(1.0f));
		shaderProgramSet(&texProgram, "spotLight.constant", 1.0f);
		shaderProgramSet(&texProgram, "spotLight.linear", 0.09f);
		shaderProgramSet(&texProgram, "spotLight.quadratic", 0.032f);
		modelRender(&cubeModel);

		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, -2.0f, 0.0f));
		textureUse(&containerTwo, 0);
		textureUse(&containerTwoSpecular, 1);
		shaderProgramSet(&texProgram, "model", modelMat);
		shaderProgramSet(&texProgram, "view", viewMat);
		shaderProgramSet(&texProgram, "projection", projectionMat);
		modelRender(&planeModel);

		for (int i = 0; i < 5; i++)
		{
			shaderProgramUse(&lampProgram);
			modelMat = glm::mat4();
			if (i < 4)
			{
				modelMat = glm::translate(modelMat, lightPositions[i] + glm::vec3(0.0f, movementAmt, 0.0f));
			}
			modelMat = glm::scale(modelMat, glm::vec3(0.2f));
			shaderProgramSet(&lampProgram, "model", modelMat);
			shaderProgramSet(&lampProgram, "view", viewMat);
			shaderProgramSet(&lampProgram, "projection", projectionMat);
			shaderProgramSet(&lampProgram, "color", lightColors[i]);
			modelRender(&lampModels[i]);
		}

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
	for(int i = 0; i < NUM_LIGHTS; i++)
		modelFree(&lampModels[i]);
	textureFree(&container);
	textureFree(&containerTwo);
	textureFree(&containerTwoSpecular);
	textureFree(&moonman);
	shaderProgramFree(&lampProgram);
	shaderProgramFree(&texProgram);
	screenFree(&screen);
}