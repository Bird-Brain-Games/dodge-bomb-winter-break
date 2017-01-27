#include "GameObject.h"

#include <iostream>
#include <fstream>

#include "GLM\gtc\type_ptr.hpp"


bool GameObjectConstructionInfo::init(std::string fileName)
{
	std::ifstream file(fileName);
	if (!file)
	{
		std::cerr << "Error: File " << fileName << std::endl;
		return false;
	}

	std::string line;
	std::string key;
	std::size_t pos;

	// Rigidbody info


	while (getline(file, line))
	{
		pos = line.find(':');
		if (pos != std::string::npos)
		{
			key = line.substr(0, pos-1);
			line = line.substr(pos + 1);

			if (key == "gameObjectTag")
			{
				tag = line;
			}
			else if (key == "inWorldMatrix")
			{

			}
		}
	}

	file.close();
	return true;
}

GameObject::GameObject(LoadObject* _model, btRigidBody* _body)
	: tag("Undefined")
{
	model = _model;
	body = _body;
	tex = nullptr;
}

GameObject::GameObject(LoadObject* _model, btRigidBody* _body, Texture* _tex, std::string _tag)
	: tag(_tag)
{
	model = _model;
	body = _body;
	tex = _tex;
}

GameObject::GameObject(GameObjectConstructionInfo CI)
	: tag(CI.tag)
{
	model = CI.model;
	tex = CI.tex;

	// Create rigid body
	if (CI.rigidBodyCI)
	{

	}
	else
		body = nullptr;
}

GameObject::~GameObject()
{
	if (body)
	{
		delete body->getMotionState();
		delete body;
	}
	

	model = nullptr;
	body = nullptr;
}

void GameObject::draw(Shader* s)
{
	// Bind texture here if has one
	if (tex != nullptr)
	{
		tex->bind(s);
	}

	// Compute local transformation
	btTransform t;
	btScalar m[16];
	body->getMotionState()->getWorldTransform(t);
	t.getOpenGLMatrix(m);

	s->uniformMat4x4("localTransform", &glm::make_mat4x4((float*)m));
	
	model->draw();
}

void GameObject::update(float dt)
{

}