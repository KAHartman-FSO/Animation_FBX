#pragma once
#include <bitset>
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <DirectXMath.h>
using namespace DirectX;

#include "tools.h"
#include "LineDebugger.h"
#include "XMPLvShader.csh"
#include "XMPLpShader.csh"
#include <string>
namespace KEY
{
	enum KEYBOARD_KEY { W, A, S, D, DOWN, UP, LEFT, RIGHT, SPACE, LSHIFT };
}
struct WVP
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};
class Renderer
{
public:
	Renderer() 
	{
	}

	// Frees All Memory from Renderer
	~Renderer()
	{
		m_Device->Release();
		m_SwapChain->Release();
		m_DeviceContext->Release();
		m_RTV->Release();
		m_PyramidVertexBuffer->Release();
		m_VertexShader->Release();
		m_PixelShader->Release();
		m_ColorVertexLayout->Release();
		m_LineVertexBuffer->Release();
		m_WVPConstantBuffer->Release();
		m_zBuffer->Release();
		m_zBufferView->Release();
	}
	void RenderLoop()
	{
		LoopCalls();
		DrawCalls();	

		m_SwapChain->Present(0, 0);
	}
	void DXSetUp(HWND _window)
	{
		m_window = _window;
		GetClientRect(m_window, &m_windowRect);
		m_aspectRatio = static_cast<float>((m_windowRect.right - m_windowRect.left) / (m_windowRect.bottom - m_windowRect.top));

		CreateCalls();
		LoadCalls();
	}
	void QueryInput(UINT _message, WPARAM _wParam);

private:
	HRESULT hr;
	HWND m_window;
	RECT m_windowRect;
	float m_aspectRatio = 1;
	float m_deltaTime;
	std::bitset<10> m_KB_Input;

	ID3D11Device* m_Device;
	IDXGISwapChain* m_SwapChain;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D11RenderTargetView* m_RTV;
	D3D11_VIEWPORT m_ViewPort;
	ID3D11Texture2D* m_zBuffer;
	ID3D11DepthStencilView* m_zBufferView;

	ID3D11Buffer* m_PyramidVertexBuffer;
	ID3D11InputLayout* m_ColorVertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11Buffer* m_WVPConstantBuffer;

	WVP m_Matrices;

	LineDebugger m_lineDebugger;
	ID3D11Buffer* m_LineVertexBuffer;

#pragma region Render.cpp
	void CreateCalls();
	void LoopCalls();

	// Before Loop
	void CreateDXVars();
	void CreateInputLayoutsAndShaders();
	void CreateViewProjectionMatrices();
	void CreateConstantBuffers();
	void CreateZBufferAndView();

	// In Loop
	void SetRenderVars();
	void UpdateCamera();
	void UpdateInput(WPARAM, bool);

	// Renderer_LOAD
	void LoadCalls();
	void Load_Pyramid();
	void Load_Lines();

	// Renderer_DRAW
	void DrawCalls();
	void Draw_Pyramid();
	void Draw_Lines();

#pragma region Extra Helper Functions
	float calc_delta_time()
	{
		static std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

		std::chrono::time_point<std::chrono::high_resolution_clock> new_time = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> elapsed_seconds = new_time - last_time;
		last_time = new_time;

		return elapsed_seconds.count();
	}
#pragma endregion
};