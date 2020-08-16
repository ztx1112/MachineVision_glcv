#pragma once


#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

namespace Zopengl
{
	class Shader
	{
		//此渲染器 ID
		unsigned int ID;

	public:
		Shader();

		//vertexpath :顶点着色器路径
		//fragmentpath :片段着色器路径
		Shader(const GLchar* vertexpath, const GLchar* fragmentpath);	

		//使用此渲染器
		void Use();
		//返回此着色器ID
		unsigned int getid();

		// uniform工具函数
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
	};

}

