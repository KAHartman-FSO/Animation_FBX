#pragma once
#include <chrono>
namespace tools
{
	// Math Types
	struct float2
	{
		float x, y;
	};
	struct float3
	{
		union
		{
			struct
			{
				float x, y, z;
			};
			float2 xy;
		};
		float3& operator=(const float3& that)
		{
			if (this != &that)
			{
				x = that.x;
				y = that.y;
				z = that.z;
			}
			return *this;
		}
		inline friend float3 operator+(float3 lhs, float3 rhs)
		{
			return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
		}
		inline friend float3 operator-(float3 lhs, float3 rhs)
		{
			return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
		}
		inline friend float3 operator*(float3 lhs, float rhs)
		{
			return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
		}
	};
	struct float4
	{
		union
		{
			struct
			{
				float x, y, z, w;
			};
			float3 xyz;
		};

		float4& operator=(const float4& that)
		{
			x = that.x;
			y = that.y;
			z= that.z;
			w = that.w;

			return *this;
		}
	};

	// Vertex Types for Rendering
	struct ColorVertex
	{
		float4 pos;
		float4 col;

		ColorVertex& operator=(ColorVertex& that)
		{
			pos.xyz = that.pos.xyz;
			pos.w = that.pos.w;
			col.xyz = that.col.xyz;
			col.w = that.col.w;
			return *this;
		}
	}; 
}

