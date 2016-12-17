// GameObject.h
// - Class that represents a object in game
// - Bare minimum must have
// -- a renderable (a sprite, a 3d obj, a wavefront obj)
// -- A position in gamespace (x,y,z)
// - additional properties may be
// -- velocity
// -- acceleration
// -- rotations (x, y, z)
// -- texture
// -- collision bounds (radius, xMins and xMaxs for cube)
#pragma once

#include <GLM\glm.hpp>
#include "loadObject.h"
#include "Shader.h"
#include <iostream>

class GameObject
{
public:
	GameObject(LoadObject * _obj);
	GameObject(char * filepath);

	void draw(Shader *s);
	void update(float deltaT);

public:
	////////////////////////////////////////////////	Utility functions
	//setters and getters
	void setPos(glm::vec3 const & _set);
	void setVel(glm::vec3 const & _set);
	void setAcc(glm::vec3 const & _set);
	void setRot(glm::vec3 const & _set);
	void setMass(float _mass);
	void setScale(glm::vec3 newScale, bool changeBoundingBox = true);

	void addPos(glm::vec3 const & _set);
	void addVel(glm::vec3 const & _set);
	void addAcc(glm::vec3 const & _set);
	void addRot(glm::vec3 const & _set);

	glm::vec3 const & getPos() const;
	glm::vec3 const & getVel() const;
	glm::vec3 const & getAcc() const;
	glm::vec3 const & getRot() const;
	glm::vec3 const & getScale() const;

private:
	// Bare minimum
	glm::vec3 position;
	GLfloat yRotation;

	// loaded obj file
	LoadObject *obj;

	// physics properties
	float mass;
	glm::vec3 velocity;
	glm::vec3 acc;
	glm::vec3 rot;

	glm::vec3 dimension;
	glm::vec3 scale;

};