#pragma once
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

class Renderer
{
public:
	// Initializes Necessary Class Variables
	// Sets Default Renderer Settings
	Renderer();
	~Renderer()
	{
		m_Device->Release();
		m_DeviceContext->Release();
		m_SwapChain->Release();
		m_RTV->Release();
	}

	void Render();
	void DXSetUp();

	// Mutators
	void SetWindow(HWND& _window) { m_window = &_window; GetClientRect(*m_window, &m_windowRect); }

private:
	HRESULT hr;
	HWND* m_window;
	RECT m_windowRect;

	ID3D11Device* m_Device;
	IDXGISwapChain* m_SwapChain;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D11RenderTargetView* m_RTV;
	D3D11_VIEWPORT m_ViewPort;
};