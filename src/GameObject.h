#pragma once

#include <GLM/glm.hpp>
#include <GLM\gtx\transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <vector>
#include <string>

#include "objLoader.h"
#include "JointTypes.h"
#include "shaderLoader.h"

class GameObject
{

protected:

	float m_pScale;

	float m_pRotX, m_pRotY, m_pRotZ; // local rotation angles


	glm::mat4 m_pLocalRotation;

	glm::mat4 m_pLocalTransformMatrix;
	glm::mat4 m_pLocalToWorldMatrix;

	glm::mat4 baseTranslation;


	// Forward Kinematics
	

	// HTR animation
	unsigned int m_pCurrentFrame;

	int frameDelay = 1;
	int frame = 0;
	Loader sphere;
	GLuint VBO;
	GLuint VAO;


public:
	glm::vec3 m_pLocalPosition;

	GameObject();
	GameObject(std::string _name, glm::vec3 position);
	~GameObject();
	
	void init();

	void setPosition(glm::vec3 newPosition);
	void setRotationAngleX(float newAngle);
	void setRotationAngleY(float newAngle);
	void setRotationAngleZ(float newAngle);
	void setScale(float newScale);

	glm::mat4 getLocalToWorldMatrix();

	virtual void update(float dt);	
	virtual void update();
	void GameObject::createBase(std::vector<glm::mat4> &temp, int &count);
	void GameObject::setMatrixStackOrignal(std::vector<glm::mat4>&, int&);
	void GameObject::getMatrixStack(std::vector<glm::mat4>&, std::vector<glm::mat4>&, int&);
	virtual void draw(glm::mat4, ShaderProgram);

	// Animation from HTR
	JointDescriptor* jointAnimation;

	GameObject* m_pParent;
	std::vector<GameObject*> m_pChildren;
	

	// Forward Kinematics
	// Pass in null to make game object a root node
	void setParent(GameObject* newParent);
	void addChild(GameObject* newChild);
	void removeChild(GameObject* rip);
	glm::vec3 getWorldPosition();
	glm::mat4 getWorldRotation();
	bool isRoot();

	// Other Properties
	std::string name;
	glm::vec4 colour; 
	// Mesh* ...
	// Material* ...
};