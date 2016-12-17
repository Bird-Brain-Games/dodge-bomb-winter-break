// NAME HERE
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
	// Bare minimum
	glm::vec3 position;
	GLfloat yRotation;

	// using Cube::draw() for generic renderable
	LoadObject *obj;

	// bonus properties
	glm::vec3 velocity;

	GameObject(LoadObject * _obj)
	{
		obj = _obj; // reuse that there memory
		position = glm::vec3(0.0, 0.0, 0.0);
		velocity = glm::vec3(0.0, 0.0, 0.0);
		yRotation = 0.0f;
	}

	GameObject(char * filepath)
	{
		obj = new LoadObject();
		obj->loadFromObject(filepath);
		position = glm::vec3(0.0, 0.0, 0.0);
		velocity = glm::vec3(0.0, 0.0, 0.0);
		yRotation = 0.0f;
	}


	void draw(Shader *s)
	{
		// bind tex here if you had one

		// compute local transformation
		glm::mat4x4 rotation = glm::rotate(yRotation, glm::vec3(0.0, 1.0, 0.0));
		glm::mat4x4 translation = glm::translate(position);

		glm::mat4x4 transform = translation * rotation;
		s->uniformMat4x4("localTransform", &transform);

		obj->draw();
	}

	void update(float deltaT)
	{
		position = position + (velocity*deltaT);
	}

	bool checkRadialCollision(GameObject *other)
	{
		float thisRadius = 1.0;	// this varies per object
		float otherRadius = 1.0; //our 2x2x2 cube is assumed to have radius of 1 for now

		float minimumSeparation = thisRadius + otherRadius;

		float dist = glm::distance(this->position, other->position);

		if (dist <= minimumSeparation)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool checkCollision(GameObject *other)
	{
		float thisMinX = position.x - 1.0; // position - width/2
		float thisMaxX = position.x + 1.0;
		float thisMinY = position.y - 1.0;
		float thisMaxY = position.y + 1.0;
		float thisMinZ = position.z - 1.0;
		float thisMaxZ = position.z + 1.0;

		float otherMinX = other->position.x - 1.0; // position - width/2
		float otherMaxX = other->position.x + 1.0;
		float otherMinY = other->position.y - 1.0;
		float otherMaxY = other->position.y + 1.0;
		float otherMinZ = other->position.z - 1.0;
		float otherMaxZ = other->position.z + 1.0;

		// check overlap X
		if ((thisMinX >= otherMinX && thisMinX <= otherMaxX) ||
			(thisMaxX >= otherMinX && thisMaxX <= otherMaxX))
		{
			// check overlap Y
			if ((thisMinY >= otherMinY && thisMinY <= otherMaxY) ||
				(thisMaxY >= otherMinY && thisMaxY <= otherMaxY))
			{
				// check overlap Z
				if ((thisMinZ >= otherMinZ && thisMinZ <= otherMaxZ) ||
					(thisMaxZ >= otherMinZ && thisMaxZ <= otherMaxZ))
				{
					return true;
				}
			}
		}
		return false;
	}
};