#pragma once

#include <vector>
#include <map>

#include <btBulletDynamicsCommon.h>

#include "loadObject.h"
#include "GameObject.h"


// Handles memory management and construction of rigidBodies
class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	bool add

private:
	// Holds all construction information to create gameObjects
	// key: given tag
	std::map<std::string, btRigidBody::btRigidBodyConstructionInfo> rigidBodyCIMap;

	// Holds all textures
	// key: filename
	std::map<std::string, Texture> textureMap;

	// Holds all models
	// key: filename
	std::vector<std::string, LoadObject> modelMap;


	// Game manager
	// Player manager














	/*void addGameObject(GameObject* newObject);
	void addCollisionShape(btCollisionShape* newShape);
	void addTexture(Texture* newTexture);
	void addModel(LoadObject* newModel);

	void draw();
	void update(float dt);

private:
	std::vector<GameObject*> objects;
	std::vector<btCollisionShape*> collisionShapes;
	std::vector<Texture*> textures;
	std::vector<LoadObject*> models;*/
};


// Contains all the environmental and stage assets and rigidbodies
class Scene
{
public:
	Scene();
	~Scene();

	bool load(char const* fileName);
	bool 
private:
	// 
	static GameWorld world;

private:
	std::vector<GameObject*> objects;
};