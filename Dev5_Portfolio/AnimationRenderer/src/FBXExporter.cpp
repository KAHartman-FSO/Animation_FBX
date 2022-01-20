#include "FBXExporter.h"

void FBXExporter::ClearControlPoints()
{
	for (unsigned int i = 0; i < ctrlPointCount; i++)
	{
		delete mControlPoints[i];
	}
	ctrlPointCount = 0;
}
void FBXExporter::ProcessControlPoints(FbxNode* inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();
	unsigned int ctrlPointCount = currMesh->GetControlPointsCount();

	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		CtrlPoint* currCtrlPoint = new CtrlPoint();
		XMFLOAT3 currPosition;
		currPosition.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);
		currCtrlPoint->mPosition = currPosition;

		mControlPoints[ctrlPointCount] = currCtrlPoint;
		++ctrlPointCount;
	}

}

void FBXExporter::ReadNormal(FbxMesh* inMesh, int inCtrlPoinIndex, int inVertexCounter, XMFLOAT3& outNormal)
{

}