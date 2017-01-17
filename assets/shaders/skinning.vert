#version 330

uniform mat4 mvp; // our model View Projection Matrix
uniform int boneCount;
uniform highp mat4 BoneMatrixArray[28];

layout (location = 0) in vec3 inPosition;//our objects position
layout (location = 1) in vec4 bones;//The bones that are affecting this vertex
layout (location = 2) in vec4 weights; //The weights for the bones we are using.
layout (location = 3) in vec2 vertexUV;
layout (location = 4) in vec3 normal;

//out vec3 colour;
out vec2 UV;

void main()
{
	mediump ivec4 boneIndex = ivec4(bones);
	mediump vec4 boneWeights = weights;
	
	highp vec4 position = vec4(0.0, 0.0, 0.0, 0.0);
	for (lowp int i = 0; i < 4; i++)
	{
		if (boneIndex.x < boneCount && boneIndex.x >= 0)
		{
			if (boneWeights.x > 0)
			{
				highp mat4 boneMatrix = BoneMatrixArray[boneIndex.x];
				position += boneMatrix * vec4(inPosition, 1.0) * boneWeights.x;
			}
		}
		boneIndex = boneIndex.yzwx;
		boneWeights = boneWeights.yzwx;
	}
	
	UV = vertexUV;
	
	gl_Position = mvp*position;
	//colour = vec3(0.0, 0.0, 1.0);
}