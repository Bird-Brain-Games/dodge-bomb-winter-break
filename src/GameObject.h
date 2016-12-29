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
#include <btBulletDynamicsCommon.h>

class GameObject
{
public:
	GameObject(LoadObject * _model, btRigidBody *_body);
	GameObject(LoadObject * _model, btRigidBody *_body, GLuint _tex);
	~GameObject();

	virtual void draw(Shader *s);
	virtual void update(float deltaT);

	void setTex(GLuint);

	btRigidBody* getRigidBody() { return body; };

private:
	// Physics rigid body with Bullet
	btRigidBody* body;

	// loaded obj file
	LoadObject *model;

	// Texture
	GLuint tex;

};