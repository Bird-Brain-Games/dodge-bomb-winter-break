#include "fbxLoader.h"
fbxLoader::fbxLoader(FbxString _name)
{
	std::vector<float> temp;
	for (int i = 0; i < 613; i++)
	{
		weights.push_back(glm::vec4(0));
		joints.push_back(glm::vec4(-1));
	}
	FbxManager* lSdkManager = NULL;
	FbxScene* lScene = NULL;
	bool lResult;


	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, lScene);
	// Load the scene.
	// The example can take a FBX file as an argument.
	FbxString lFilePath("assets\\fbx\\" + _name);

	if (lFilePath.IsEmpty())
	{
		lResult = false;
		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
	}

	if (lResult == false)
	{
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
	}
	FbxNode* lNode = lScene->GetRootNode();


	if (lNode)
	{
		int vertexId = 0;
		for (int i = 0; i < lNode->GetChildCount(); i++)
		{

			recursive(lNode->GetChild(i));
		}
	}

	std::vector<float> stop;

}


void fbxLoader::recursive(FbxNode *pNode)
{

	FbxNodeAttribute::EType lAttributeType;
	lAttributeType = pNode->GetNodeAttribute()->GetAttributeType();
	switch (lAttributeType)
	{
	default:
		break;
	case FbxNodeAttribute::eMesh:
		FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
		savePolygons(lMesh);
		skinWeights(lMesh);
		for (int i = 0; i <= weights.size() - 1; i++)
		{
			float total = weights[i].x + weights[i].y + weights[i].z + weights[i].w;
			if (total != 1)
			{
				weights[i].x = weights[i].x / total;
				weights[i].x = weights[i].x * 1.0f;

				weights[i].y = weights[i].y / total;
				weights[i].y = weights[i].y * 1.0f;

				weights[i].z = weights[i].z / total;
				weights[i].z = weights[i].z * 1.0f;

				weights[i].w = weights[i].w / total;
				weights[i].w = weights[i].w * 1.0f;
			}
		}
		for (int i = 0; i <= controlPoint.size() - 1; i++)
		{
			jointsFixed.push_back(joints[controlPoint[i]]);
			weightFixed.push_back(weights[controlPoint[i]]);
		}
		break;
	}

	for (int i = 0; i < pNode->GetChildCount(); i++)
	{

		recursive(pNode->GetChild(i));
	}
}


void fbxLoader::savePolygons(FbxMesh* pMesh)
{
	int i, j, l;
	int vertexId = 0;
	int	lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	for (i = 0; i < lPolygonCount; i++)
	{
		int lPolygonSize = pMesh->GetPolygonSize(i);

		for (j = 0; j < lPolygonSize; j++)
		{

			//fetch vertecies.
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			vertexs.push_back(glm::vec3(
				(float)lControlPoints[lControlPointIndex][0],
				(float)lControlPoints[lControlPointIndex][1],
				(float)lControlPoints[lControlPointIndex][2]
			));

			// Fetch uv coordinates
			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
				int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
				FbxVector2 pValue = leUV->GetDirectArray().GetAt(lTextureUVIndex);

				UV.push_back(glm::vec2(
					(float)pValue[0],
					(float)pValue[1]
				));
			}
			//fetch normals
			for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
				FbxVector4 pValue = leNormal->GetDirectArray().GetAt(vertexId);
				normals.push_back(glm::vec3(
					(float)pValue[0],
					(float)pValue[1],
					(float)pValue[2]
				));
			}

			controlPoint.push_back(lControlPointIndex);
			vertexId++;
		}
	} // for polygonSize

}

void fbxLoader::skinWeights(FbxGeometry* pGeometry)
{
	//Display cluster now

	//int i, lLinkCount;
	//FbxCluster* lLink;

	int i, j;
	int lSkinCount = 0;
	int lClusterCount = 0;
	FbxCluster* lCluster;

	lSkinCount = pGeometry->GetDeformerCount(FbxDeformer::eSkin);



	//lLinkCount = pGeometry->GetLinkCount();
	for (i = 0; i != lSkinCount; ++i)
	{
		lClusterCount = ((FbxSkin *)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
		for (j = 0; j != lClusterCount; ++j)
		{
			lCluster = ((FbxSkin *)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);
			//lLink = pGeometry->GetLink(i);    


			FbxString lString1 = "        Link Indices: ";
			FbxString lString2 = "        Weight Values: ";

			int k, lIndexCount = lCluster->GetControlPointIndicesCount();
			int* lIndices = lCluster->GetControlPointIndices();
			double* lWeights = lCluster->GetControlPointWeights();

			for (k = 0; k < lIndexCount; k++)
			{
				printf("spot: %d \n", lIndices[k]);
				//weights[lIndices[k]].push_back((float)lWeights[k]);
				for (int l = 0; l < 4; l++)
				{
					switch (l)
					{
					case 0:
						if (joints[lIndices[k]].x == -1)
						{
							joints[lIndices[k]].x = j;
							weights[lIndices[k]].x = (float)lWeights[k];
							l = 5;
						}
						break;
					case 1:
						if (joints[lIndices[k]].y == -1)
						{
							joints[lIndices[k]].y = j;
							weights[lIndices[k]].y = (float)lWeights[k];
							l = 5;
						}
						break;
					case 2:
						if (joints[lIndices[k]].z == -1)
						{
							joints[lIndices[k]].z = j;
							weights[lIndices[k]].z = (float)lWeights[k];
							l = 5;
						}
						break;
					case 3:
						if (joints[lIndices[k]].w == -1)
						{
							joints[lIndices[k]].w = j;
							weights[lIndices[k]].w = (float)lWeights[k];
							l = 5;
						}
						break;

					}
				}
			}


			//base translation

			//lMatrix = lCluster->GetTransformMatrix(lMatrix);
			//Display3DVector("        Transform Translation: ", lMatrix.GetT());
			//Display3DVector("        Transform Rotation: ", lMatrix.GetR());
			//Display3DVector("        Transform Scaling: ", lMatrix.GetS());

			//lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
			//Display3DVector("        Transform Link Translation: ", lMatrix.GetT());
			//Display3DVector("        Transform Link Rotation: ", lMatrix.GetR());
			//Display3DVector("        Transform Link Scaling: ", lMatrix.GetS());

			//if (lCluster->GetAssociateModel() != NULL)
			//{
			//	lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
			//	DisplayString("        Associate Model: ", (char *)lCluster->GetAssociateModel()->GetName());
			//	Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
			//	Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
			//	Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
			//}

			//DisplayString("");
		}
	}
}




bool fbxLoader::InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	pManager = FbxManager::Create();

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	pScene = FbxScene::Create(pManager, "My Scene");
	if (!pScene)
	{
		return false;
	}
	return true;
}


bool fbxLoader::LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		return false;
	}


	if (lImporter->IsFBX())
	{
		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);


	// Destroy the importer.
	lImporter->Destroy();



	return lStatus;
}
std::vector<glm::vec4> fbxLoader::getWeights() { return weightFixed; };
std::vector<glm::vec4> fbxLoader::getJoints() { return jointsFixed; };

