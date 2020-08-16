#include "stdafx.h"
#include "cvclass/DetectionFrame.h"
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

using namespace ImGui;

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
		static_cast<float>(userdata.image.rows) };
	ImGui::Image((void *)(intptr_t)userdata.texture, size);

	auto scrollx = GetScrollX();
	auto scrolly = GetScrollY();

	mousepos.x = GetIO().MousePos.x - GetCursorScreenPos().x;
	mousepos.y = GetIO().MousePos.y - 45 - 4 + scrolly;

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
	End();

	return userdata;
}