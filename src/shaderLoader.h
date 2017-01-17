#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

class ShaderLoader
{
public:
	//shaderloader functions
	ShaderLoader();
	bool loadShader(std::string fileName, int shaderType);
	void deleteShader();
	bool ShaderLoader::getLoaded();
	UINT getShaderID();

	void ShaderLoader::OutputProgramLog() const;
private:
	//variables for a shader
	UINT shaderID;// Shader ID
	int sType; // what type of shader that the program will load
	bool loaded;// whether everthing worked


	UINT programID;

};


class ShaderProgram
{
public:

	void ShaderProgram::OutputProgramLog() const;

	ShaderProgram();

	void createProgram();
	void deleteProgram();

	bool addShader(ShaderLoader *shader);
	bool linkProgram();

	void useProgram();

	UINT getProgramID();
private:
	UINT programID;
	bool linked;
};

