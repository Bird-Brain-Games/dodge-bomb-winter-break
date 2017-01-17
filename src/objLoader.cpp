#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>
#include <gl\glew.h>
#include "objLoader.h"

Loader::Loader()
{

}

Loader::Loader(char const* filePath)
{
	load(filePath);
}

bool Loader::load(char const* fileName)
{
	FILE *file = fopen(fileName, "r");//second parameter specifies what were doing with file ie read
	if (file == NULL)
	{
		printf("you didn't open a file");
		return false;
	}
	int lineCount = 0;


	std::vector<unsigned int> vertexIndecies, uvIndecies, normalindiecies;
	std::vector<glm::vec3>	temporary_vertices;
	std::vector<glm::vec2>	temporary_uvs;
	std::vector<glm::vec3>	temporary_normals;
	while (1)
	{
		char line[255];
		//read first word of the line
		int res = fscanf(file, "%s", line);
		if (res == EOF)
		{
			break;
		}

		if (strcmp(line, "vt") == 0)
		{
			glm::vec2 textureCoord;
			fscanf(file, "%f %f\n", &textureCoord.x, &textureCoord.y);
			temporary_uvs.push_back(textureCoord);
		}
		else if (strcmp(line, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temporary_normals.push_back(normal);
		}
		else if (strcmp(line, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temporary_vertices.push_back(vertex);

		}
		else if (strcmp(line, "f") == 0)
		{
			//std::cout << strcmp(line, "f") << std::endl;
			unsigned int vertexIndex[3], UVindex[3], normalIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &UVindex[0], &normalIndex[0],
				&vertexIndex[1], &UVindex[1], &normalIndex[1],
				&vertexIndex[2], &UVindex[2], &normalIndex[2]);
			vertexIndecies.push_back(vertexIndex[0]);
			vertexIndecies.push_back(vertexIndex[1]);
			vertexIndecies.push_back(vertexIndex[2]);

			//vboModelData.addData(&vVertices[iVertIndex - 1], sizeof(glm::vec3));
			//data.insert(data.end(), (BYTE*)ptrData, (BYTE*)ptrData + uiDataSize);

			uvIndecies.push_back(UVindex[0]);
			uvIndecies.push_back(UVindex[1]);
			uvIndecies.push_back(UVindex[2]);

			normalindiecies.push_back(normalIndex[0]);
			normalindiecies.push_back(normalIndex[1]);
			normalindiecies.push_back(normalIndex[2]);
		}

	}
	//for each vertex of each triangle 
	for (unsigned int i = 0; i < vertexIndecies.size(); i++)
	{
		unsigned int vertexIndex = vertexIndecies[i];
		
		glm::vec3 vertex = temporary_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
		switch (i % 3)
		{
		case 0:
			color.push_back(1.0f);
			color.push_back(0.0f);
			color.push_back(0.0f);
			break;
		case 1:
			color.push_back(0.0f);
			color.push_back(1.0f);
			color.push_back(0.0f);
			break;
		case 2:
			color.push_back(0.0f);
			color.push_back(0.0f);
			color.push_back(1.0f);
			break;
		}
	}
	
	for (unsigned int i = 0; i < uvIndecies.size(); i++)
	{
		unsigned int uvIndex = uvIndecies[i];
		glm::vec2 uvs = temporary_uvs[uvIndex - 1];
		out_uvs.push_back(uvs);
	}
	
	for (unsigned int i = 0; i < normalindiecies.size(); i++)
	{
		unsigned int normalIndex = normalindiecies[i];
		glm::vec3 normal = temporary_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}

	return true;

}

std::vector<glm::vec3> const& Loader::getVertices() const { return out_vertices; }
std::vector<glm::vec2> const& Loader::getUVs() const { return out_uvs; }
std::vector<glm::vec3> const& Loader::getNormals() const { return out_normals; }
std::vector<float> const& Loader::getColors() const { return color; }

void Loader::setVertex(int index, glm::vec3 const& newVertex)
{
	out_vertices.at(index) = newVertex;
}

void Loader::setVertices(std::vector<glm::vec3> const& newVertices)
{
	out_vertices = newVertices;
	/*for (int i = 0; i < getVertices().size(); i++)
	{
		setVertex(i, newVertices.at(i));
	}*/
}



Animation::Animation()
{
	addPose(Loader());
	reset();
}

Animation::Animation(const char* filePath)
{
	std::ifstream inFile(filePath);
	if (!inFile)
	{
		std::cerr << "Error reading file: " << filePath << std::endl;
		throw (-1);
	}

	std::string line;
	while (std::getline(inFile, line))
	{
		addPose(line.c_str());
	}


	inFile.close();
	reset();
}

Animation::Animation(std::vector<Loader> const& _poseList)
{
	for (int i = 0; i < _poseList.size(); i++)
	{
		addPose(_poseList.at(i));
	}
	reset();
}

void Animation::addPose(const char* filePath)
{
	poseList.push_back(Loader(filePath));
}

void Animation::addPose(Loader const& loader)
{
	poseList.push_back(loader);
}

void Animation::reset()
{
	time = 0.0f;
	currentPose = 0;
	nextPose = 0;
}

template <typename T>
std::vector<T> vectorLerp(std::vector<T> const& p0, std::vector<T> const& p1, float t)
{
	std::vector<T> output;

	for (int count = 0; count < p0.size(); count++)
	{
		output.push_back((1.0f - t) * p0[count] + p1[count] * t);
	}

	return output;
}

void Animation::update(float deltaTime, Loader & base)
{
	// Update the time (currently all pose transitions last 1s.)
	time += deltaTime;

	if (time > 1.0f)
	{
		time = 0.0f;
		currentPose = nextPose;
		nextPose++;
		if (nextPose >= poseList.size())
		{
			nextPose = 0;
		}
	}
	//std::vector<glm::vec3>::const_iterator end = poseList.at(currentPose).getVertices().end();
	//std::vector<glm::vec3>::const_iterator p0 = poseList.at(currentPose).getVertices().begin();
	//std::vector<glm::vec3>::const_iterator p1 = poseList.at(nextPose).getVertices().begin();

	//int iterator = 0;
	//while(p0 != end)
	//{ 
	//	//base.setVertex(iterator, (1.0f - time) * *p0 + *p1 * time);
	//	iterator++;
	//	p0++;
	//	p1++;
	//}

	/*std::vector<glm::vec3> newVertices =
		vectorLerp<glm::vec3>
		(
			poseList.at(currentPose).getVertices(),
			poseList.at(nextPose).getVertices(),
			time
		);*/

	/*base.setVertices
	(
		newVertices
	);*/
}