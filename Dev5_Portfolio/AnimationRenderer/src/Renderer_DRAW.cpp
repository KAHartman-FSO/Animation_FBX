#include "Renderer.h"
void Renderer::DrawCalls()
{
	m_DeviceContext->IASetInputLayout(m_ColorVertexLayout);
	Draw_Pyramid();
	Draw_Lines();
	
}

void Renderer::Draw_Pyramid()
{
	const int PYRAMID_VERTS = 12;

	// World Matrix Set
	XMMATRIX temp = XMMatrixIdentity();
	temp = XMMatrixTranslation(0, 0, 1);

	float rot_speed = 3;
	static float theta = 0;
	theta += rot_speed * m_deltaTime;
	XMMATRIX temp2 = XMMatrixRotationY(theta);

	temp = XMMatrixMultiply(temp2, temp);
	XMStoreFloat4x4(&m_Matrices.world, temp);

	// Sending Data to GPU
	D3D11_MAPPED_SUBRESOURCE gpuBuffer;
	hr = m_DeviceContext->Map(m_WVPConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	memcpy(gpuBuffer.pData, &m_Matrices, sizeof(WVP));
	m_DeviceContext->Unmap(m_WVPConstantBuffer, 0);

	ID3D11Buffer* constants[] = { m_WVPConstantBuffer };
	m_DeviceContext->VSSetConstantBuffers(0, 1, constants);

	UINT strides[] = { sizeof(tools::ColorVertex) };
	UINT offsets[] = { 0 };
	ID3D11Buffer* tempVB[] = { m_PyramidVertexBuffer };
	m_DeviceContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->VSSetShader(m_VertexShader, 0, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, 0, 0);

	m_DeviceContext->Draw(PYRAMID_VERTS, 0);
}

void Renderer::Draw_Lines()
{
	XMMATRIX temp = XMMatrixIdentity();
	XMStoreFloat4x4(&m_Matrices.world, temp);

	// Sending Data to GPU
	D3D11_MAPPED_SUBRESOURCE gpuBuffer;
	hr = m_DeviceContext->Map(m_WVPConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	memcpy(gpuBuffer.pData, &m_Matrices, sizeof(WVP));
	m_DeviceContext->Unmap(m_WVPConstantBuffer, 0);

	ID3D11Buffer* constants[] = { m_WVPConstantBuffer };
	m_DeviceContext->VSSetConstantBuffers(0, 1, constants);

	UINT strides[] = { sizeof(tools::ColorVertex) };
	UINT offsets[] = { 0 };
	ID3D11Buffer* tempVB[] = { m_LineVertexBuffer };
	m_DeviceContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_DeviceContext->VSSetShader(m_VertexShader, 0, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, 0, 0);

	m_DeviceContext->Draw(m_lineDebugger.GetLineCount(), 0);
}

