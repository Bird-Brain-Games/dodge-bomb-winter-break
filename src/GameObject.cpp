#include "GameObject.h"
#include <GLM\gtx\rotate_vector.hpp>

GameObject::GameObject(LoadObject * _obj)
{
	obj = _obj; // reuse that there memory
	position = glm::vec3(0.0, 0.0, 0.0);
	velocity = glm::vec3(0.0, 0.0, 0.0);
	yRotation = 0.0f;
}

GameObject::GameObject(char * filepath)
{
	obj = new LoadObject();
	obj->loadFromObject(filepath);
	position = glm::vec3(0.0, 0.0, 0.0);
	velocity = glm::vec3(0.0, 0.0, 0.0);
	yRotation = 0.0f;
}

void GameObject::draw(Shader *s)
{
	// bind tex here if you had one

	// compute local transformation
	glm::mat4x4 rotation = glm::rotate(yRotation, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4x4 translation = glm::translate(position);

	glm::mat4x4 transform = translation * rotation;
	s->uniformMat4x4("localTransform", &transform);

	obj->draw();
}

void GameObject::update(float deltaT)
{
	position = position + (velocity*deltaT);
}


void GameObject::setPos(glm::vec3 const & _set) { position = _set; }
void GameObject::setVel(glm::vec3 const & _set) { velocity = _set; }
void GameObject::setAcc(glm::vec3 const & _set) { acc = _set; }
void GameObject::setRot(glm::vec3 const & _set) { rot = _set; }
void GameObject::setMass(float _mass) { mass = _mass; }

void GameObject::addPos(glm::vec3 const & _set) { position += _set; }
void GameObject::addVel(glm::vec3 const & _set) { velocity += _set; }
void GameObject::addAcc(glm::vec3 const & _set) { acc += _set; }
void GameObject::addRot(glm::vec3 const & _set) { rot += _set; }

glm::vec3 const & GameObject::getPos() const { return  position; }
glm::vec3 const & GameObject::getVel() const { return  velocity; }
glm::vec3 const & GameObject::getAcc() const { return  acc; }
glm::vec3 const & GameObject::getRot() const { return  rot; }
glm::vec3 const & GameObject::getScale() const { return scale; }