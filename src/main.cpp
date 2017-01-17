// (C) Bird Brain Games 2017
// main.cpp 

// Core Libraries
#include <iostream>
#include <string>
#include <math.h>
#include <Windows.h>
#include <vector>

// 3rd Party Libraries
#include <GL\glew.h>
#include <GLUT\glut.h>
#include <IL\ilut.h>
#include <GLM\glm.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include <GLM\gtx\rotate_vector.hpp>

// user headers
#include "fbxLoader.h"
#include "shaderLoader.h"

// Monitor our Projections
glm::mat4x4 projectionMatrix;
glm::mat4x4 modelViewMatrix;

// Defines and Core variables
#define FRAMES_PER_SECOND 60
const int FRAME_DELAY = 1000 / FRAMES_PER_SECOND; // Miliseconds per frame

int windowWidth = 600;
int windowHeight = 600;

//mouse variables
float mousepositionX;
float mousepositionY;
float lastMousepositionX;
float lastMousepositionY;
bool mouseMovement;

//camera variables
glm::vec3 up;
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
//camera defaults
glm::vec3 direction;
glm::vec3 position = glm::vec3(0.0f, 0.0f, -10.0f);
glm::vec3 gameDefaultPos = glm::vec3(0.0f);
glm::vec3 menuDefaultPos = glm::vec3(0.0f);
glm::vec2 gameDefaultAngle(0.0f);
glm::vec2 menuDefaultAngle(0.0f);
glm::vec2 currentAngles = gameDefaultAngle;
float mouseSpeed = 0.005f;
float speed = 0.07f;
float initialFoV = 45.0f;

//Timer
float dt;
bool keyDown[256];

//basic vbo's
GLuint VBO[6];
GLuint VAO[2];
GLuint tex;



//HTR variables
#include "HTRLoader.h"
HTRLoader skeleton;
GameObject character;
GameObject *selected;
int level = 0;
std::vector<int> branch;



fbxLoader test("bombot.fbx");
std::vector<glm::mat4> matricies;
std::vector<glm::mat4> multipliedMatricies;
int boneCount = 28;

//shader variables
ShaderLoader vertLoader;
ShaderLoader skinningLoader;
ShaderLoader fragLoader;
ShaderLoader textureLoader;
ShaderProgram basicShader;
ShaderProgram skinning;

std::vector<glm::vec3> tester;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

bool debug = false;


bool cameraLock = true;
void makeMatricies()
{
	if (cameraLock == false)
	{
		float tempX = float(lastMousepositionX - mousepositionX);
		float tempY = float(lastMousepositionY - mousepositionY);
		currentAngles.x += mouseSpeed * tempX;
		currentAngles.y += mouseSpeed * tempY;
	}

	direction = glm::vec3(
		cos(currentAngles.y) * sin(currentAngles.x),
		sin(currentAngles.y),
		cos(currentAngles.y) * cos(currentAngles.x)
	);
	glm::vec3 right = glm::vec3(
		sin(currentAngles.x - 3.14f / 2.0f),
		0,
		cos(currentAngles.x - 3.14f / 2.0f)
	);
	up = glm::cross(right, direction);

	if (keyDown['w'] == true)
	{
		position += direction *  speed;
	}
	// Move backward
	if (keyDown['s'] == true)
	{
		position -= direction *  speed;
	}
	// Strafe right
	if (keyDown['d'] == true)
	{
		position += right *  speed;
	}
	// Strafe left
	if (keyDown['a'] == true)
	{
		position -= right * speed;
	}


	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(initialFoV, (float)windowWidth / windowHeight, 0.1f, 10000.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

}
glm::vec3 translations = glm::vec3(0.0f);
float xRot = 0.0f;
int incrememnt = 0;
int size;
void init()
{

	size = test.vertexs.size();
	glGenVertexArrays(1, VAO);
	glGenBuffers(6, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, test.vertexs.size() * sizeof(glm::vec3), test.vertexs.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, test.getJoints().size() * sizeof(glm::vec4), test.getJoints().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, test.getWeights().size() * sizeof(glm::vec4), test.getWeights().data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, test.UV.size() * sizeof(glm::vec2), test.UV.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, test.normals.size() * sizeof(glm::vec3), test.normals.data(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);

	skeleton.loadHTR("assets\\htr\\bombot4.htr");
	skeleton.createGameObjects();
	character.addChild(skeleton.getRootGameObject());
	character.init();
	character.m_pParent = &character;
	selected = character.m_pChildren[0];
	branch.push_back(0);
	branch.push_back(0);


	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	//glGenSamplers(1, &texSampler[texIt]);
	ilutGLLoadImage("assets\\img\\bombot.png");


	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
		ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
		0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData()); /* Texture specification */

	int temporary = ilGetInteger(IL_IMAGE_HEIGHT);
	for (int i = 0; i < 28; i++)
	{
		matricies.push_back(glm::mat4());
		multipliedMatricies.push_back(glm::mat4());
	}
	int temp = -1;
	character.createBase(matricies, temp);
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





	skinning.useProgram();

	//bind our shader
	makeMatricies();
	// Draw our scene
	// boneCount BoneMatrixArray
	glm::mat4 identity = glm::mat4(1.0f);
	glm::mat4 mvp = getProjectionMatrix() * getViewMatrix() * glm::mat4(1.0f);

	int imvp = glGetUniformLocation(skinning.getProgramID(), "mvp");
	int iBoneCount = glGetUniformLocation(skinning.getProgramID(), "boneCount");
	int iBoneMatrixArray = glGetUniformLocation(skinning.getProgramID(), "BoneMatrixArray");
	int iSampler = glGetUniformLocation(skinning.getProgramID(), "gSampler");


	glBindTexture(GL_TEXTURE_2D, tex);

	glUniform1i(iSampler, 0);
	glUniformMatrix4fv(imvp, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(iBoneMatrixArray, 28, GL_FALSE, glm::value_ptr(multipliedMatricies[0]));
	boneCount = 28;
	glUniform1i(iBoneCount, boneCount);
	glActiveTexture(0);





	glBindVertexArray(VAO[0]);
	if (debug == false)
		glDrawArrays(GL_TRIANGLES, 0, size);
	basicShader.useProgram();
	//character.draw(mvp, basicShader);


	if (debug == true)
	{
		//glm::vec4 v;
		//float weight1 = test.getWeights();
		//float weight2 = 
		//for (int i = 0; i < 36; i++)
		//{
		//	//magic math happens
		//	v = glm::vec4(tester[i], 1.0f);
		//	//	v = v * glm::inverse(root->getMM()) * root->getWM();
		//	if (i < 16)
		//	{
		//		glm::mat4 worldM = root->children[0]->parent->getWM();
		//		glm::mat4 modelM = glm::inverse(root->children[0]->getMM());
		//		glm::mat4 trans = modelM * worldM;
		//
		//		glm::vec4 end = trans * v;
		//		end *= weight1;
		//
		//		worldM = root->children[0]->children[0]->parent->getWM();
		//		modelM = glm::inverse(root->children[0]->children[0]->getMM());
		//		trans = modelM * worldM;
		//		end += weight2 * (trans * v);
		//
		//		object[0].setVerticies(i, end);
		//
		//	}
		//	else
		//	{
		//		glm::mat4 worldM = root->children[0]->children[0]->parent->getWM();
		//		glm::mat4 modelM = glm::inverse(root->children[0]->children[0]->getMM());
		//		glm::mat4 trans = modelM * worldM;
		//
		//		glm::vec4 end = trans * v;
		//		end *= 1;
		//
		//		object[0].setVerticies(i, end);
		//
		//	}
		//	glm::vec3 temp = object[0].getVerticies()[i];
		//	printf("vertex: %i %f %f %f \n", i, temp.x, temp.y, temp.z);
		//
		//}
		//object[0].rebindVertex();

		glBindVertexArray(VAO[0]);
		basicShader.useProgram();
		int imvp2 = glGetUniformLocation(basicShader.getProgramID(), "mvp");
		glUniformMatrix4fv(imvp2, 1, GL_FALSE, glm::value_ptr(mvp));

		glDrawArrays(GL_TRIANGLES, 0, size);
	}
	//unbind our shader


	glutSwapBuffers();
}

/* function void KeyboardCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is pressed
*/
void KeyboardCallbackFunction(unsigned char key, int x, int y)
{
	//should only set what key is held down. check whats happening in the timer callback.
	keyDown[key] = true;
	switch (key)
	{
	case 27: // the escape key
		exit(0);
		break;
	}
}

/* function void KeyboardUpCallbackFunction(unsigned char, int,int)
* Description:
*   - this handles keyboard input when a button is lifted
*/
void KeyboardUpCallbackFunction(unsigned char key, int x, int y)
{
	keyDown[key] = false;
}

void keyboardInput()
{
	if (keyDown['i'])
	{

	}
}
float timeIncrement = 0;
void TimerCallbackFunction(int value)
{
	/* this call makes it actually show up on screen */
	glutPostRedisplay();
	/* this call gives it a proper frame delay to hit our target FPS */

	// Process all inputs /////////////////////////////////////////////////////

	//// update physics	///////////////////////////////////////////////////////
	static int elapsedTimeAtLastTick = 0;
	int totalElapsedTime = glutGet(GLUT_ELAPSED_TIME);

	dt = totalElapsedTime - elapsedTimeAtLastTick;
	dt /= 1000.0f;

	elapsedTimeAtLastTick = totalElapsedTime;


	if (keyDown['h'] && totalElapsedTime - timeIncrement > 250)
	{
		timeIncrement = totalElapsedTime;
		if (selected->m_pChildren.size() > 0)
		{
			level++;
			if (branch.size() - 1 < level)
			{
				branch.push_back(0);
			}
		}
	}
	if (keyDown['n'] && totalElapsedTime - timeIncrement > 250)
	{
		timeIncrement = totalElapsedTime;
		if (level > 0)
		{
			level--;
		}
	}
	if (keyDown['m'] && totalElapsedTime - timeIncrement > 250)
	{
		timeIncrement = totalElapsedTime;
		if (selected->m_pParent->m_pChildren.size() - 1 > branch[level])
			branch[level]++;
	}
	if (keyDown['b'] && totalElapsedTime - timeIncrement > 250)
	{
		timeIncrement = totalElapsedTime;
		if (branch[level] > 0)
			branch[level]--;
	}



	bool temp = true;
	while (temp)
	{
		if (selected != selected->m_pParent)
			selected = selected->m_pParent;
		else
			temp = false;
	}
	for (int i = 0; i < level; i++)
	{
		selected = selected->m_pChildren[branch[i+1]];
	}

	character.update(dt);

	int stack = 0;
	character.getMatrixStack(multipliedMatricies, matricies, stack);
	if (keyDown['i'] == true)
	{
		selected->m_pLocalPosition.y += 0.5;
	}
	// Move backward
	if (keyDown['k'] == true)
	{
		selected->m_pLocalPosition.y -= 0.5;
	}
	// Strafe right
	if (keyDown['l'] == true)
	{
		selected->m_pLocalPosition.z += 0.5;
	}
	// Strafe left
	if (keyDown['j'] == true)
	{
		selected->m_pLocalPosition.z -= 0.5;
	}




	if (keyDown['u'] == true)
	{
		xRot += 0.5;
	}
	// Strafe left
	if (keyDown['o'] == true)
	{
		xRot -= 0.5;
	}
	if (keyDown['z'] == true && totalElapsedTime - timeIncrement > 500)
	{
		timeIncrement = totalElapsedTime;
		debug = !debug;
	}

	//if (keyDown['p'] == true && totalElapsedTime - timeIncrement > 1000)
	//{
	//	incrememnt += 1;
	//	timeIncrement = totalElapsedTime;
	//}
	//// Strafe left
	//if (keyDown[';'] == true && totalElapsedTime - timeIncrement > 1000)
	//{
	//	incrememnt -= 1;
	//	timeIncrement = totalElapsedTime;
	//}


	keyboardInput();
	// Update the objects /////////////////////////////////////////////////////

	/*for (GameObject & o : object)
	{
		o.update(dt);
	}
	for (PlayerObject & o : animation)
	{
		o.update(dt);
		o.bomb.update(dt);
	}*/

	//	handle all events /////////////////////////////////////////////////////
	//handleEvents(dt);


	// if no mouse movement zero the mouse posistions.
	if (mouseMovement == false)
	{
		lastMousepositionX = mousepositionX;
		lastMousepositionY = mousepositionY;
	}

	mouseMovement = false;
	glutTimerFunc(FRAME_DELAY, TimerCallbackFunction, 0); // after x Ticks call again.
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(initialFoV, (float)w / h, 0.1f, 10000.0f);
	windowWidth = w;
	windowHeight = h;
	//glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void MouseClickCallbackFunction(int button, int state, int x, int y)
{
	// Handle mouse clicks
	mouseMovement = true;
	cameraLock = true;

	if (state == GLUT_DOWN)
	{
		if (button == 0)
		{
			cameraLock = false;
		}
		else
		{
			cameraLock = true;
		}
	}
	lastMousepositionX = mousepositionX;
	lastMousepositionY = mousepositionY;
	mousepositionX = x;
	mousepositionY = y;
}


/* function MouseMotionCallbackFunction()
* Description:
*   - this is called when the mouse is clicked and moves
*/
void MouseMotionCallbackFunction(int x, int y)
{
	mouseMovement = true;
	lastMousepositionX = mousepositionX;
	lastMousepositionY = mousepositionY;
	mousepositionX = x;
	mousepositionY = y;
}

/* function MousePassiveMotionCallbackFunction()
* Description:
*   - this is called when the mouse is moved in the window
*/
void MousePassiveMotionCallbackFunction(int x, int y)
{
	mouseMovement = true;
	lastMousepositionX = mousepositionX;
	lastMousepositionY = mousepositionY;
	mousepositionX = x;
	mousepositionY = y;
}


int main(int argc, char **argv)
{
	// initialize the window and OpenGL properly
	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("soul less");

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



	vertLoader.loadShader("assets\\shaders\\shader_colour.vert", GL_VERTEX_SHADER);
	skinningLoader.loadShader("assets\\shaders\\skinning.vert", GL_VERTEX_SHADER);
	fragLoader.loadShader("assets\\shaders\\shader_colour.frag", GL_FRAGMENT_SHADER);
	textureLoader.loadShader("assets\\shaders\\shader_texture.frag", GL_FRAGMENT_SHADER);

	basicShader.createProgram();
	basicShader.addShader(&vertLoader);
	basicShader.addShader(&fragLoader);
	basicShader.linkProgram();


	skinning.createProgram();
	skinning.addShader(&skinningLoader);
	skinning.addShader(&textureLoader);
	skinning.linkProgram();


	init();
	/* start the event handler */
	glutMainLoop();

	//delete shader; shader = NULL;
	return 0;
}

//if (__cplusplus == 201103L) std::cout << "C++11\n";
//else if (__cplusplus == 199711L) std::cout << "C++98\n";
//else std::cout << "pre-standard C++\n";
//
//Character player("Cam");
//Character wolf("Wolf");
//
//
//while (wolf.getHealth() > 0)
//{
//	if (GetAsyncKeyState(VK_UP))
//	{
//		player.attack(&wolf);
//		wolf.attack(&player);
//		Sleep(500);
//	}
//
//}