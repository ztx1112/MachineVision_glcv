#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

namespace Zopengl
{
	class Shader
	{
		//����Ⱦ�� ID
		unsigned int ID;

	public:
		Shader();

		//vertexpath :������ɫ��·��
		//fragmentpath :Ƭ����ɫ��·��
		Shader(const GLchar *vertexpath, const GLchar *fragmentpath);

		//ʹ�ô���Ⱦ��
		void Use();
		//���ش���ɫ��ID
		unsigned int getid();

		// uniform���ߺ���
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
	};

} // namespace Zopengl
