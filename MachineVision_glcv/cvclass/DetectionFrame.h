#pragma once
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include<opencv2\videoio.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\core\opengl.hpp>
#include <opencv2\core\types.hpp>
#include <imgui.h>

using namespace std;
using namespace cv;

#define CAMERASTATE_SINGLE	10
#define CAMERASTATE_MULTI	11

#define FRAMESTATE_DRAWINGRECT	20

#define FRAMESTATE_OTHER		100


class DetectionFrame
{
public:
	DetectionFrame();
	~DetectionFrame();

	void Grab();

public:
	string name;
	string nameid;
	int mousestate;
	float scrollx;
	float scrolly;
	int camerastate;
	int state;
	unsigned int texture;
	string camerarunstate;
	vector<ImVec4> rois;
	ImGuiWindowFlags winflag;
	VideoCapture camera;

	Mat image;
	Mat imagesource;
};

