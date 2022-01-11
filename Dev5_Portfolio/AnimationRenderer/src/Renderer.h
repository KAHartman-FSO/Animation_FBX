#pragma once
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

#include "tools.h"
#include "XMPLvShader.csh"
#include "XMPLpShader.csh"

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
		vertexBuffer->Release();
		vertexShader->Release();
	}

	void Render();
	void DXSetUp(HWND _window);

private:
	HRESULT hr;
	HWND m_window;
	RECT m_windowRect;

	ID3D11Device* m_Device;
	IDXGISwapChain* m_SwapChain;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D11RenderTargetView* m_RTV;
	D3D11_VIEWPORT m_ViewPort;

	ID3D11Buffer* vertexBuffer;
	ID3D11InputLayout* vLayout;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

};