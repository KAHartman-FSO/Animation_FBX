#include "Renderer.h"
void Renderer::LoadCalls()
{
	Load_Pyramid();
	Load_Lines();
}

void Renderer::Load_Lines()
{
	tools::ColorVertex gridCenterVertex = { {0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} };
	m_lineDebugger.CreateGrid(64, 64, 0.25f, gridCenterVertex);

	// Create a Buffer
	D3D11_BUFFER_DESC vbDesc;
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbDesc.ByteWidth = m_lineDebugger.GetSize();
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = NULL;
	vbDesc.MiscFlags = NULL;
	vbDesc.StructureByteStride = 0;
	subData.pSysMem = m_lineDebugger.m_LineArray;
	hr = m_Device->CreateBuffer(&vbDesc, &subData, &m_LineVertexBuffer);
}

void Renderer::Load_Pyramid()
{
	tools::ColorVertex pyramid[]
	{
		// Front
		{{0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{0.25f, -0.25, -0.25f, 1.0f}, {0.0f, 0.0f,1.0f, 1.0f}},
		{{-0.25f, -0.25, -0.25f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		// Right
		{ {0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{0.25f, -0.25, 0.25f, 1.0f}, {0.0f, 0.0f,1.0f, 1.0f}},
		{{0.25f, -0.25, -0.25f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		// Left
		{{0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{-0.25f, -0.25, -0.25f, 1.0f}, {0.0f, 0.0f,1.0f, 1.0f}},
		{{-0.25f, -0.25, 0.25f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		// Back
		{{0.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{-0.25f, -0.25, 0.25f, 1.0f}, {0.0f, 0.0f,1.0f, 1.0f}},
		{{0.25f, -0.25, 0.25f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}
	};
	D3D11_BUFFER_DESC vbDesc;
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbDesc.ByteWidth = sizeof(pyramid);
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = NULL;
	vbDesc.MiscFlags = NULL;
	vbDesc.StructureByteStride = 0;
	subData.pSysMem = pyramid;
	hr = m_Device->CreateBuffer(&vbDesc, &subData, &m_PyramidVertexBuffer);
}