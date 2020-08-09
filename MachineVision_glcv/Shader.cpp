#include "Shader.h"

Zopengl::Shader::Shader()
{
}

Zopengl::Shader::Shader(const GLchar* vertexpath, const GLchar* fragmentpath)
{
	ifstream vertfile,fragfile;
	string vertshadercode, fragshadercode;

	vertfile.exceptions(ios::failbit | ios::eofbit | ios::badbit);
	fragfile.exceptions(ios::failbit | ios::eofbit | ios::badbit);
	try
	{
		vertfile.open(vertexpath);
		fragfile.open(fragmentpath);

		stringstream vertstream, fragstream;
		vertstream << vertfile.rdbuf();
		fragstream << fragfile.rdbuf();
		vertshadercode = vertstream.str();
		fragshadercode = fragstream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vshadercode;
	const char* fshadercode;
	vshadercode = vertshadercode.c_str();
	fshadercode = fragshadercode.c_str();

	unsigned int vshader, fshader;
	vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader, 1, &vshadercode, NULL);
	glCompileShader(vshader);
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader, 1, &fshadercode, NULL);
	glCompileShader(fshader);

	ID = glCreateProgram();
	glAttachShader(ID, vshader);
	glAttachShader(ID, fshader);
	glLinkProgram(ID);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
}

void Zopengl::Shader::Use()
{
	glUseProgram(ID);
}

unsigned int Zopengl::Shader::getid()
{
	return ID;
}

void Zopengl::Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Zopengl::Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Zopengl::Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
