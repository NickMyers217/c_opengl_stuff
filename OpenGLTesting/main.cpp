#include <stdio.h>
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
#include "baseModel.h"


Camera freeLookCam;
Screen screen;
ShaderProgram lampProgram, texProgram;
Texture container, moonman;
BaseModel cubeModel, lampCubeModel;


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
	textureInit(&moonman, "moonman.png", true, true);

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
		shaderProgramUse(&texProgram);
		float movementAmt = sin((float)glfwGetTime()) * 2.0f + 1;
		float rotAmt = sin((float)glfwGetTime() * 0.5f + 1.0f);
		glm::mat4 modelMat;
		modelMat = glm::translate(modelMat, glm::vec3(movementAmt, movementAmt, -2.0f));
		modelMat = glm::rotate(modelMat, rotAmt, glm::vec3(1.0f, 1.0f, 0.0f));
		glm::mat4 viewMat = freeLookCam.GetViewMatrix();
		textureUse(&container, 0);
		textureUse(&moonman, 1);
		glUniform1i(glGetUniformLocation(texProgram.id, "texture0"), 0);
		glUniform1i(glGetUniformLocation(texProgram.id, "texture1"), 1);
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(texProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));
		glUniform3f(glGetUniformLocation(texProgram.id, "material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(texProgram.id, "material.diffuse"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(texProgram.id, "material.specular"), 0.5f, 0.5f, 0.5f);
		glUniform1f(glGetUniformLocation(texProgram.id, "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(texProgram.id, "light.position"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(texProgram.id, "light.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(texProgram.id, "light.diffuse"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(texProgram.id, "light.specular"), 1.0f, 1.0f, 1.0f);
		modelRender(&cubeModel);

		shaderProgramUse(&lampProgram);
		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, lightPos);
		modelMat = glm::scale(modelMat, glm::vec3(0.2f));
		glUniformMatrix4fv(glGetUniformLocation(lampProgram.id, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
		glUniformMatrix4fv(glGetUniformLocation(lampProgram.id, "view"), 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(glGetUniformLocation(lampProgram.id, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMat));
		modelRender(&lampCubeModel);

		// Swap buffers
		screenSwapAndPoll(&screen);
	}

	cleanUp();
	return 0;
}


void cleanUp()
{
	modelFree(&cubeModel);
	modelFree(&lampCubeModel);
	textureFree(&container);
	textureFree(&moonman);
	shaderProgramFree(&lampProgram);
	shaderProgramFree(&texProgram);
	screenFree(&screen);
}