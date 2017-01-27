#pragma once

#include <vector>
#include <map>

#include <btBulletDynamicsCommon.h>

#include "loadObject.h"
#include "GameObject.h"

// Contains all the environmental and stage assets and rigidbodies
class Level
{
public:

private:
	// 

};

// Class that manages all world variables
// and holds templates to create gameObjects
class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void loadLevel(std::string levelFileName);
	void unloadLevel(std::string levelFileName);

private:
	// Contains all loaded levels
	std::map<std::string, Level*> levelMap;

	// Holds all construction information to create gameObjects
	std::map<std::string, GameObjectConstructionInfo> gameObjectCIMap;

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