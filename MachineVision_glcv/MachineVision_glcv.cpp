// MachineVision_glcv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"resource.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include"Shader.h"


using namespace cv;
using namespace ImGui;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);	//窗口大小改变回调函数
void processInput(GLFWwindow *window);		//按键回调


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
HWND hwnd;			//全局窗口句柄
HDC hdc;			//全局设备对象
HGLRC hglrc;		//全局渲染对象
HINSTANCE hinstance;	//去局窗口实例
Zopengl::Shader *shader;
cv::Mat matdata;
cv::VideoCapture *capture;

double win_width = SCR_WIDTH;
double win_height = SCR_HEIGHT;



int main()
{
	const char* glsl_version = "#version 130";
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}
	hwnd = glfwGetWin32Window(window);
	if (!hwnd)
		std::cout << "hwnd null";
	hinstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(1);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	capture = new cv::VideoCapture(0, cv::CAP_ANY);
	*capture >> matdata; 
	std::cout << matdata.size;

	Mat data;
	cvtColor(matdata, data, COLOR_BGR2GRAY);
	cvtColor(data, matdata, COLOR_GRAY2RGB);
	capture->release();
	delete capture;


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	std::cout << std::endl;
	std::cout << "The Opencv version used is: " << cv::getVersionString() << std::endl;

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	int width, height, nrChannels;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, matdata.cols, matdata.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, matdata.data);
	glGenerateMipmap(GL_TEXTURE_2D);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImVec2 pos{ 0,0 };
		//SetNextWindowPos(pos);
		//ImVec2 size{ 800,600 };
		//SetNextWindowSize(size);

		Begin("hello");
		ImGui::Image((void *)(intptr_t)texture,ImVec2(matdata.cols, matdata.rows));
		End();
		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);

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


		
