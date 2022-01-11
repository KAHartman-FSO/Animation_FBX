#pragma once
namespace tools
{
	enum POSITION_VALUES
	{
		x, y, z, w
	};
	enum COLOR_VALUES
	{
		r, g, b, a
	};
	struct ColorVertex
	{
		float pos[4];
		float col[4];
	}; 
}
