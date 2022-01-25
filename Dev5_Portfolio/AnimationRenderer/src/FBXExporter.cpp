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

void FBXExporter::ReadNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal)
{
	if (inMesh->GetElementNormalCount() < 1) { throw std::exception("Invalid Normal Number"); }

	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);

	switch (vertexNormal->GetMappingMode())
	{
		// Each Control Point has 1 Normal
			case	FbxGeometryElement::eByControlPoint:
					switch (vertexNormal->GetReferenceMode())
					{
							case FbxGeometryElement::eDirect:
							{
								outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
								outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
								outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
							}
							break;

							case FbxGeometryElement::eIndexToDirect:
							{
								int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
								outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
								outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
								outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
							}
							break;

							default:
								throw std::exception("Invalid Reference");
					}
					break;
			// Need to get normals of each vertex, on each face = Sharp Edges
			case FbxGeometryElement::eByPolygonVertex:
				switch (vertexNormal->GetReferenceMode())
				{
							case FbxGeometryElement::eDirect:
							{
								outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
								outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
								outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
							}
							break;
							case FbxGeometryElement::eIndexToDirect:
							{
								int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
								outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
								outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
								outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
							}
							break;

							default:
								throw std::exception("Invalid Reference");
				}
				break;

			default:
				break;
	}
}
void FBXExporter::ReadUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVindex, int inUVLayer, XMFLOAT2& outUV) {};

void FBXExporter::ProcessMesh(FbxNode* inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();
	mTriangleCount = currMesh->GetPolygonCount();
	int vertexCounter = 0;
	mTriangles.reserve(mTriangleCount);

	for (unsigned int i = 0; i < mTriangleCount; ++i)
	{
		XMFLOAT3 normal[3];
		XMFLOAT3 tangent[3];
		XMFLOAT3 binormal[3];
		XMFLOAT2 UV[3][2];
		Triangle currTriangle;
		mTriangles.push_back(currTriangle);

		for (unsigned int j = 0; j < 3; ++j)
		{
			// Read Normals
			int ctrlPointIndex = currMesh->GetPolygonVertex(i, j);
			CtrlPoint* currCtrlPoint = mControlPoints[ctrlPointIndex];
			ReadNormal(currMesh, ctrlPointIndex, vertexCounter, normal[j]);

			// Read UVs
			// For only a diffuse texture
			for (int k = 0; k < 1; k++)
			{
				
			}

		}
	}
	

	
}