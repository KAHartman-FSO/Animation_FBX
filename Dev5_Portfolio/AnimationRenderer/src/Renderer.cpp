#include "Renderer.h"

Renderer::Renderer()
{}
void Renderer::DXSetUp(HWND _window)
{
	m_window = _window;
	GetClientRect(m_window, &m_windowRect);

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
	swap_desc.OutputWindow = m_window;
	swap_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_desc.BufferDesc = bufferDesc;
	swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_desc.SampleDesc.Count = 1;
	swap_desc.Windowed = true;

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
	// Create Render Target View
	ID3D11Resource* backBuffer;
	hr = m_SwapChain->GetBuffer(0, __uuidof(backBuffer), (void**)&backBuffer);
	if (backBuffer != NULL)
		hr = m_Device->CreateRenderTargetView(backBuffer, NULL, &m_RTV);
	// Create ViewPort
	m_ViewPort.Width = static_cast<float>(m_windowRect.right - m_windowRect.left);
	m_ViewPort.Height = static_cast<float>(m_windowRect.bottom - m_windowRect.top);
	m_ViewPort.TopLeftX = m_ViewPort.TopLeftY = 0;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;

	// Drawing a Triangle
	tools::ColorVertex triangle[]
	{
		{{0.0f, 0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{0.5f, -0.5, 0.0f, 1.0f}, {0.0f, 0.0f,1.0f, 1.0f}},
		{{-0.5f, -0.5, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}
	};
	D3D11_BUFFER_DESC vbDesc;
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	vbDesc.ByteWidth = sizeof(triangle);
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = NULL;
	vbDesc.MiscFlags = NULL;
	vbDesc.StructureByteStride = 0;

	subData.pSysMem = triangle;

	hr = m_Device->CreateBuffer(&vbDesc, &subData, &vertexBuffer);

	hr = m_Device->CreateVertexShader(XMPLvShader, sizeof(XMPLvShader), nullptr, &vertexShader);
	hr = m_Device->CreatePixelShader(XMPLpShader, sizeof(XMPLpShader), nullptr, &pixelShader);
}
void Renderer::Render()
{
	ID3D11RenderTargetView* tempRTVs[] = { m_RTV };
	m_DeviceContext->OMSetRenderTargets(1, tempRTVs, nullptr);

	float Color[] = { 0.25, 0.75, 0, 1 };
	m_DeviceContext->ClearRenderTargetView(m_RTV, Color);

	m_SwapChain->Present(1, 0);
}