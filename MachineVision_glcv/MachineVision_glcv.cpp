// MachineVision_glcv.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"resource.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include"Shader.h"


using namespace cv;
using namespace ImGui;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);	//���ڴ�С�ı�ص�����
void processInput(GLFWwindow *window);		//�����ص�


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
HWND hwnd;			//ȫ�ִ��ھ��
HDC hdc;			//ȫ���豸����
HGLRC hglrc;		//ȫ����Ⱦ����
HINSTANCE hinstance;	//ȥ�ִ���ʵ��
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
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���ز���������
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

// �����������룺��ѯGLFW��֡�Ƿ���/�ͷ���ؼ��������ǵ�
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw��ÿ�����ڴ�С�ı�ʱ��ͨ������ϵͳ���û�������С��������ص������ͻ�ִ��
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//ȷ���Ӵ����µ��Ӵ��ߴ���ƥ�䣻ע���Ⱥ�
	//�߶Ƚ����Դ�������Ĥ��ʾ����ָ���ĸ߶ȡ�
	glViewport(0, 0, width, height);
}


		
