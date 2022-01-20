#pragma once
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <array>
using namespace DirectX;
#define MAX_CONTROL_POINTS 10000

class FBXExporter
{
private:
	struct CtrlPoint
	{
		XMFLOAT3 mPosition;
	};
	unsigned int ctrlPointCount = 0;
	std::array<CtrlPoint*, MAX_CONTROL_POINTS> mControlPoints;
	

private:
	
	void ClearControlPoints();
	/*	
	*		Gets the positions of all the vertices of a mesh
	* 
	*		@param inNode is the Node in this FBX Scene that contains the Mesh
	*/
	void ProcessControlPoints(FbxNode* inNode);

	/*
	*	@param inMesh >> the mesh we are trying to export
	*	 @param inCtrlPoinIndex >> the index of the Control Point
	*	@param inVertexCounter >> index of current vertex we are processing
	*	@param outNormal >> the output
	*/
	void ReadNormal(FbxMesh* inMesh, int inCtrlPoinIndex, int inVertexCounter, XMFLOAT3& outNormal);
};

