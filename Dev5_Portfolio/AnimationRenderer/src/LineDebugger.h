#pragma once
#define WHITE {0, 0, 0, 1}
#include "tools.h"
#include <vector>
using namespace tools;
// This Class's Job is to Store all Lines into One big Buffer ( Containing 1 - 2 Colors and 2 Color Vertexes )
class LineDebugger
{
public:
	
	void ClearLines();
	void CreateGrid(int pWidth, int pHeight, float pSpacing, ColorVertex pCenter);
	//void CreateMatrixLines();

	unsigned int GetSize()
	{
		return m_LineCount * sizeof(ColorVertex);
	}
	unsigned int GetLineCount()
	{
		return m_LineCount;
	}

private:
	static const int MAX_LINE_VERTS = 10000;
	unsigned int m_LineCount;
	void AddLine(ColorVertex vertex1, ColorVertex vertex2);

public:
	ColorVertex m_LineArray[MAX_LINE_VERTS];
};

