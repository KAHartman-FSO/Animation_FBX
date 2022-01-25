#pragma once
#include <fbxsdk.h>
#include <DirectXMath.h>
#include <array>
#include <vector>
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
	
	struct Triangle
	{

	};
	int mTriangleCount = 0;
	std::vector<Triangle> mTriangles;
	

private:
	
	void ProcessMesh(FbxNode* inNode);

	void ClearControlPoints();
	/*	
	*		Gets the positions of all the vertices of a mesh
	* 
	*		@param inNode is the Node in this FBX Scene that contains the Mesh
	*/
	void ProcessControlPoints(FbxNode* inNode);

	/*
	*	@param inMesh					>> the mesh we are trying to export
	*	 @param inCtrlPoinIndex	>> the index of the Control Point
	*	@param inVertexCounte	r	>> index of current vertex we are processing
	*	@param outNormal			>> the output
	*/
	void ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal);

	void ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVindex, int inUVLayer, XMFLOAT2& outUV);
};

