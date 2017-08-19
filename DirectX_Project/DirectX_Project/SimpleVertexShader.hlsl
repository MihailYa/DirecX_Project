cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
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
    float4 Position     : SV_POSITION;
};

PixelInputType VS( AppData IN )
{
	PixelInputType OUT;

    OUT.Position = mul(float4(IN.Position, 1.0f), WVP );
    OUT.PositionWS = mul(float4(IN.Position, 1.0f), World );
    OUT.NormalWS = mul( IN.Normal, World );
    OUT.TexCoord = IN.TexCoord;

    return OUT;
}