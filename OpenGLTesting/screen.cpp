#include "screen.h"

static void printMajorAndMinorGlVersion()
{
	gladGLversionStruct version;
	glGetIntegerv(GL_MAJOR_VERSION, &version.major);
	glGetIntegerv(GL_MINOR_VERSION, &version.minor);
	printf("Initializing OpenGL V%i.%i\n", version.major, version.minor);
}

int screenInit(Screen * screen, GLFWcursorposfun mouseCallback)
{
	// Glfw initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, 0);
	glfwWindowHint(GLFW_MAXIMIZED, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWmonitor * monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode * mode = glfwGetVideoMode(monitor);
	screen->width = mode->width;
	screen->height = mode->height;
	screen->mouseCaptured = false;
	screen->lastX = (float)screen->width / 2.0f;
	screen->lastY = (float)screen->height / 2.0f;
	screen->window = glfwCreateWindow(mode->width, mode->height, "MOONMAN", monitor, NULL);
	if (screen->window == NULL)
	{
		printf("Failed to initialize a GLFW window\n");
		glfwTerminate();
		return -1;
	}
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
	glViewport(0, 0, mode->width, mode->height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_CULL_FACE);
	printMajorAndMinorGlVersion();
	return 0;
}

int screenShouldClose(Screen * screen)
{
	return glfwWindowShouldClose(screen->window);
}

void screenClear(Screen * screen)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

