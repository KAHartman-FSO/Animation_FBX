#include "Renderer.h"

float Renderer::calc_delta_time()
{
	static std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

	std::chrono::time_point<std::chrono::high_resolution_clock> new_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed_seconds = new_time - last_time;
	last_time = new_time;

	return elapsed_seconds.count();
}

void Renderer::DXSetUp(HWND _window)
{
	m_window = _window;
	GetClientRect(m_window, &m_windowRect);
	m_aspectRatio = static_cast<float>((m_windowRect.right - m_windowRect.left) / (m_windowRect.bottom - m_windowRect.top));

	CreateDXVars();
	CreateInputLayoutsAndShaders();
	Load_Pyramid();
	Load_Lines();
}

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
void Renderer::Load_Lines()
{
	tools::ColorVertex gridCenterVertex = { {0.0f, 0.0f, 0.0f, 1.0f}, {0.65f, 0.0f, 0.65f, 1.0f} };
	m_lineDebugger.CreateGrid(64, 8, 0.5f, gridCenterVertex);

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

void Renderer::Load_Pyramid() {
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

void Renderer::CreateInputLayoutsAndShaders()
{
	// Create Shaders
	hr = m_Device->CreateVertexShader(XMPLvShader, sizeof(XMPLvShader), nullptr, &m_VertexShader);
	hr = m_Device->CreatePixelShader(XMPLpShader, sizeof(XMPLpShader), nullptr, &m_PixelShader);

	// Create InputLayout
	D3D11_INPUT_ELEMENT_DESC inputDesc[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	hr = m_Device->CreateInputLayout(inputDesc, 2, XMPLvShader, sizeof(XMPLvShader), &m_ColorVertexLayout);

	// Allocate Space for World View and Projection Matrix
	D3D11_BUFFER_DESC vbDesc;
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&vbDesc, sizeof(D3D11_BUFFER_DESC));
	vbDesc.ByteWidth = sizeof(WVP);
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;
	subData.pSysMem = &m_Matrices;
	hr = m_Device->CreateBuffer(&vbDesc, &subData, &m_WVPConstantBuffer);
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
void Renderer::Render()
{
	float Color[] = { 1.0f, 1.0f, 1.0f, 1 };
	m_DeviceContext->ClearRenderTargetView(m_RTV, Color);
	m_deltaTime = calc_delta_time();

	ID3D11RenderTargetView* tempRTVs[] = { m_RTV };
	m_DeviceContext->OMSetRenderTargets(1, tempRTVs, nullptr);
	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
	m_DeviceContext->IASetInputLayout(m_ColorVertexLayout);
	CreateViewProjectionMatrices();

	Draw_Lines();
	Draw_Pyramid();
	m_SwapChain->Present(0, 0);
}

