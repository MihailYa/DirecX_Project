cbuffer cbPerObject : register(b0)
{
	matrix WVP;
	matrix World;
	matrix Inverse;
}

struct AppData
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD;
    float3 Normal   : NORMAL;
};

struct PixelInputType
{
	float4 PositionWS   : TEXCOORD1;
	float3 NormalWS     : TEXCOORD2;
	float2 TexCoord     : TEXCOORD0;
	float4 Position     : SV_Position;
};

PixelInputType VS( AppData IN )
{
	PixelInputType OUT;

    OUT.Position = mul(WVP, float4(IN.Position, 1.0f));
    OUT.PositionWS = mul(World, float4(IN.Position, 1.0f));
	OUT.NormalWS = mul((float3x3)Inverse, IN.Normal);
    OUT.TexCoord = IN.TexCoord;

    return OUT;
}