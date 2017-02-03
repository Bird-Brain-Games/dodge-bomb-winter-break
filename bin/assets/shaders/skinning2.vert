#version 330

uniform mat4 mvp; // our model View Projection Matrix


layout (location = 0) in vec3 inPosition;//our objects position
layout (location = 1) in vec2 inCoord;//texture coordinates (uv)
layout (location = 6) in vec4 bones;//The bones that are affecting this vertex
layout (location = 7) in vec4 weights; //The weights for the bones we are using.

uniform int boneCount;
uniform highp mat4 BoneMatrixArray[28];


out vec2 texCoord;

void main()
{
	mediump ivec4 boneIndex = ivec4(bones);
	mediump vec4 boneWeights = weights;
	
	highp vec4 position;
	for (lowp int i = 0; i < 4; i++)
	{
		if (boneIndex.x < boneCount)
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
	gl_Position = mvp*position;
	texCoord = inCoord;
}