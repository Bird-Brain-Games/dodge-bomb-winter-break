#pragma once

// (C) Bird Brain Games 2017
// main.cpp 

// Core Libraries
#include <iostream>
#include <string>
#include <math.h>

// 3rd Party Libraries
#include "gl\glew.h"
#include "gl\freeglut.h"
#include <GLM\glm.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtx\rotate_vector.hpp>
#include <IL\ilut.h>

#include <btBulletDynamicsCommon.h>

// user headers
#include "loadObject.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Shader.h"

// create game object
std::vector<GameObject*> objects;
std::vector<btCollisionShape*> collisionShapes;

// Create Shader
Shader *shader;
glm::vec3 lightPosition(10.0, 0.0, 0.0);

// Monitor our Projections
glm::mat4x4 projectionMatrix;
glm::mat4x4 modelViewMatrix;

// Bullet core variables
btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;

// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Miliseconds per frame

int windowWidth = 1024;
int windowHeight = 768;

int mousepositionX;
int mousepositionY;

glm::vec3 cameraPosition(0.0f, 0.0f, 10.0f);
glm::vec3 forwardVector(0.0f, 0.0f, -1.0f);
glm::vec3 rightVector;
float movementScalar = 0.1f;

// A few conversions to know
const float degToRad = 3.14159f / 180.0f;
const float radToDeg = 180.0f / 3.14159f;

// separate, cleaner, draw function
void drawObjects()
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->draw(shader);
	}
}

void initObjects()
{
	// Shapes can be reused, to conserve memory
	btCollisionShape* groundShape = new btBoxShape(btVector3(2.5, 0.5, 2.5));
	btCollisionShape* ballShape = new btSphereShape(1);

	collisionShapes.push_back(groundShape);
	collisionShapes.push_back(ballShape);

	// Determine the ball information
	// quaternion - rotation, vec3 - position
	btDefaultMotionState* ballMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	
	btScalar mass = 1;
	btVector3 ballInertia(0, 0, 0);
	ballShape->calculateLocalInertia(mass, ballInertia);

	btRigidBody::btRigidBodyConstructionInfo ballRigidBodyCI(mass, ballMotionState, ballShape, ballInertia);
	btRigidBody* ballRigidBody = new btRigidBody(ballRigidBodyCI);	// Can use construction info for multiple objects
	dynamicsWorld->addRigidBody(ballRigidBody);

	// Determine the ground information
	// quaternion - rotation, vec3 - position
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	
	mass = 0;	// no mass means immovable
	
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(mass, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);


	// Load the .obj files
	LoadObject* ballModel = new LoadObject();
	ballModel->loadFromObject("obj\\sphere.obj");

	LoadObject* groundModel = new LoadObject();
	groundModel->loadFromObject("obj\\5x5box.obj");


	// Create the game objects
	GameObject* ball = new GameObject(ballModel, ballRigidBody);
	GameObject* ground = new GameObject(groundModel, groundRigidBody);

	objects.push_back(ball);
	objects.push_back(ground);
}

bool initBullet()
{
	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));
	return true;
}

/* function DisplayCallbackFunction(void)
* Description:
*  - this is the openGL display routine
*  - this draws the sprites appropriately
*/
void DisplayCallbackFunction(void)
{
	////////////////////////////////////////////////////////////////// Clear our screen
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////////////////////////////////////////////////////////////////// Draw Our Scene
	glViewport(0, 0, windowWidth, windowHeight);
	projectionMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 10000.f);
	modelViewMatrix = glm::mat4x4(1.f);

	glm::mat4x4 transform = glm::lookAt(
		cameraPosition,
		cameraPosition + forwardVector,
		glm::vec3(0.f, 1.f, 0.f));
	modelViewMatrix = transform * modelViewMatrix;

	shader->bind();

	// Draw our scene

	shader->unbind();

	glutSwapBuffers();
}

/* function void KeyboardCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is pressed
*/
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	KEYBOARD_INPUT->SetActive(key, true);

	switch (key)
	{
	case 27: // the escape key
		//exit(0);
		glutLeaveMainLoop();
		break;
	}
}

/* function void KeyboardUpCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is lifted
*/
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	KEYBOARD_INPUT->SetActive(key, false);
}

/* function TimerCallbackFunction(int value)
* Description:
*  - this is called many times per second
*  - this enables you to animate things
*  - no drawing, just changing the state
*  - changes the frame number and calls for a redisplay
*  - FRAME_DELAY is the number of milliseconds to wait before calling the timer again
*/
void TimerCallbackFunction(int value)
{
	//// process inputs

	KEYBOARD_INPUT->WipeEventList();



	//// update physics
	static unsigned int oldTimeSinceStart = 0;
	unsigned int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	unsigned int deltaT = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;

	float deltaTasSeconds = float(deltaT) / 1000.0f;

	// Bullet step through world simulation
	dynamicsWorld->stepSimulation(deltaTasSeconds, 10);

	/*for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->update(deltaTasSeconds);
	}*/

	//// force draw call next tick
	glutPostRedisplay();

	//// delay timestep to maintain framerate
	glutTimerFunc(FRAME_DELAY, TimerCallbackFunction, 0);
}

/* function WindowReshapeCallbackFunction()
* Description:
*  - this is called whenever the window is resized
*  - and sets up the projection matrix properly
*  - currently set up for an orthographic view (2D only)
*/
void WindowReshapeCallbackFunction(int w, int h)
{
	// switch to projection because we're changing projection
	float asp = (float)w / (float)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	windowWidth = w;
	windowHeight = h;

	//switch back to modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	// Handle mouse clicks
	if (state == GLUT_DOWN)
	{
		std::cout << "Mouse X: " << x << " Y: " << y << std::endl;
	}
}


/* function MouseMotionCallbackFunction()
* Description:
*   - this is called when the mouse is clicked and moves
*/
void MouseMotionCallbackFunction(int x, int y)
{
	float changeX = x - mousepositionX;
	float changeY = y - mousepositionY;
	mousepositionX = x;
	mousepositionY = y;
}

/* function MousePassiveMotionCallbackFunction()
* Description:
*   - this is called when the mouse is moved in the window
*/
void MousePassiveMotionCallbackFunction(int x, int y)
{
	mousepositionX = x;
	mousepositionY = y;
}

/* function main()
* Description:
*  - this is the main function
*  - does initialization and then calls glutMainLoop() to start the event handler
*/
int main(int argc, char **argv)
{
	// initialize the window and OpenGL properly
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Dodge Bomb");

	// Initialize OpenGL Extention Wrangler
	GLenum res = glewInit();
	if (res != GLEW_OK)
		std::cerr << "Glew failed to initialize!" << std::endl;

	// check OpenGL version
	printf("Detecting OpenGL version... ");
	const unsigned char *version = glGetString(GL_VERSION);
	printf("Using OpenGL version %s\n", version);


	// set up our function callbacks
	glutDisplayFunc(DisplayCallbackFunction); // draw
	glutKeyboardFunc(KeyboardCallbackFunction); // keyDown
	glutKeyboardUpFunc(KeyboardUpCallbackFunction); // keyUp
	glutReshapeFunc(WindowReshapeCallbackFunction); // windowResized
	glutMouseFunc(MouseClickCallbackFunction); // mouseClick
	glutMotionFunc(MouseMotionCallbackFunction); // mouseMovedActive
	glutPassiveMotionFunc(MousePassiveMotionCallbackFunction); // mouseMovedPassive
	glutTimerFunc(1, TimerCallbackFunction, 0); // timer or tick

	////////////////////////////////////////////	Call some OpenGL parameters
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Turn on the lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH); //GL_FLAT

	////////////////////////////////////////////	Textures & Texture parameters
	glEnable(GL_TEXTURE_2D);
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);

	// Magnification, minification, mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	/* Initialize Shader */
	shader = new Shader("shaders//passthru_v.glsl", "shaders//passthru_f.glsl");
	shader->bind();
	glEnableVertexAttribArray(4);	glBindAttribLocation(shader->getID(), 4, "vPos");
	glEnableVertexAttribArray(5);	glBindAttribLocation(shader->getID(), 5, "texture");
	glEnableVertexAttribArray(6);	glBindAttribLocation(shader->getID(), 6, "normal");
	glEnableVertexAttribArray(7);	glBindAttribLocation(shader->getID(), 7, "color");

	// Initialize Bullet physics
	initBullet();
	
	// Load Textures

	glBindTexture(GL_TEXTURE_2D, 0);


	// Load objects
	initObjects();


	/* start the event handler */
	glutMainLoop();

	delete shader; shader = NULL;
	KEYBOARD_INPUT->Destroy();

	// Remove rigid bodies
	for (unsigned i = 0; i < objects.size(); i++)
	{
		dynamicsWorld->removeRigidBody(objects.at(i)->getRigidBody());
		delete objects.at(i);
		objects.at(i) = nullptr;
	}

	// Destroy collision shapes
	for (unsigned i = 0; i < collisionShapes.size(); i++)
	{
		delete collisionShapes.at(i);
		collisionShapes.at(i) = nullptr;
	}

	// Destroy Bullet core variables
	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
	return 0;
}