#include "Renderer.h"

Renderer::Renderer(HWND& window)
{
	m_window = &window;
	GetClientRect(*m_window, &m_windowRect);
	
	//	Buffer Descriptor
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.Width = m_windowRect.right - m_windowRect.left;
	bufferDesc.Height = m_windowRect.bottom - m_windowRect.top;
	// Swap Chain Descriptor
	DXGI_SWAP_CHAIN_DESC swap_desc;
	ZeroMemory(&swap_desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swap_desc.BufferCount = 1;
	swap_desc.OutputWindow = *m_window;
	swap_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_desc.BufferDesc = bufferDesc;
	swap_desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swap_desc.SampleDesc.Count = 1;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	// Create Device and Swap Chain
	hr = D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&swap_desc,
		&m_SwapChain,
		&m_Device,
		0,
		&m_DeviceContext
	);
}