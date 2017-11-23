#pragma once

#include <stdio.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>


struct Screen {
	GLFWwindow * window;
	float lastX, lastY;
	bool mouseCaptured;
	int width, height;
};

int screenInit(Screen * screen, GLFWcursorposfun mouseCallback);
int screenShouldClose(Screen * screen);
void screenClear(Screen * screen);
void screenSwapAndPoll(Screen * screen);
void screenFree(Screen * screen);
