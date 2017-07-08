#include "MyRender.h"

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightColor[2];
	XMFLOAT4 vOutputColor;
};

MyRender::MyRender()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pPixelShaderSolid = nullptr;
	m_pVertexLayout = nullptr;
	m_pVertexBuffer = nullptr;

	m_pIndexBuffer = nullptr;
	m_pConstantBuffer = nullptr;
	/*m_pTextureRV = nullptr;
	m_pSamplerLinear = nullptr;*/

}

bool MyRender::Init(HWND hwnd)
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = NULL;
	hr = m_compileshaderfromfile(L"shader.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		Log::Get()->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
	if (FAILED(hr))
	{
		_RELEASE(pVSBlob);
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = m_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
	_RELEASE(pVSBlob);
	if (FAILED(hr))
		return false;

	m_pImmediateContext->IASetInputLayout(m_pVertexLayout);

	ID3DBlob* pPSBlob = NULL;
	hr = m_compileshaderfromfile(L"shader.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		Log::Get()->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader);
	_RELEASE(pPSBlob);
	if (FAILED(hr))
		return false;

	pPSBlob = NULL;
	hr = m_compileshaderfromfile(L"shader.fx", "PSSolid", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
		return false;

	hr = m_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShaderSolid);
	_RELEASE(pPSBlob);
	if (FAILED(hr))
		return false;

	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	if (FAILED(hr))
		return false;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = m_pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return false;

	m_pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = m_pd3dDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
		return false;

	m_World1 = XMMatrixIdentity();
	m_World2 = XMMatrixIdentity();

	User *user = new User(&m_View, -10, 5, -10);
	Framework::Get()->AddInputListener(user);

	/*XMVECTOR Eye = XMVectorSet(10.0f, 10.0f, 10.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = XMMatrixLookAtLH(Eye, At, Up);*/

	float width = Window::Get()->GetWidth();
	float height = Window::Get()->GetHeight();
	m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / height, 0.01f, 100.0f);

	return true;
}

bool MyRender::Draw()
{
	Update();
	XMFLOAT4 vLightDirs[2] =
	{
		XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
		XMFLOAT4(1.0f, 0.0f, -1.0f, 1.0f),
	};
	XMFLOAT4 vLightColors[2] =
	{
		XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
	};

	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(m_World1);
	cb1.mView = XMMatrixTranspose(m_View);
	cb1.mProjection = XMMatrixTranspose(m_Projection);
	cb1.vLightDir[0] = vLightDirs[0];
	cb1.vLightDir[1] = vLightDirs[1];
	cb1.vLightColor[0] = vLightColors[0];
	cb1.vLightColor[1] = vLightColors[1];
	cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb1, 0, 0);

	//m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	//m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	//m_pImmediateContext->DrawIndexed(36, 0, 0);

	m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	ConstantBuffer cb2;
	cb2.mWorld = XMMatrixTranspose(m_World2);
	cb2.mView = XMMatrixTranspose(m_View);
	cb2.mProjection = XMMatrixTranspose(m_Projection);
	cb2.vLightDir[0] = vLightDirs[0];
	cb2.vLightDir[1] = vLightDirs[1];
	cb2.vLightColor[0] = vLightColors[0];
	cb2.vLightColor[1] = vLightColors[1];
	cb2.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb2, 0, 0);

	m_pImmediateContext->DrawIndexed(36, 0, 0);

	cb2.mWorld = XMMatrixTranspose(XMMatrixScaling(1.0f, 10.0f, 1.0f));
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb2, 0, 0);
	//m_pImmediateContext->DrawIndexed(36, 0, 0);

	for (int m = 0; m < 2; m++)
	{
		XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&vLightDirs[m]));
		XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
		mLight = mLightScale * mLight;

		cb1.mWorld = XMMatrixTranspose(mLight);
		cb1.vOutputColor = vLightColors[m];
		m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb1, 0, 0);

		m_pImmediateContext->PSSetShader(m_pPixelShaderSolid, NULL, 0);
		m_pImmediateContext->DrawIndexed(36, 0, 0);
	}

	//float m_Yaw = 0;
	//float m_Pitch = 45;
	//XMMATRIX Rotation = XMMatrixRotationY(m_Yaw / 180 * XM_PI);
	//XMMATRIX Rotation_Right_Vector = XMMatrixRotationY(m_Yaw / 180 * XM_PI + XM_PIDIV2);
	//XMVECTOR Right_Vector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	//Right_Vector = XMVector3Transform(Right_Vector, Rotation_Right_Vector);
	//
	//Rotation *= XMMatrixRotationAxis(Right_Vector, m_Pitch / 180 * XM_PI);
	////Log::Get()->Debug("Right vector: %f %f %f\n", XMVectorGetByIndex(Right_Vector, 0), XMVectorGetByIndex(Right_Vector, 1), XMVectorGetByIndex(Right_Vector, 2));
	//cb1.mWorld = XMMatrixTranspose(Rotation);
	//cb1.vOutputColor = XMFLOAT4(0, 1, 0, 1);
	//m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb1, 0, 0);
	//m_pImmediateContext->DrawIndexed(36, 0, 0);



	// oY
	cb1.mWorld = XMMatrixTranspose(XMMatrixScaling(0.08f, 10.0f, 0.08f));
	cb1.vOutputColor = XMFLOAT4(0, 1, 0, 1);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb1, 0, 0);
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	// oX
	cb1.mWorld = XMMatrixTranspose(XMMatrixScaling(10.0f, 0.08f, 0.08f));
	cb1.vOutputColor = XMFLOAT4(1, 1, 0, 1);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb1, 0, 0);
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	// oZ
	cb1.mWorld = XMMatrixTranspose(XMMatrixScaling(0.08f, 0.08f, 10.0f));
	cb1.vOutputColor = XMFLOAT4(0, 0, 1, 1);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb1, 0, 0);
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	//Log::Get()->Debug("Target__: %f %f %f\n", XMVectorGetByIndex(User::Get()->GetTarget(), 0), XMVectorGetByIndex(User::Get()->GetTarget(), 1), XMVectorGetByIndex(User::Get()->GetTarget(), 2));
	/*XMMATRIX mWorld3 = XMMatrixTranslation(XMVectorGetByIndex(User::Get()->GetTarget(), 0), XMVectorGetByIndex(User::Get()->GetTarget(), 1), XMVectorGetByIndex(User::Get()->GetTarget(), 2));
	cb1.mWorld = XMMatrixTranspose(XMMatrixScaling(0.1f, 0.1f, 0.1f)*mWorld3);
	cb1.vOutputColor = XMFLOAT4(1, 1, 1, 1);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, NULL, &cb1, 0, 0);

	m_pImmediateContext->PSSetShader(m_pPixelShaderSolid, NULL, 0);
	m_pImmediateContext->DrawIndexed(36, 0, 0);*/

	return true;
}

void MyRender::Update()
{
	static float t = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	t = (dwTimeCur - dwTimeStart) / 1000.0f;

	m_World1 = XMMatrixRotationY(t);

	XMMATRIX mScale = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	XMMATRIX mSpin = XMMatrixRotationY(-t*3.0);
	XMMATRIX mTranslate = XMMatrixTranslation(-10.0f, 0.0f, 10.0f);
	//XMMATRIX mOrbit = XMMatrixRotationY(-t * 2.0f);

	m_World2 =  mScale*mSpin*mTranslate;
}

void MyRender::Close()
{
	_RELEASE(m_pConstantBuffer);
	_RELEASE(m_pVertexBuffer);
	_RELEASE(m_pIndexBuffer);
	_RELEASE(m_pVertexLayout);
	_RELEASE(m_pVertexShader);
	_RELEASE(m_pPixelShader);
	_RELEASE(m_pPixelShaderSolid);
}
