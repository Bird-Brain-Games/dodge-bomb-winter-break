#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <gl\glew.h>
#include <glm\glm.hpp>

class uv
{
public:
	float u, v;
};

class Loader
{
public:
	Loader();
	Loader(char const* filePath);

	bool load(char const* filePath);
	std::vector<glm::vec3> const& getVertices() const;
	std::vector<glm::vec2> const& getUVs() const;
	std::vector<glm::vec3> const& getNormals() const;
	std::vector<float> const& getColors() const;
	void setVertex(int index, glm::vec3 const& newVertex);
	void setVertices(std::vector<glm::vec3> const& newVertices);

	std::vector<glm::vec3> out_vertices;
private:
	std::vector<glm::vec2> out_uvs;
	std::vector<glm::vec3> out_normals;
	std::vector<float> color;
};

// Animation class used to control flow of objects and speed
// Holds a list of poses that can be morphed between
class Animation
{
public:
	Animation();
	Animation(const char* filePath);
	Animation(std::vector<Loader> const&);
	
	// Updates the given base loader based on the current animation
	void update(float deltaTime, Loader & base);

	// Resets the values to the original ones
	void reset();

private:
	void addPose(Loader const&);
	void addPose(const char* filePath);

private:
	std::vector<Loader> poseList;
	int currentPose, nextPose;
	
	// To put speed control in here
	float time;
};