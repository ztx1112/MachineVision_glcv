#include "DetectionFrame.h"



DetectionFrame::DetectionFrame()
{
	name = "image";
	nameid = "";
	mousestate = 0;
	camerastate = 0;
	state = 0;
	unsigned int texture;
	camerarunstate = "single";
	vector<ImVec4> rois;
	winflag = ImGuiWindowFlags_HorizontalScrollbar;

}


DetectionFrame::~DetectionFrame()
{
}
