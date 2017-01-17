#pragma once

#include <vector>
#include "glm\glm.hpp"
#include "fbxsdk.h"

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

class fbxLoader
{
public:

	fbxLoader(FbxString _name);
	std::vector<glm::vec4> getWeights();
	std::vector<glm::vec4> getJoints();
	std::vector<glm::vec3> vertexs;
	std::vector<glm::vec2> UV;
	std::vector<glm::vec3> normals;
private:
	std::vector<int> controlPoint;// the order of the control points

	std::vector<glm::vec4> weights;//the weights by control point
	std::vector<glm::vec4> joints;//the joint each weight is affecting.

	std::vector<glm::vec4> weightFixed;//the weights for each vertex
	std::vector<glm::vec4> jointsFixed;//the joint for each vertex

	bool InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
	bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);
	void recursive(FbxNode *pNode);
	void skinWeights(FbxGeometry* pGeometry);
	void fbxLoader::savePolygons(FbxMesh* pMesh);
};

