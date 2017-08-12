#include "MyRender.h"
#include "User.h"

MyRender::MyRender()
{
	m_mesh = nullptr;
	WireFrame = nullptr;
	Solid = nullptr;
}

bool MyRender::Init(HWND hwnd)
{
	User *user = new User(&m_view);
	Framework::Get()->AddInputListener(user);

	m_mesh = new StaticMesh();
	if (!m_mesh->Init(this, L"mesh.ms3d"))
		return false;

	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	m_pd3dDevice->CreateRasterizerState(&desc, &WireFrame);
	desc.FillMode = D3D11_FILL_SOLID;
	m_pd3dDevice->CreateRasterizerState(&desc, &Solid);

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;
	m_pd3dDevice->CreateBlendState(&blendDesc, &Transparency);

	return true;
}

bool MyRender::Draw()
{

	m_pImmediateContext->OMSetBlendState(0, 0, 0xffffffff);

	m_pImmediateContext->RSSetState(WireFrame); // Display only web
	m_mesh->Identity();
	m_mesh->Translate(-1.5, 0.0, 0.0);
	m_mesh->Draw(m_view);

	// Add transperency
	float blendFactor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_pImmediateContext->OMSetBlendState(Transparency, blendFactor, 0xffffffff);

	m_pImmediateContext->RSSetState(Solid); // Display filled web
	m_mesh->Identity();
	m_mesh->Translate(1.5, 0.0, 0.0);
	m_mesh->Draw(m_view);

	return true;
}

void MyRender::Close()
{
	_CLOSE(m_mesh);
	_RELEASE(WireFrame);
	_RELEASE(Solid);
}
