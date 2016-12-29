#include "GameObject.h"
#include "GLM\gtc\type_ptr.hpp"

GameObject::GameObject(LoadObject * _model, btRigidBody *_body)
{
	model = _model;
	body = _body;
	tex = -1;
}

GameObject::GameObject(LoadObject * _model, btRigidBody *_body, GLuint _tex)
{
	model = _model;
	body = _body;
	tex = _tex;
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
	// Bind texture here if has one
	if (tex >= 0)
	{

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