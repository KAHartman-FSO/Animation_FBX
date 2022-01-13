#pragma once
#define WHITE {0, 0, 0, 1}
#include "tools.h"
#include <vector>
using namespace tools;
// This Class's Job is to Store all Lines into One big Buffer ( Containing 1 - 2 Colors and 2 Color Vertexes )
class LineDebugger
{
public:
	bool AddLine(ColorVertex vertex1, ColorVertex vertex2);
	void ClearLines();
	
	void CreateGrid(int pWidth, int pHeight, float pSpacing, ColorVertex pCenter);

private:
	const size_t MAX_LINE_VERTS = 10000;
	unsigned int m_LineCount;
	std::vector<ColorVertex> m_lineList;

};

