#include "LineDebugger.h"
// Returns Success or No Room Available for More Lines
void LineDebugger::AddLine(ColorVertex vertex1, ColorVertex vertex2)
{
	m_LineArray[m_LineCount] = vertex1;
	++m_LineCount;
	m_LineArray[m_LineCount] = vertex2;
	++m_LineCount;
}
void LineDebugger::ClearLines()
{
	m_LineCount = 0;
}
void LineDebugger::CreateGrid(int pWidth, int pHeight, float pSpacing, ColorVertex pCenter)
{
	// Get 4 Corners of Grid
	float3 topLeft;
	topLeft.x = pCenter.pos.x - (0.5f * pWidth * pSpacing);
	topLeft.y = pCenter.pos.y;
	topLeft.z = pCenter.pos.z - (0.5f * pHeight * pSpacing);

	float3 bottomLeft = topLeft;
	bottomLeft.z = -bottomLeft.z;

	float3 topRight = topLeft;
	topRight.x = -topRight.x;

	unsigned int num_xSquares = pWidth / pSpacing;
	unsigned int num_ySquares = pHeight / pSpacing;

	ColorVertex temp;
	temp.col.xyz = pCenter.col.xyz;
	temp.col.w = 1;
	// Get Y Axis Lines
	for (int i = 0; i < pWidth + 1; i++)
	{
		float xOffset = i * pSpacing;
		ColorVertex temp = { {topLeft.x + xOffset, topLeft.y, topLeft.z, 1}, pCenter.col };
		ColorVertex temp2 = { {bottomLeft.x + xOffset, bottomLeft.y, bottomLeft.z, 1}, pCenter.col };
		AddLine(temp, temp2);
	}
	// Get X Axis Lines
	for (int i = 0; i < pHeight + 1; i++)
	{
		float zOffset = i * pSpacing;
		ColorVertex temp = { {topLeft.x, topLeft.y, topLeft.z + zOffset, 1}, pCenter.col };
		ColorVertex temp2 = { {topRight.x, topRight.y, topRight.z + zOffset, 1}, pCenter.col };
		AddLine(temp, temp2);
	}
}
