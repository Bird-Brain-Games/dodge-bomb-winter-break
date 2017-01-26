#include "GameObject.h"
#include "GLM\gtc\type_ptr.hpp"

GameObject::GameObject(LoadObject* _model, btRigidBody* _body)
{
	model = _model;
	body = _body;
	tex = nullptr;
	hierarchy = nullptr;
}

GameObject::GameObject(LoadObject* _model, btRigidBody* _body, Texture* _tex)
{
	hierarchy = nullptr;
	model = _model;
	body = _body;
	tex = _tex;
}

GameObject::GameObject(Holder* _hierarchy, btRigidBody* _body, Texture* _tex)
{
	model = nullptr;
	hierarchy = _hierarchy;
	body = _body;
	tex = _tex;
}

GameObject::~GameObject()
{
	delete body->getMotionState();
	delete body;

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
	if (model == nullptr && hierarchy != nullptr)
	{
		hierarchy->draw(s);
	}
	else if (model != nullptr && hierarchy == nullptr)
	{
		model->draw();
	}
}

void GameObject::update(float dt)
{
	if (hierarchy)
	{
		hierarchy->update(dt);
	}
}