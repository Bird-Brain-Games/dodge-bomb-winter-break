#include "GameObject.h"
#include <iostream>
#include "TTK\GraphicsUtils.h"
GameObject::GameObject()
	: m_pScale(1.0f),
	colour(glm::vec4(1.0)),
	m_pCurrentFrame(0),
	jointAnimation(nullptr),
	sphere("assets\\obj\\ball.obj")
{
	

}

GameObject::~GameObject() {}

void GameObject::init()
{
	glGenVertexArrays(1, &VAO); 
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.getVertices().size() * sizeof(glm::vec3), sphere.getVertices().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	for (int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->init();
}

void GameObject::setPosition(glm::vec3 newPosition)
{
	m_pLocalPosition = newPosition;
}

void GameObject::setRotationAngleX(float newAngle)
{
	m_pRotX = newAngle;
}

void GameObject::setRotationAngleY(float newAngle)
{
	m_pRotY = newAngle;
}

void GameObject::setRotationAngleZ(float newAngle)
{
	m_pRotZ = newAngle;
}

void GameObject::setScale(float newScale)
{
	m_pScale = newScale;
}

glm::mat4 GameObject::getLocalToWorldMatrix()
{
	return m_pLocalToWorldMatrix;
}

void GameObject::update(float dt)
{
	frame++;
	//if (frameDelay == frame)
	//{
	frame = 0;
	// Create 4x4 transformation matrix

	// If there is no animation for this joint, create the transform matrix as usual
	if (jointAnimation == nullptr)
	{
		// Create rotation matrix
		glm::mat4 rx = glm::rotate(glm::radians(m_pRotX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 ry = glm::rotate(glm::radians(m_pRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rz = glm::rotate(glm::radians(m_pRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

		// Note: pay attention to rotation order, ZYX is not the same as XYZ
		// Order is specified by an HTR file when you export it
		m_pLocalRotation = rz * ry * rx;

	}
	else // Transform comes from HTR
	{

		m_pLocalRotation =
			glm::mat4_cast(jointAnimation->jointBaseRotation * jointAnimation->jointRotations[m_pCurrentFrame]);

		m_pLocalPosition = jointAnimation->jointBasePosition + jointAnimation->jointPositions[m_pCurrentFrame];

		jointAnimation->jointScales[m_pCurrentFrame];

		// Increment frame (note: you could do this based on dt)
		m_pCurrentFrame++;

		if (m_pCurrentFrame >= jointAnimation->numFrames - 1)
			m_pCurrentFrame = 0;

	}

	// Create translation matrix
	glm::mat4 tran = glm::translate(m_pLocalPosition);

	// Create scale matrix
	glm::mat4 scal = glm::scale(glm::vec3(m_pScale, m_pScale, m_pScale));

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a transform, then we must apply the parent's transform
	m_pLocalTransformMatrix = tran * m_pLocalRotation * scal;

	if (m_pParent != this)
		m_pLocalToWorldMatrix = m_pParent->getLocalToWorldMatrix() * m_pLocalTransformMatrix;
	else
		m_pLocalToWorldMatrix = m_pLocalTransformMatrix;

	//}
		// Update children
	for (int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->update(dt);

}

void GameObject::update()
{
	
	// Create translation matrix
	glm::mat4 tran = glm::translate(m_pLocalPosition);

	// Create scale matrix
	glm::mat4 scal = glm::scale(glm::vec3(m_pScale, m_pScale, m_pScale));

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a transform, then we must apply the parent's transform
	m_pLocalTransformMatrix = tran * m_pLocalRotation * scal;

	if (m_pParent != this)
		m_pLocalToWorldMatrix = m_pParent->getLocalToWorldMatrix() * m_pLocalTransformMatrix;
	else
		m_pLocalToWorldMatrix = m_pLocalTransformMatrix;

	//}
	// Update children
	for (int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->update();

}

void GameObject::createBase(std::vector<glm::mat4> &temp, int &count)
{
	frame++;
	//if (frameDelay == frame)
	//{
	frame = 0;
	// Create 4x4 transformation matrix

	// If there is no animation for this joint, create the transform matrix as usual
	if (jointAnimation == nullptr)
	{
		// Create rotation matrix
		glm::mat4 rx = glm::rotate(glm::radians(m_pRotX), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 ry = glm::rotate(glm::radians(m_pRotY), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rz = glm::rotate(glm::radians(m_pRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

		// Note: pay attention to rotation order, ZYX is not the same as XYZ
		// Order is specified by an HTR file when you export it
		m_pLocalRotation = rz * ry * rx;

	}
	else // Transform comes from HTR
	{
		int size = jointAnimation->jointRotations.size();
		m_pLocalRotation =
			glm::mat4_cast(jointAnimation->jointBaseRotation * jointAnimation->jointRotations[size - 1]);

		m_pLocalPosition = jointAnimation->jointBasePosition + jointAnimation->jointPositions[size - 1];

		jointAnimation->jointScales[size - 1];

		count++;
	}

	// Create translation matrix
	glm::mat4 tran = glm::translate(m_pLocalPosition);

	// Create scale matrix
	glm::mat4 scal = glm::scale(glm::vec3(m_pScale, m_pScale, m_pScale));

	// Combine all above transforms into a single matrix
	// This is the local transformation matrix, ie. where is this game object relative to it's parent
	// If a game object has no parent (it is a root node) then its local transform is also it's global transform
	// If a game object has a transform, then we must apply the parent's transform
	m_pLocalTransformMatrix = tran * m_pLocalRotation * scal;

	if (m_pParent)
		m_pLocalToWorldMatrix = m_pParent->getLocalToWorldMatrix() * m_pLocalTransformMatrix;
	else
		m_pLocalToWorldMatrix = m_pLocalTransformMatrix;

	if (count >= 0)
		temp[count] = m_pLocalToWorldMatrix;

	//}
	// Update children
	for (int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->createBase(temp, count);

}


void GameObject::setMatrixStackOrignal(std::vector<glm::mat4> &temp, int &count)
{
	if (jointAnimation != NULL)
	{
		temp[count] = glm::mat4(1);
		count++;
	}
	// Update children
	for (int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->setMatrixStackOrignal(temp, count);
}

void GameObject::getMatrixStack(std::vector<glm::mat4> &temp, std::vector<glm::mat4> &origin, int &count)
{
	if (jointAnimation != NULL)
	{
		this;
		glm::mat4 identity;
		temp[count] = m_pLocalToWorldMatrix * glm::inverse(origin[count]);
		identity = glm::inverse(origin[count]);
		identity = identity * m_pLocalToWorldMatrix;
		count++;
	}
	// Update children
	for (int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->getMatrixStack(temp, origin, count);
}


void GameObject::draw(glm::mat4 mvp, ShaderProgram shader)
{
	glBindVertexArray(VAO);
	glm::mat4 mvp2 = mvp * m_pLocalToWorldMatrix;
	int imvp2 = glGetUniformLocation(shader.getProgramID(), "mvp");
	glUniformMatrix4fv(imvp2, 1, GL_FALSE, glm::value_ptr(mvp2));
	glDrawArrays(GL_TRIANGLES, 0, sphere.getVertices().size());
	// Draw children
	for (int i = 0; i < m_pChildren.size(); ++i)
		m_pChildren[i]->draw(mvp, shader);
}

void GameObject::setParent(GameObject* newParent)
{
	m_pParent = newParent;
}

void GameObject::addChild(GameObject* newChild)
{
	if (newChild)
	{
		m_pChildren.push_back(newChild);
		newChild->setParent(this); // tell new child that this game object is its parent
	}
}

void GameObject::removeChild(GameObject* rip)
{
	for (int i = 0; i < m_pChildren.size(); ++i)
	{
		if (m_pChildren[i] == rip) // compare memory locations (pointers)
		{
			std::cout << "Removing child: " + rip->name << " from object: " << this->name;
			m_pChildren.erase(m_pChildren.begin() + i);
		}
	}
}

glm::vec3 GameObject::getWorldPosition()
{
	if (m_pParent)
		return m_pParent->getLocalToWorldMatrix() * glm::vec4(m_pLocalPosition, 1.0f);
	else
		return m_pLocalPosition;
}

glm::mat4 GameObject::getWorldRotation()
{
	if (m_pParent)
		return m_pParent->getWorldRotation() * m_pLocalRotation;
	else
		return m_pLocalRotation;
}

bool GameObject::isRoot()
{
	if (m_pParent)
		return false;
	else
		return true;
}

