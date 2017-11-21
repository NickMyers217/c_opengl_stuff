#pragma once

#include <stdio.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>


struct Screen {
	GLFWwindow * window;
	float lastX, lastY;
	bool mouseCaptured;
};

const int WIDTH = 1600;
const int HEIGHT = 900;
const int FPS = 120;


void printMajorAndMinorGlVersion();
int screenInit(Screen * screen, GLFWcursorposfun mouseCallback);
int screenShouldClose(Screen * screen);
void screenClear(Screen * screen);
void screenSwapAndPoll(Screen * screen);
void screenFree(Screen * screen);


void printMajorAndMinorGlVersion()
{
	gladGLversionStruct version;
	glGetIntegerv(GL_MAJOR_VERSION, &version.major);
	glGetIntegerv(GL_MINOR_VERSION, &version.minor);
	printf("Initializing OpenGL V%i.%i\n", version.major, version.minor);
}

int screenInit(Screen * screen, GLFWcursorposfun mouseCallback)
{
	screen->mouseCaptured = false;
	screen->lastX = (float)WIDTH / 2.0f;
	screen->lastY = (float)HEIGHT / 2.0f;

	// Glfw initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	screen->window = glfwCreateWindow(WIDTH, HEIGHT, "MOONMAN", NULL, NULL);
	if (screen->window == NULL)
	{
		printf("Failed to initialize a GLFW window\n");
		glfwTerminate();
		return -1;
	}
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(screen->window, mode->width/2 - WIDTH/2, mode->height/2 - HEIGHT/2);
	glfwMakeContextCurrent(screen->window);
	glfwSetCursorPosCallback(screen->window, mouseCallback);
	glfwSetInputMode(screen->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
}

int screenShouldClose(Screen * screen)
{
	return glfwWindowShouldClose(screen->window);
}

void screenClear(Screen * screen)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void screenSwapAndPoll(Screen * screen)
{
	glfwSwapBuffers(screen->window);
	glfwPollEvents();
}

void screenFree(Screen * screen)
{
	glfwTerminate();
}
