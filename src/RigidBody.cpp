#include "RigidBody.h"
#include <iostream>

PhysicsEngine RigidBody::Sys;

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////	PhysicsEngine
PhysicsEngine::PhysicsEngine()
{
	useDebug = false;
	systemInit = false;
}

PhysicsEngine::~PhysicsEngine()
{
	// Destroy debugger
	delete debugger; debugger = nullptr;

	// Destroy Bullet core variables
	delete dynamicsWorld;	dynamicsWorld = nullptr;
	delete solver;	solver = nullptr;
	delete collisionConfiguration;	collisionConfiguration = nullptr;
	delete dispatcher;	dispatcher = nullptr;
	delete broadphase;	broadphase = nullptr;
}

bool PhysicsEngine::init()
{
	if (systemInit) return true;

	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	// The debugger
	debugger = new BulletDebugger();
	dynamicsWorld->setDebugDrawer(debugger);
	return true;
}

void PhysicsEngine::update(float deltaTasSeconds, int maxStep)
{
	dynamicsWorld->stepSimulation(deltaTasSeconds, maxStep);
}

void PhysicsEngine::drawDebug(glm::mat4x4 const& modelViewMatrix, glm::mat4x4 const& projectionMatrix)
{
	debugger->SetMatrices(modelViewMatrix, projectionMatrix);
	dynamicsWorld->debugDrawWorld();
}

btRigidBody::btRigidBodyConstructionInfo* PhysicsEngine::getRigidBodyCI(std::string fileName)
{

}

void PhysicsEngine::addRigidBody(btRigidBody* rb)
{
	dynamicsWorld->addRigidBody(rb);
}

void PhysicsEngine::removeRigidBody(btRigidBody* rb)
{
	dynamicsWorld->removeRigidBody(rb);
}

void PhysicsEngine::setDebugDraw(bool isDrawing)
{
	useDebug = isDrawing;

	if (useDebug)
		debugger->setDebugMode(1);
	else
		debugger->setDebugMode(0);
}

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////	RigidBody
RigidBody::RigidBody()
{
	body = nullptr;
}

RigidBody::~RigidBody()
{
	Sys.removeRigidBody(body);
	body = nullptr;
}

bool RigidBody::load(std::string fileName)
{
	if (!Sys.init()) return false;

	btRigidBody::btRigidBodyConstructionInfo* rigidCI;
	try
	{
		rigidCI = Sys.getRigidBodyCI(fileName);
	}
	catch (char const* e)
	{
		std::cerr << e << std::endl;
		return false;
	}
	
	body = new btRigidBody(*rigidCI);
	body->setMotionState(new btDefaultMotionState());
	Sys.addRigidBody(body);
	return true;
}

void RigidBody::systemUpdate(float deltaTasSeconds, int maxStep)
{
	Sys.update(deltaTasSeconds, maxStep);
}

void RigidBody::drawDebug(glm::mat4x4 const& modelViewMatrix, glm::mat4x4 const& projectionMatrix)
{
	Sys.drawDebug(modelViewMatrix, projectionMatrix);
}