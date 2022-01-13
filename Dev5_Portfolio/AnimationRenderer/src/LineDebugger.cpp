#include "LineDebugger.h"
// Returns Success or No Room Available for More Lines
bool LineDebugger::AddLine(ColorVertex vertex1, ColorVertex vertex2)
{
	if (m_lineList.size() >= MAX_LINE_VERTS - 1)
		return false;

	m_lineList.push_back(vertex1);
	m_lineList.push_back(vertex2);
	++m_LineCount;
	return true;
}
void LineDebugger::ClearLines()
{
	m_lineList.clear();
	m_LineCount = 0;
}
void LineDebugger::CreateGrid(int pWidth, int pHeight, float pSpacing, ColorVertex pCenter)
{
	// Get 4 Corners of Grid
	float3 topLeft;
	topLeft.x = pCenter.pos.x - 0.5f * pWidth;
	topLeft.y = pCenter.pos.y;
	topLeft.z = pCenter.pos.z - 0.5f * pHeight;

	float3 bottomLeft = topLeft;
	bottomLeft.z = -bottomLeft.z;

	float3 topRight = topLeft;
	topRight.x = -topRight.x;

	unsigned int num_xSquares = pWidth / pSpacing;
	unsigned int num_ySquares = pHeight / pSpacing;

	// Get Y Axis Lines
	for (int i = 0; i < num_xSquares; i++)
	{

	}
	// Get X Axis Lines
	for (int i = 0; i < num_ySquares; i++)
	{

	}


	
}
