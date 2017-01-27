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

#include <string>
#include <GLM\glm.hpp>
#include "loadObject.h"
#include "Shader.h"
#include <iostream>
#include <btBulletDynamicsCommon.h>

// Contains the information necessary to create a game object
// Used to generate gameObjects using allocated resources in a level
struct GameObjectConstructionInfo
{
	LoadObject *model;
	btRigidBody::btRigidBodyConstructionInfo *rigidBodyCI;
	Texture *tex;
	std::string tag;

	GameObjectConstructionInfo::GameObjectConstructionInfo()
	{
		model = nullptr;
		rigidBodyCI = nullptr;
		tex = nullptr;
		tag = "";
	}

	bool init(std::string fileName);
};

class GameObject
{
public:
	GameObject(LoadObject* _model, btRigidBody* _body);
	GameObject(LoadObject* _model, btRigidBody* _body, Texture* _tex, std::string _tag = "Undefined");
	GameObject(GameObjectConstructionInfo, std::string name);
	~GameObject();

	virtual void draw(Shader *s);
	virtual void update(float deltaT);

	btRigidBody* getRigidBody() { return body; };

private:
	// Physics rigid body with Bullet
	btRigidBody* body;

	// loaded obj file
	LoadObject* model;

	// Texture
	Texture* tex;

	// Identifier Tag
	std::string const tag;

	// Unique name
	std::string name;
};