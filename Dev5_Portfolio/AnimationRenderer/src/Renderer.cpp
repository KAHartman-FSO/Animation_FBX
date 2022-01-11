#include "Renderer.h"
#define PYRAMID_VERTS 12

Renderer::Renderer()
{}
////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Creates Device, SwapChain, DeviceContext, Render Target View, and Viewport
///
////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::CreateDXVars() {
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
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
		featureLevels, 1, D3D11_SDK_VERSION, &swap_desc, &m_SwapChain, &m_Device, 0, &m_DeviceContext);
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
}
void Renderer::Prep_Pyramid() {
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
		{{-0.25f, 0.25, 0.25f, 1.0f}, {0.0f, 0.0f,1.0f, 1.0f}},
		{{0.25f, 0.25, 0.25f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}
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

	hr = m_Device->CreateBuffer(&vbDesc, &subData, &m_VertexBuffer);

	hr = m_Device->CreateVertexShader(XMPLvShader, sizeof(XMPLvShader), nullptr, &m_VertexShader);
	hr = m_Device->CreatePixelShader(XMPLpShader, sizeof(XMPLpShader), nullptr, &m_PixelShader);

	D3D11_INPUT_ELEMENT_DESC inputDesc[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	hr = m_Device->CreateInputLayout(inputDesc, 2, XMPLvShader, sizeof(XMPLvShader), &m_VertexLayout);

	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.ByteWidth = sizeof(WVP);
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	hr = m_Device->CreateBuffer(&vbDesc, &subData, &m_ConstantBuffer);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Function Call to Set Up / Send Everything we need to the GPU before our Game Loop
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Renderer::DXSetUp(HWND _window)
{
	m_window = _window;
	GetClientRect(m_window, &m_windowRect);

	CreateDXVars();
	Prep_Pyramid();
}
void Renderer::CreateViewProjectionMatrices()
{
	// View Matrix ( Camera Matrix )
	XMMATRIX temp;
	temp = XMMatrixLookAtLH({ 2, 1, -3 }, { 0, 0, 0 }, { 0, 1, 0 });
	XMStoreFloat4x4(&m_Matrices.view, temp);

	// Projection
	temp = XMMatrixPerspectiveFovLH(3.14f / 2.0f, m_aspectRatio, 0.1f, 1000);
	XMStoreFloat4x4(&m_Matrices.projection, temp);
}
void Renderer::Draw_Pyramid()
{
	m_aspectRatio = static_cast<float>((m_windowRect.right - m_windowRect.left) / (m_windowRect.bottom - m_windowRect.top));

	// World Matrix Set
	XMMATRIX temp = XMMatrixIdentity();
	temp = XMMatrixTranslation(0, 0, 3);
	temp = XMMatrixTranspose(temp);
	//XMMATRIX temp2 = XMMatrixRotationY()
	XMStoreFloat4x4(&m_Matrices.world, temp);

	// Sending Data to GPU
	D3D11_MAPPED_SUBRESOURCE gpuBuffer;
	hr = m_DeviceContext->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &gpuBuffer);
	memcpy(gpuBuffer.pData, &m_Matrices, sizeof(WVP));
	m_DeviceContext->Unmap(m_ConstantBuffer, 0);

	ID3D11Buffer* constants[] = { m_ConstantBuffer };
	m_DeviceContext->VSSetConstantBuffers(0, 1, constants);


	UINT strides[] = { sizeof(tools::ColorVertex) };
	UINT offsets[] = { 0 };
	ID3D11Buffer* tempVB[] = { m_VertexBuffer };
	m_DeviceContext->IASetVertexBuffers(0, 1, tempVB, strides, offsets);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->VSSetShader(m_VertexShader, 0, 0);
	m_DeviceContext->PSSetShader(m_PixelShader, 0, 0);

	m_DeviceContext->Draw(PYRAMID_VERTS, 0);
}
void Renderer::Render()
{
	float Color[] = { 0.25, 0.25, 0.25, 1 };
	m_DeviceContext->ClearRenderTargetView(m_RTV, Color);

	ID3D11RenderTargetView* tempRTVs[] = { m_RTV };
	m_DeviceContext->OMSetRenderTargets(1, tempRTVs, nullptr);
	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
	m_DeviceContext->IASetInputLayout(m_VertexLayout);
	CreateViewProjectionMatrices();
	Draw_Pyramid();
	m_SwapChain->Present(0, 0);
}