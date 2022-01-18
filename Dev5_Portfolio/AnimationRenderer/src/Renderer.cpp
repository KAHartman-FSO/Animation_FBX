#include "Renderer.h"

void Renderer::CreateCalls()
{
	CreateDXVars();
	CreateInputLayoutsAndShaders();
	CreateConstantBuffers();
	CreateViewProjectionMatrices();
}
void Renderer::LoopCalls()
{
	m_deltaTime = calc_delta_time();
	float Color[] = { 0.25f, 0.25f, 0.5f, 1 };
	m_DeviceContext->ClearRenderTargetView(m_RTV, Color);

	UpdateCamera();
	SetRenderTargetsAndViewPorts();
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

/// <summary>
///								<<<<<<<<< __Loop Calls__ >>>>>>>>>>>
/// </summary>
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
	else if (_wParam == VK_LCONTROL)
		m_KB_Input.set(KEY::LCTRL, desiredState);
	else if (_wParam == VK_MBUTTON)
		m_KB_Input.set(KEY::MBUTTON, desiredState);
	
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
	// Take in input from Windows, update View Matrix
	// Check state of each key, and update a matrix accordingly

	if (m_KB_Input.any())
	{
		float moveSpeed = 10;
		float appliedSpeed = moveSpeed * m_deltaTime;
		XMMATRIX temp = XMMatrixIdentity();
		float3 translation = { 0, 0, 0 };
		
		// Translation
		if (m_KB_Input.test(KEY::LSHIFT))			// Sprint
			appliedSpeed *= 2.0f;
		if (m_KB_Input.test(KEY::MBUTTON))	// Slow Walk
			appliedSpeed *= 0.5f;

		if (m_KB_Input.test(KEY::W))
			translation.z += appliedSpeed;
		if (m_KB_Input.test(KEY::S))
			translation.z -= appliedSpeed;
		if (m_KB_Input.test(KEY::A))
			translation.x -= appliedSpeed;
		if (m_KB_Input.test(KEY::D))
			translation.x += appliedSpeed;

	
		// Update View Matrix
		temp = XMMatrixTranslation(translation.x, 0, translation.z);

		XMMATRIX camera = XMLoadFloat4x4(&m_Matrices.view);
		camera = XMMatrixMultiply(temp, camera);


		camera = XMMatrixTranspose(camera);
		XMStoreFloat4x4(&m_Matrices.view, camera);
	}
}
void Renderer::SetRenderTargetsAndViewPorts()
{
	ID3D11RenderTargetView* tempRTVs[] = { m_RTV };
	m_DeviceContext->OMSetRenderTargets(1, tempRTVs, nullptr);
	m_DeviceContext->RSSetViewports(1, &m_ViewPort);
}


