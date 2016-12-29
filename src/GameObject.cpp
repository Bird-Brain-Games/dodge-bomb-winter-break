#include "GameObject.h"

GameObject::GameObject(LoadObject * _model, btRigidBody *_body)
{
	model = _model;
	body = _body;
}

GameObject::~GameObject()
{
	delete model;
	delete body->getMotionState();
	delete body;

	model = nullptr;
	body = nullptr;
}

void GameObject::draw(Shader* s)
{
	btTransform worldTransform = body->getWorldTransform();
	//worldTransform.
}

void GameObject::update(float dt)
{

}