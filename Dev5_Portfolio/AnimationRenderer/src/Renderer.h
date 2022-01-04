#pragma once
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")

class Renderer
{
public:
	// Initializes Necessary Class Variables
	// Sets Default Renderer Settings
	Renderer(HWND& window);
	

private:
	HRESULT hr;
	HWND* m_window;
	RECT m_windowRect;

	ID3D11Device* m_Device;
	IDXGISwapChain* m_SwapChain;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D10RenderTargetView* m_RTV;
	D3D11_VIEWPORT m_ViewPort;

};