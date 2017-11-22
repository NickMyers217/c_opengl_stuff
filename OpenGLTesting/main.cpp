#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string>
#include <vector>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "screen.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "texture.h"
#include "cubeData.h"
#include "planeData.h"
#include "baseModel.h"
#include "Mesh.h"


using namespace std;


// Global state
Camera freeLookCam;
Screen screen;


void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void processInput(GLFWwindow * window, double deltaTime);


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

	freeLookCam.Position = glm::vec3(0.0f, 0.0f, 10.0f);

	ShaderProgram lampProgram("basic_vertex.glsl", "lamp_frag.glsl");
	ShaderProgram texProgram("basic_vertex.glsl", "texture_phong_frag.glsl");

	Texture container, containerTwo, containerTwoSpecular, moonman;
	textureInit(&container, "container.jpg", DIFFUSE_AND_SPECULAR);
	textureInit(&moonman, "moonman.png", DIFFUSE_AND_SPECULAR, true, true);
	textureInit(&containerTwo, "container2.png", DIFFUSE, true, true);
	textureInit(&containerTwoSpecular, "container2_specular.png", SPECULAR, true, true);

	vector<Vertex> planeVertices;
	vector<GLuint> planeIndices;
	generatePlaneData(planeVertices, planeIndices, 20, 20);
	vector<Texture> planeTextures;
	planeTextures.push_back(moonman);
	Mesh planeMesh(planeVertices, planeIndices, planeTextures);

	vector<Vertex> cubeVertices;
	vector<GLuint> cubeIndices;
	generateCubeData(cubeVertices, cubeIndices);

	vector<Texture> cubeTextures;
	cubeTextures.push_back(containerTwo);
	cubeTextures.push_back(containerTwoSpecular);
	Mesh containerMesh(cubeVertices, cubeIndices, cubeTextures);

	#define NUM_LIGHTS 5
	Mesh lampMeshes[NUM_LIGHTS] = {
		Mesh(cubeVertices, cubeIndices, vector<Texture>()),
		Mesh(cubeVertices, cubeIndices, vector<Texture>()),
		Mesh(cubeVertices, cubeIndices, vector<Texture>()),
		Mesh(cubeVertices, cubeIndices, vector<Texture>()),
		Mesh(cubeVertices, cubeIndices, vector<Texture>()),
	};
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

	glm::mat4 projectionMat = glm::perspective(glm::radians(freeLookCam.GetFov()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

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

		lampProgram.Use();
		for (int i = 0; i < NUM_LIGHTS; i++)
		{
			modelMat = glm::mat4();
			if (i < 4)
			{
				modelMat = glm::translate(modelMat, lightPositions[i] + glm::vec3(0.0f, movementAmt, 0.0f));
			}
			modelMat = glm::scale(modelMat, glm::vec3(0.2f));
			lampProgram.SetUniform("model", modelMat);
			lampProgram.SetUniform("view", viewMat);
			lampProgram.SetUniform("projection", projectionMat);
			lampProgram.SetUniform("color", lightColors[i]);
			lampMeshes[i].Draw(lampProgram);
		}

		texProgram.Use();
		texProgram.SetUniform("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		texProgram.SetUniform("dirLight.ambient", glm::vec3(0.1f));
		texProgram.SetUniform("dirLight.diffuse", glm::vec3(0.1f));
		texProgram.SetUniform("dirLight.specular", glm::vec3(0.1f));
		for (int i = 0; i < NUM_LIGHTS; i++)
		{
			std::string lightUniform = "pointLights[" + std::to_string(i) + "]";
			if (i < 4)
			{
				texProgram.SetUniform((lightUniform + ".position").c_str(), lightPositions[i] + glm::vec3(0.0f, movementAmt, 0.0f));
			}
			else
			{
				texProgram.SetUniform((lightUniform + ".position").c_str(), lightPositions[i]);
			}
			texProgram.SetUniform((lightUniform + ".color").c_str(), lightColors[i]);
			texProgram.SetUniform((lightUniform + ".ambient").c_str(), glm::vec3(0.1f));
			texProgram.SetUniform((lightUniform + ".diffuse").c_str(), glm::vec3(0.4f));
			texProgram.SetUniform((lightUniform + ".specular").c_str(), glm::vec3(1.0f));
			texProgram.SetUniform((lightUniform + ".constant").c_str(), 1.0f);
			texProgram.SetUniform((lightUniform + ".linear").c_str(), 0.09f);
			texProgram.SetUniform((lightUniform + ".quadratic").c_str(), 0.032f);
		}
		texProgram.SetUniform("spotLight.position", freeLookCam.Position);
		texProgram.SetUniform("spotLight.direction", freeLookCam.Front);
		texProgram.SetUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		texProgram.SetUniform("spotLight.color", glm::vec3(1.0f));
		texProgram.SetUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
		texProgram.SetUniform("spotLight.ambient", glm::vec3(0.3f));
		texProgram.SetUniform("spotLight.diffuse", glm::vec3(0.5f));
		texProgram.SetUniform("spotLight.specular", glm::vec3(1.0f));
		texProgram.SetUniform("spotLight.constant", 1.0f);
		texProgram.SetUniform("spotLight.linear", 0.09f);
		texProgram.SetUniform("spotLight.quadratic", 0.032f);

		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, -2.0f, 0.0f));
		texProgram.SetUniform("material.shininess", 32.0f);
		texProgram.SetUniform("model", modelMat);
		texProgram.SetUniform("view", viewMat);
		texProgram.SetUniform("projection", projectionMat);
		planeMesh.Draw(texProgram);

		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, 2.0f, movementAmt * 3.0f));
		modelMat = glm::rotate(modelMat, rotAmt, glm::vec3(1.0f, 1.0f, 0.0f));
		texProgram.SetUniform("material.shininess", 32.0f);
		texProgram.SetUniform("model", modelMat);
		texProgram.SetUniform("view", viewMat);
		texProgram.SetUniform("projection", projectionMat);
		containerMesh.Draw(texProgram);

		// Swap buffers
		screenSwapAndPoll(&screen);
	}

	textureFree(&container);
	textureFree(&containerTwo);
	textureFree(&containerTwoSpecular);
	textureFree(&moonman);
	screenFree(&screen);
	return 0;
}
