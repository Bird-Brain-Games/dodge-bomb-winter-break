#define _CRT_SECURE_NO_WARNINGS
#include "shaderLoader.h"
#include "gl\glew.h"


ShaderLoader::ShaderLoader()
{
	loaded = false;
}

UINT ShaderLoader::getShaderID()
{
	return shaderID;
}

bool ShaderLoader::getLoaded()
{
	return loaded;
}

bool ShaderLoader::loadShader(std::string name, int shaderType)
{
	FILE* fp = fopen(name.c_str(), "rt"); // fp: filepath


	char line[255];
	std::vector<std::string> lines;


	while (fgets(line, 255, fp))
	{
		lines.push_back(line);
	}
	fclose(fp);
	const char** size = new const char*[(int)lines.size()];
	for (int count = 0; count < (int)lines.size(); count++)
	{
		size[count] = lines[count].c_str();
	}
	shaderID = glCreateShader(shaderType);

	glShaderSource(shaderID, lines.size(), size, NULL);
	glCompileShader(shaderID);

	delete[] size;

	int iCompilationStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &iCompilationStatus);

	if (iCompilationStatus == GL_FALSE)
	{
		OutputProgramLog();
		return false;
	}
	sType = shaderType;
	loaded = true;

	return true;
}

void ShaderLoader::deleteShader()
{
	if (getLoaded() == false) return;
	loaded = false;
	glDeleteShader(shaderID);
}

void ShaderLoader::OutputProgramLog() const
{
	GLint maxLength = 0;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	std::vector<GLchar> errorLog(maxLength);
	glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);
}

ShaderProgram::ShaderProgram()
{
	linked = false;
}


void ShaderProgram::createProgram()
{
	programID = glCreateProgram();//creates the program
}

void ShaderProgram::deleteProgram()
{
	if (linked == false) return;

	linked = false;
	glDeleteProgram(programID);
}

bool ShaderProgram::addShader(ShaderLoader *shader)
{
	if (shader->getLoaded() == false) return false;

	glAttachShader(programID, shader->getShaderID());//adds the shader we loaded to the program

	return true;
}
bool ShaderProgram::linkProgram()
{
	glLinkProgram(programID);
	int linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	linked = linkStatus == GL_TRUE;

	// TEMP
	return linked;
}
UINT ShaderProgram::getProgramID()
{
	return programID;
}

void ShaderProgram::useProgram()
{
	if (linked)
	{
		glUseProgram(programID);
	}
}

void ShaderProgram::OutputProgramLog() const
{
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLen);
	glGetProgramInfoLog(programID, sizeof(char) * 512, &infoLen, &infoLog[0]); std::cout << std::string(infoLog.begin(), infoLog.end()) << "\n";
}

