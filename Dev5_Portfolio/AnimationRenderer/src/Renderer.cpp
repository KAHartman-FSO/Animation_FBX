#include "Renderer.h"
#pragma region Basic
// CREATION
void Renderer::CreateCalls()
{
	CreateDXVars();
	CreateInputLayoutsAndShaders();
	CreateConstantBuffers();
	CreateViewProjectionMatrices();
	CreateZBufferAndView();
}
void Renderer::CreateDXVars() {
	// Swap Chain Descriptor
	DXGI_SWAP_CHAIN_DESC swap_desc;
	ZeroMemory(&swap_desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swap_desc.BufferCount = 1;
	swap_desc.OutputWindow = m_window;
	swap_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_desc.BufferDesc.Width = m_windowRect.right - m_windowRect.left;
	swap_desc.BufferDesc.Height = m_windowRect.bottom - m_windowRect.top;
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
}
void Renderer::CreateConstantBuffers()
{
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
void Renderer::CreateZBufferAndView()
{
	D3D11_TEXTURE2D_DESC zDesc;
	ZeroMemory(&zDesc, sizeof(D3D11_TEXTURE2D_DESC));
	zDesc.ArraySize = 1;
	zDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	zDesc.Width = m_windowRect.right - m_windowRect.left;
	zDesc.Height = m_windowRect.bottom - m_windowRect.top;
	zDesc.Format = DXGI_FORMAT_D32_FLOAT;
	zDesc.Usage = D3D11_USAGE_DEFAULT;
	zDesc.MipLevels = 1;
	zDesc.SampleDesc.Count = 1;

	hr = m_Device->CreateTexture2D(&zDesc, nullptr, &m_zBuffer);

	if (m_zBuffer != 0)
		hr = m_Device->CreateDepthStencilView(m_zBuffer, nullptr, &m_zBufferView);
}
void Renderer::CreateViewProjectionMatrices()
{
	// View Matrix ( Camera Matrix )
	XMMATRIX temp;
	temp = XMMatrixLookAtLH({ 1, 2, -2 }, { 0, 0, 0 }, { 0, 1, 0 });
	XMStoreFloat4x4(&m_Matrices.view, temp);

	// Projection
	temp = XMMatrixPerspectiveFovLH(3.14f / 2.0f, m_aspectRatio, 0.1f, 1000);
	XMStoreFloat4x4(&m_Matrices.projection, temp);
}

// UPDATE / REITERATION
void Renderer::LoopCalls()
{
	m_deltaTime = calc_delta_time();
	float Color[] = { 0.25f, 0.25f, 0.5f, 1 };
	m_DeviceContext->ClearRenderTargetView(m_RTV, Color);
	m_DeviceContext->ClearDepthStencilView(m_zBufferView, D3D11_CLEAR_DEPTH, 1.0f, NULL);

	UpdateCamera();
	SetRenderVars();
}
void Renderer::SetRenderVars()
{
	ID3D11RenderTargetView* tempRTVs[] = { m_RTV };
	m_DeviceContext->OMSetRenderTargets(1, tempRTVs, m_zBufferView);
	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
}
void Renderer::UpdateInput(WPARAM _wParam, bool desiredState)
{
	if (_wParam == 'W')
		m_KB_Input.set(KEY::W, desiredState);
	else if (_wParam == 'A')
		m_KB_Input.set(KEY::A, desiredState);
	else if (_wParam == 'S')
		m_KB_Input.set(KEY::S, desiredState);
	else if (_wParam == 'D')
		m_KB_Input.set(KEY::D, desiredState);
	else if (_wParam == VK_SPACE)
		m_KB_Input.set(KEY::SPACE, desiredState);
	else if (_wParam == 16)
		m_KB_Input.set(KEY::LSHIFT, desiredState);
	else if (_wParam == VK_UP)
		m_KB_Input.set(KEY::UP, desiredState);
	else if (_wParam == VK_DOWN)
		m_KB_Input.set(KEY::DOWN, desiredState);
	else if (_wParam == VK_LEFT)
		m_KB_Input.set(KEY::LEFT, desiredState);
	else if (_wParam == VK_RIGHT)
		m_KB_Input.set(KEY::RIGHT, desiredState);
}
void Renderer::QueryInput(UINT _message, WPARAM _wParam)
{
	switch (_message)
	{
	case WM_KEYDOWN:
		UpdateInput(_wParam, true);
		break;
	case WM_KEYUP:
		UpdateInput(_wParam, false);
		break;
	case WM_MOUSEMOVE:
		break;
	}
}
void Renderer::UpdateCamera()
{
	if (m_KB_Input.any())
	{
		const float moveSpeed = 10;
		const float rotationSpeed = 5;

		const float xClamp = 0.8f;

		float appliedMovement = moveSpeed * m_deltaTime;
		float appliedRotation = rotationSpeed * m_deltaTime;

		XMMATRIX camera = XMLoadFloat4x4(&m_Matrices.view);
		XMVECTOR determinant = XMMatrixDeterminant(camera);
		camera = XMMatrixInverse(&determinant, camera);

		XMMATRIX local_temp = XMMatrixIdentity();
		XMMATRIX global_temp = XMMatrixIdentity();
		float3 translation = { 0, 0, 0 };
		float3 rotation = { 0, 0, 0 };

		// Translation
		if (m_KB_Input.test(KEY::W))
			translation.z += appliedMovement;
		if (m_KB_Input.test(KEY::S))
			translation.z -= appliedMovement;
		if (m_KB_Input.test(KEY::A))
			translation.x -= appliedMovement;
		if (m_KB_Input.test(KEY::D))
			translation.x += appliedMovement;

		if (m_KB_Input.test(KEY::SPACE))
			translation.y += appliedMovement;
		if (m_KB_Input.test(KEY::LSHIFT))
			translation.y -= appliedMovement;

		// Rotation
		if (m_KB_Input.test(KEY::UP) && camera.r[2].m128_f32[1] < xClamp)
			rotation.x -= appliedRotation;
		if (m_KB_Input.test(KEY::DOWN) && camera.r[2].m128_f32[1] > -xClamp)
			rotation.x += appliedRotation;
		if (m_KB_Input.test(KEY::RIGHT))
			rotation.y += appliedRotation;
		if (m_KB_Input.test(KEY::LEFT))
			rotation.y -= appliedRotation;

		// Update View Matrix Position
		local_temp = XMMatrixTranslation(translation.x, 0, translation.z);
		global_temp = XMMatrixTranslation(0, translation.y, 0);
		camera = XMMatrixMultiply(local_temp, camera);
		camera = XMMatrixMultiply(camera, global_temp);

		// Update View Matrix Rotation
		XMVECTOR position = camera.r[3];
		camera = XMMatrixMultiply(XMMatrixRotationX(rotation.x), camera);
		camera = XMMatrixMultiply(camera, XMMatrixRotationY(rotation.y));
		camera.r[3] = position;

		// Inverse Matrix
		determinant = XMMatrixDeterminant(camera);
		camera = XMMatrixInverse(&determinant, camera);

		XMStoreFloat4x4(&m_Matrices.view, camera);
	}
}
#pragma endregion

#pragma region Draw
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
#pragma endregion

#pragma region Load
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
#pragma endregion
