#include "screen.h"
#include "Camera.h"
#include "ShaderProgram.h"
#include "texture.h"
#include "cubeData.h"
#include "planeData.h"
#include "Model.h"
#include "Mesh.h"
#include "Light.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <string>
#include <vector>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


using namespace std;


// Global state
Camera freeLookCam;
Screen screen;


void mouse_callback(GLFWwindow * window, double xpos, double ypos);
void processInput(GLFWwindow * window, double deltaTime);


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

	Texture joey, containerTwo, containerTwoSpecular, moonman;
	textureInit(&joey, "joey.jpg", TextureType::DIFFUSE_AND_SPECULAR, true);
	textureInit(&moonman, "moonman.png", TextureType::DIFFUSE_AND_SPECULAR, true, true);
	textureInit(&containerTwo, "container2.png", TextureType::DIFFUSE, true, true);
	textureInit(&containerTwoSpecular, "container2_specular.png", TextureType::SPECULAR, true, true);

	vector<Vertex> planeVertices;
	vector<GLuint> planeIndices;
	generatePlaneData(planeVertices, planeIndices, 20, 20);
	vector<Texture> planeTextures = {
		moonman
	};
	Mesh planeMesh(planeVertices, planeIndices, planeTextures);

	vector<Vertex> cubeVertices;
	vector<GLuint> cubeIndices;
	generateCubeData(cubeVertices, cubeIndices);
	vector<Texture> cubeTextures = {
		containerTwo,
		containerTwoSpecular
	};
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
	vector<Light> pointLights(NUM_LIGHTS);
	for (unsigned int i = 0; i < NUM_LIGHTS; i++)
		pointLights[i] = Light::PointLight(lightPositions[i], lightColors[i]);
	Light dirLight = Light::DirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(1.0f));
	Light spotLight = Light::SpotLight(glm::vec3(0.0f), glm::vec3(0.0f));

	Model nanosuit("C:/Users/Boromir/Downloads/nanosuit/nanosuit.obj");
	Model suzanne("C:/Program Files/Assimp/test/models/BLEND/Suzanne_248.blend");

	glm::mat4 projectionMat = glm::perspective(glm::radians(freeLookCam.GetFov()), (float)screen.width / (float)screen.height, 0.1f, 100.0f);

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
		for (unsigned int i = 0; i < pointLights.size() - 1; i++)
		{
			pointLights[i].Position = lightPositions[i] + glm::vec3(0.0f, movementAmt, 0.0f);
		}
		spotLight.Position = freeLookCam.Position;
		spotLight.Direction = freeLookCam.Front;

		texProgram.SetUniform("dirLight", dirLight);
		texProgram.SetUniform("pointLights", pointLights);
		texProgram.SetUniform("spotLight", spotLight);

		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, -2.0f, -5.0f));
		modelMat = glm::scale(modelMat, glm::vec3(0.25f));
		texProgram.SetUniform("material.shininess", 32.0f);
		texProgram.SetUniform("model", modelMat);
		texProgram.SetUniform("view", viewMat);
		texProgram.SetUniform("projection", projectionMat);
		nanosuit.Draw(texProgram);

		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, -2.0f, 0.0f));
		texProgram.SetUniform("material.shininess", 32.0f);
		texProgram.SetUniform("model", modelMat);
		texProgram.SetUniform("view", viewMat);
		texProgram.SetUniform("projection", projectionMat);
		planeMesh.Draw(texProgram);

		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, glm::vec3(movementAmt * 2.0f, 2.0f, 0.0f));
		modelMat = glm::rotate(modelMat, rotAmt, glm::vec3(1.0f, 1.0f, 0.0f));
		texProgram.SetUniform("material.shininess", 32.0f);
		texProgram.SetUniform("model", modelMat);
		texProgram.SetUniform("view", viewMat);
		texProgram.SetUniform("projection", projectionMat);
		containerMesh.Draw(texProgram);

		// TODO: consider moving Material and Transformation data be part of the Model/Mesh
		modelMat = glm::mat4();
		modelMat = glm::translate(modelMat, glm::vec3(0.0f, 2.0f, 10.0f + movementAmt * 1.5f));
		modelMat = glm::rotate(modelMat, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		texProgram.SetUniform("material.shininess", 32.0f);
		texProgram.SetUniform("model", modelMat);
		texProgram.SetUniform("view", viewMat);
		texProgram.SetUniform("projection", projectionMat);
		suzanne.Draw(texProgram);

		// Swap buffers
		screenSwapAndPoll(&screen);
	}
	MyUtils::glCheckError();

	textureFree(&joey);
	textureFree(&containerTwo);
	textureFree(&containerTwoSpecular);
	textureFree(&moonman);
	screenFree(&screen);
	return 0;
}


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

