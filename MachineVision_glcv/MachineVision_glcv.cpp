// MachineVision_glcv.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "resource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "cvclass/DetectionFrame.h"
#include "Shader.h"

using namespace cv;
using namespace ImGui;

void framebuffer_size_callback(GLFWwindow *window, int width, int height); //窗口大小改变回调函数
void keyboard_callback(GLFWwindow *, int, int, int, int);
void mousebutton_callback(GLFWwindow *, int, int, int);
void glfw_error_callback(int, const char *);

void showmenu();
void showframe();
DetectionFrame showimage(DetectionFrame &);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
HWND hwnd;			 //全局窗口句柄
HINSTANCE hinstance; //去局窗口实例
Zopengl::Shader *shader;
double win_width = SCR_WIDTH;
double win_height = SCR_HEIGHT;
float titleheigh = 45;
float border = 4;
int state = 0;
ImVec2 beginpos;
ImVec2 endpos;
ImVec2 mousepos;
ImGuiIO io;
ImFont *font2;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
vector<DetectionFrame> Deteframes;
int teststate;

vector<unsigned int> textures;

wstring test = L"示例";

int main()
{
	const char *glsl_version = "#version 330";
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Machine Vision", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		std::cin.get();
		glfwTerminate();
		return -1;
	}
#ifdef _WIN32
	hwnd = glfwGetWin32Window(window);
	if (!hwnd)
		std::cout << "hwnd null";
	hinstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
#endif
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyboard_callback);
	//glfwSetMouseButtonCallback(window, mousebutton_callback);
	glfwSwapInterval(1);
	glfwSetErrorCallback(glfw_error_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	ImGui::CreateContext();
	io = GetIO();
	GetIO().Fonts->AddFontFromFileTTF("msyh.ttf", 14, NULL, GetIO().Fonts->GetGlyphRangesChineseFull());

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	std::cout << std::endl;
	std::cout << "The Opencv version used is: " << cv::getVersionString() << std::endl;

	Deteframes.push_back(DetectionFrame());

	Deteframes[0].camera.open(0);
	Deteframes[0].camerarunstate = "multi";
	if (!Deteframes[0].camera.isOpened())
	{
		cout << "failed open camera" << endl;
	}

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		showmenu();

		ImVec2 pos{0, 20};
		SetNextWindowPos(pos);

		if (Deteframes.size() > 0)
		{
			for (vector<DetectionFrame>::iterator iter = Deteframes.begin(); iter != Deteframes.end(); ++iter)
			{
				auto a = *iter;
				*iter = showimage(a);
			}
		}
		showframe();

		// Rendering
		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		if (Deteframes.size() > 0)
		{
			for (vector<DetectionFrame>::iterator iter = Deteframes.begin(); iter != Deteframes.end(); ++iter)
			{
				glDeleteTextures(1, &iter->texture);
			}
		}
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// 处理所有输入：查询GLFW此帧是否按下/释放相关键并根据是的
// ---------------------------------------------------------------------------------------------------------
void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int modsBit)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
	{
		glfwSetWindowShouldClose(window, true);
	}
	default:
		break;
	}
}

void mousebutton_callback(GLFWwindow *window, int button, int action, int modsBit)
{

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		switch (action)
		{
		case GLFW_PRESS:
		{
		}
		case GLFW_RELEASE:
		{
			state = 1;
		}
		default:
			break;
		}
		return;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
	{
		switch (action)
		{
		case GLFW_PRESS:
		{
		}
		case GLFW_RELEASE:
		{
			state = 2;
		}
		default:
			break;
		}
		return;
	}
	default:
		break;
	}
}

void glfw_error_callback(int errcode, const char *msg)
{
	cout << endl;
	cout << "ERROR: " << errcode << "  " << msg << endl;
}

// glfw：每当窗口大小改变时（通过操作系统或用户调整大小），这个回调函数就会执行
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	//确保视窗与新的视窗尺寸相匹配；注意宽度和
	//高度将明显大于视网膜显示器上指定的高度。
	glViewport(0, 0, width, height);
}

void showmenu()
{
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(u8"示例"))
		{
			ImGui::MenuItem("Main menu bar");
			ImGui::MenuItem("Console");
			ImGui::MenuItem("Log");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void showframe()
{
	ImGui::Begin("othe frame");
	ImGui::Text(u8"测试文本\n测试号或或或或或");
	if (IsItemClicked(0))
	{
		cout << GetIO().MousePos.x << "   " << GetIO().MousePos.y << endl;
	}
	ImGui::End();
}

DetectionFrame showimage(DetectionFrame &userdata)
{
	if (userdata.camerarunstate == "single")
	{
		if (userdata.camerastate == 0)
		{
			userdata.camera >> userdata.imagesource;
			userdata.camerastate = 1;
		}
	}
	if (userdata.camerarunstate == "multi")
	{
		userdata.camera >> userdata.imagesource;
		userdata.camerastate = 0;
	}

	Mat data;
	cvtColor(userdata.imagesource, data, COLOR_BGR2GRAY);
	if (userdata.state == 0)
		cvtColor(data, userdata.image, COLOR_GRAY2RGB);

	if (userdata.rois.size() > 0)
	{
		for (vector<ImVec4>::iterator iter = userdata.rois.begin(); iter != userdata.rois.end(); ++iter)
		{
			cv::rectangle(userdata.image, Point2f(iter->x, iter->y), Point2f(iter->z, iter->w), Scalar(255, 0, 0), 1, LINE_8, 0);
		}
	}
	glGenTextures(1, &userdata.texture);
	glBindTexture(GL_TEXTURE_2D, userdata.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, userdata.image.cols, userdata.image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, userdata.image.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	ImGuiWindowFlags winflag = ImGuiWindowFlags_HorizontalScrollbar;
	Begin(userdata.name.c_str(), 0, userdata.winflag);
	ImVec2 size{
		static_cast<float>(userdata.image.cols),
		static_cast<float>(userdata.image.rows)};
	ImGui::Image((void *)(intptr_t)userdata.texture, size);

	auto scrollx = GetScrollX();
	auto scrolly = GetScrollY();

	mousepos.x = GetIO().MousePos.x - GetCursorScreenPos().x;
	mousepos.y = GetIO().MousePos.y - titleheigh - border + scrolly;

	if (IsItemHovered())
	{
		if (GetIO().MouseDown[0] && userdata.state == 0)
		{
			beginpos = mousepos;
			userdata.state = FRAMESTATE_DRAWINGRECT;
		}
		else
		{
			if (GetIO().MouseDown[0] && userdata.state == FRAMESTATE_DRAWINGRECT)
			{
				cvtColor(data, userdata.image, COLOR_GRAY2RGB);
				cv::rectangle(userdata.image, Point2f(beginpos.x, beginpos.y), Point2f(mousepos.x, mousepos.y), Scalar(255, 0, 0), 1, LINE_8, 0);
			}
		}
		if (GetIO().MouseReleased[0])
		{
			if (userdata.state == FRAMESTATE_DRAWINGRECT)
			{
				endpos = mousepos;
				userdata.rois.push_back(ImVec4(beginpos.x, beginpos.y, endpos.x, endpos.y));
				userdata.state = 0;
			}
			else
			{
				userdata.state = 0;
			}
		}
	}

	bool b = true;
	if (BeginPopupContextWindow("right menu"))
	{
		if (userdata.camerarunstate == "multi"&&b)
		{
			if (Button(u8"单次采集"))
			{
				userdata.camerarunstate = "single";
				b = false;
			}
		}
		if (userdata.camerarunstate == "single"&&b)
		{
			if (Button(u8"连续采集"))
			{
				userdata.camerarunstate = "multi";
				b = false;
			}
		}
		if (!b)CloseCurrentPopup();
		EndPopup();

	}
	End();

	return userdata;
}