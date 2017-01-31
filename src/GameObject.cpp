#include "GameObject.h"

#include <iostream>
#include <fstream>

#include "RigidBody.h"

GameObject::GameObject(LoadObject* _model, RigidBody* _body)
	: tag("Undefined")
{
	model = _model;
	body = _body;
	tex = nullptr;
}

GameObject::GameObject(LoadObject* _model, RigidBody* _body, Texture* _tex, std::string _tag)
	: tag(_tag)
{
	model = _model;
	body = _body;
	tex = _tex;
}

GameObject::~GameObject()
{
	if (body)
	{
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
	s->uniformMat4x4("localTransform", &body->getWorldTransform());
	
	model->draw();
}

void GameObject::update(float dt)
{

}