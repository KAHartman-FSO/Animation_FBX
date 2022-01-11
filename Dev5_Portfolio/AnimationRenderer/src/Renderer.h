#pragma once
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <DirectXMath.h>
using namespace DirectX;

#include "tools.h"
#include "XMPLvShader.csh"
#include "XMPLpShader.csh"
struct WVP
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};
class Renderer
{
public:
	Renderer();
	~Renderer()
	{
		m_Device->Release();
		m_SwapChain->Release();
		m_DeviceContext->Release();
		m_RTV->Release();
		m_VertexBuffer->Release();
		m_VertexShader->Release();
		m_PixelShader->Release();
		m_VertexLayout->Release();
	}

	void Render();
	void DXSetUp(HWND _window);

private:
	HRESULT hr;
	HWND m_window;
	RECT m_windowRect;
	float m_aspectRatio = 1;

	ID3D11Device* m_Device;
	IDXGISwapChain* m_SwapChain;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D11RenderTargetView* m_RTV;
	D3D11_VIEWPORT m_ViewPort;

	ID3D11Buffer* m_VertexBuffer;
	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11Buffer* m_ConstantBuffer;

	
	WVP m_Matrices;


	void CreateDXVars();
	void CreateViewProjectionMatrices();
	void Prep_Pyramid();
	void Draw_Pyramid();
};