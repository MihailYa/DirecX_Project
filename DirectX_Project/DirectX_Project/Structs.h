#pragma once
#define MAX_LIGHTS 8

struct _Material
{
	_Material()
		: Emissive(0.0f, 0.0f, 0.0f, 1.0f)
		, Ambient(0.1f, 0.1f, 0.1f, 1.0f)
		, Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		, Specular(1.0f, 1.0f, 1.0f, 1.0f)
		, SpecularPower(128.0f)
		, UseTexture(true)
	{}

	XMFLOAT4   Emissive;
	//----------------------------------- (16 byte boundary)
	XMFLOAT4   Ambient;
	//----------------------------------- (16 byte boundary)
	XMFLOAT4   Diffuse;
	//----------------------------------- (16 byte boundary)
	XMFLOAT4   Specular;
	//----------------------------------- (16 byte boundary)
	float               SpecularPower;
	// Add some padding complete the 16 byte boundary.
	bool                 UseTexture;
	// Add some padding to complete the 16 byte boundary.
	float                 Padding[2];
	//----------------------------------- (16 byte boundary)
}; // Total:                                80 bytes (5 * 16)

struct MaterialProperties
{
	_Material   Material;
};

enum LightType
{
	DirectionalLight = 0,
	PointLight = 1,
	SpotLight = 2
};

struct Light
{
	Light()
		: Position(0.0f, 0.0f, 0.0f, 1.0f)
		, Direction(0.0f, 0.0f, 1.0f, 0.0f)
		, Color(1.0f, 1.0f, 1.0f, 1.0f)
		, SpotAngle(XM_PIDIV2)
		, ConstantAttenuation(1.0f)
		, LinearAttenuation(0.0f)
		, QuadraticAttenuation(0.0f)
		, LightType(DirectionalLight)
		, Enabled(0)
	{}

	XMFLOAT4    Position;
	//----------------------------------- (16 byte boundary)
	XMFLOAT4    Direction;
	//----------------------------------- (16 byte boundary)
	XMFLOAT4    Color;
	//----------------------------------- (16 byte boundary)
	float       SpotAngle;
	float       ConstantAttenuation;
	float       LinearAttenuation;
	float       QuadraticAttenuation;
	//----------------------------------- (16 byte boundary)
	int         LightType;
	int         Enabled;
	// Add some padding to make this struct size a multiple of 16 bytes.
	int         Padding[2];
	//----------------------------------- (16 byte boundary)
};  // Total:                              80 bytes ( 5 * 16 )

struct LightProperties
{
	LightProperties()
		: EyePosition(5.0f, 5.0f, 5.0f, 1.0f)
		, GlobalAmbient(0.2f, 0.2f, 0.8f, 1.0f)
	{}
	XMFLOAT4   EyePosition;
	//----------------------------------- (16 byte boundary)
	XMFLOAT4   GlobalAmbient;
	//----------------------------------- (16 byte boundary)
	Light               Lights[MAX_LIGHTS]; // 80 * 8 bytes
};  // Total:                                  672 bytes (42 * 16)

	// Per-vertex data.
struct Vertex
{
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz) :
		Position(x, y, z), Tex0(u, v), Normal(nx, ny, nz) {}
	XMFLOAT3 Position;
	XMFLOAT2 Tex0;
	XMFLOAT3 Normal;
};
// Per-instance data (must be 16 byte aligned)
__declspec(align(16)) struct PlaneInstanceData
{
	XMMATRIX WorldMatrix;
	XMMATRIX InverseTransposeWorldMatrix;
};
