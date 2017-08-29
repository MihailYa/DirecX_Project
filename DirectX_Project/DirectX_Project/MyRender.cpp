#include "MyRender.h"

#include "User.h"
#include "Shader.h"
#include "Buffer.h"



struct cbMatrixData
{
	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX Inverse;
};

MyRender::MyRender()
{
	//m_mesh = nullptr;
	IndexBuffer = nullptr;
	VertBuffer = nullptr;
	constMatrixBuffer = nullptr;
	constLightBuffer = nullptr;
	shader = nullptr;
}

bool MyRender::Init()
{
	User *user = new User(&m_view);
	Framework::Get()->AddInputListener(user);

	shader = new Shader(this);
	if (!shader)
		return false;


	if (!shader->LoadTexture(L"Texture.png"))
		return false;

	shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	shader->AddInputElementDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);

	if (!shader->CreateShader(L"SimpleVertexShader.hlsl", L"TexturedLitPixelShader.hlsl"))
		return false;

	

	

	Vertex v[] =
	{
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),

		Vertex(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f),
		Vertex(1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 1.0f),
		Vertex(1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f),
		Vertex(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f,-1.0f,  1.0f, 1.0f),

		Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f,-1.0f, 1.0f, -1.0f),
		Vertex(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f,-1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f),
		Vertex(1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f),

		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, -1.0f,  1.0f),
		Vertex(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f,-1.0f, -1.0f,  1.0f),

		Vertex(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f,-1.0f, -1.0f,  1.0f),
		Vertex(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f,-1.0f,  1.0f,  1.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f,-1.0f,  1.0f, -1.0f),
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, -1.0f),

		Vertex(1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f),
		Vertex(1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, -1.0f),
		Vertex(1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f),
		Vertex(1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, -1.0f,  1.0f),
	};

	DWORD indices[] =
	{
		0,  1,  2,
		0,  2,  3,

		4,  5,  6,
		4,  6,  7,

		8,  9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23
	};

	IndexBuffer = Buffer::CreateIndexBuffer(m_pd3dDevice, sizeof(DWORD) * 36, false, indices);

	VertBuffer = Buffer::CreateVertexBuffer(m_pd3dDevice, sizeof(Vertex) * 24, false, v);

	constMatrixBuffer = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(cbMatrixData), false);
	//constLightBuffer = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(cbLightData), false);

	m_material = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(MaterialProperties), false);

	constLightBuffer = Buffer::CreateConstantBuffer(m_pd3dDevice, sizeof(LightProperties), false);

	light_prop = new LightProperties;
	light_prop->Lights[0].Enabled = true;
	light_prop->Lights[0].LightType = 1;
	light_prop->Lights[0].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	light_prop->Lights[0].SpotAngle = XMConvertToRadians(45.0f);
	light_prop->Lights[0].ConstantAttenuation = 1.0f;
	light_prop->Lights[0].LinearAttenuation = 0.08f;
	light_prop->Lights[0].QuadraticAttenuation = 0.0f;
	XMFLOAT4 LightPosition = XMFLOAT4(0.0f, 5.0f, 0.0f, 0.0f);
	light_prop->Lights[0].Position = LightPosition;
	XMVECTOR LightDirection = XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f);
	LightDirection = XMVector3Normalize(LightDirection);
	XMStoreFloat4(&light_prop->Lights[0].Direction, LightDirection);

	light_prop->GlobalAmbient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	return true;
}

bool MyRender::Draw()
{
	//m_mesh->Draw(m_view);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &VertBuffer, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	light_prop->EyePosition = User::Get()->GetPos();
	m_pImmediateContext->UpdateSubresource(constLightBuffer, 0, NULL, light_prop, 0, 0);
	//m_pImmediateContext->PSSetConstantBuffers(0, 1, &constLightBuffer);

	/*             Пол из 4 кубов:
	cube1World   cube4World   cube7World
	cube2World   cube5World   cube8World
	cube3World   cube6World   cube9World

	Поменяйте параматры XMMatrixTranslation, посмотрите результат.
	*/
	MaterialProperties pearlMaterial;
	pearlMaterial.Material.Ambient = XMFLOAT4(0.25f, 0.20725f, 0.20725f, 1.0f);
	pearlMaterial.Material.Diffuse = XMFLOAT4(1.0f, 0.829f, 0.829f, 1.0f);
	pearlMaterial.Material.Specular = XMFLOAT4(0.296648f, 0.296648f, 0.296648f, 1.0f);
	pearlMaterial.Material.SpecularPower = 11.264f;
	pearlMaterial.Material.UseTexture = true;

	/*pearlMaterial.Material.Ambient = XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	pearlMaterial.Material.Diffuse = XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	pearlMaterial.Material.Specular = XMFLOAT4(0.07568f, 0.61424f, 0.07568f, 1.0f);
	pearlMaterial.Material.SpecularPower = 76.8f;*/

	m_pImmediateContext->UpdateSubresource(m_material, 0, NULL, &pearlMaterial, 0, 0);
	ID3D11Buffer* pixelShaderConstantBuffers[2] = { m_material, constLightBuffer };
	m_pImmediateContext->PSSetConstantBuffers(0, 2, pixelShaderConstantBuffers);

	XMVECTOR tmp;

	XMMATRIX cube1World = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	XMMATRIX WVP = cube1World * m_view * m_Projection;
	cbMatrixData cbMat;
	cbMat.World = cube1World;
	cbMat.WVP = WVP;
	cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube1World));
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube2World = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);
	WVP = cube2World * m_view * m_Projection;
	cbMat.World = cube2World;
	cbMat.WVP = WVP;
	cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube2World));
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube3World = XMMatrixTranslation(-2.0f, 0.0f, -2.0f);
	WVP = cube3World * m_view * m_Projection;
	cbMat.World = cube3World;
	cbMat.WVP = WVP;
	cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube3World));
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube4World = XMMatrixTranslation(0.0f, 0.0f, 2.0f);
	WVP = cube4World * m_view * m_Projection;
	cbMat.World = cube4World;
	cbMat.WVP = WVP;
	cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube4World));
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube5World = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	WVP = cube5World * m_view * m_Projection;
	cbMat.World = cube5World;
	cbMat.WVP = WVP;
	cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube5World));
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube6World = XMMatrixTranslation(0.0f, 0.0f, -2.0f);
	WVP = cube6World * m_view * m_Projection;
	cbMat.World = cube6World;
	cbMat.WVP = WVP;
	cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube6World));
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube7World = XMMatrixTranslation(2.0f, 0.0f, 2.0f);
	WVP = cube7World * m_view * m_Projection;
	cbMat.World = cube7World;
	cbMat.WVP = WVP;
	cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube7World));
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube8World = XMMatrixTranslation(2.0f, 0.0f, 0.0f);
	WVP = cube8World * m_view * m_Projection;
	cbMat.World = cube8World;
	cbMat.WVP = WVP;
	cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube8World));
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	XMMATRIX cube9World = XMMatrixTranslation(2.0f, 0.0f, -2.0f);
	WVP = cube9World * m_view * m_Projection;
	cbMat.World = cube9World;
	cbMat.WVP = WVP;
	cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube9World));
	m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	shader->Draw();
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	//XMMATRIX cube10World = XMMatrixTranslation(0.0f, 5.0f, 0.0f);//*XMMatrixScaling(0.2f, 0.2f, 0.2f);
	//WVP = cube10World * m_view * m_Projection;
	//cbMat.World = cube10World;
	//cbMat.WVP = WVP;
	//cbMat.Inverse = XMMatrixTranspose(XMMatrixInverse(&tmp, cube10World));
	//m_pImmediateContext->UpdateSubresource(constMatrixBuffer, 0, NULL, &cbMat, 0, 0);
	//m_pImmediateContext->VSSetConstantBuffers(0, 1, &constMatrixBuffer);
	//shader->Draw();
	//m_pImmediateContext->DrawIndexed(36, 0, 0);

	return true;
}

void MyRender::Close()
{
	_CLOSE(shader);
	_RELEASE(IndexBuffer);
	_RELEASE(VertBuffer);
	_RELEASE(constMatrixBuffer);
	_RELEASE(constLightBuffer);
	//_CLOSE(m_mesh);
}
