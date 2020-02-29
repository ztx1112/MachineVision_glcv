// MachineVision_glcv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"resource.h"
#include<Winuser.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);	//窗口大小改变回调函数
void processInput(GLFWwindow *window);		//按键回调
void commandFun(WPARAM wParam, LPARAM lParam);	//按钮回调

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
HWND hwnd;			//全局窗口句柄
HDC hdc;			//全局设备对象
HGLRC hglrc;		//全局渲染对象
HINSTANCE hinstance;	//去局窗口实例


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//hwnd = glfwGetWin32Window(window);
	hwnd = GetActiveWindow();
	if (!hwnd)
		std::cout << "hwnd null";
	hinstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
	HMENU hMenu = LoadMenu(hinstance, MAKEINTRESOURCE(IDR_MENU1));
	SetMenu(hwnd, hMenu);


	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCammandCallback(window, commandFun);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	std::cout << "The Opencv version used is: " << cv::getVersionString();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// 处理所有输入：查询GLFW此帧是否按下/释放相关键并根据是的
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw：每当窗口大小改变时（通过操作系统或用户调整大小），这个回调函数就会执行
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//确保视窗与新的视窗尺寸相匹配；注意宽度和
	//高度将明显大于视网膜显示器上指定的高度。
	glViewport(0, 0, width, height);
}

// 按钮事件回调函数
// ---------------------------------------------------------------------------------------------
void commandFun(WPARAM wParam, LPARAM lParam)
{
	WORD id = LOWORD(wParam);
	switch (id)
	{
	case IDM_FILE_NEW:
		break;
	default:
		MessageBox(hwnd, L"No Command Handle", 0, MB_ICONEXCLAMATION);
		break;
	}
	return;
}
		
