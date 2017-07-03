/*
//--------------------------------------------------------------------------------------
// Вершинный шейдер
//--------------------------------------------------------------------------------------
float4 VS( float4 Pos : POSITION ) : SV_POSITION
{
    return Pos;
}

//--------------------------------------------------------------------------------------
// Пикслельный шейдер
//--------------------------------------------------------------------------------------
float4 PS( float4 Pos : SV_POSITION ) : SV_Target
{
    float fLimiter = 500.0f;
    float dist = Pos.x*Pos.x + Pos.y*Pos.y;
    dist = (dist % fLimiter) / fLimiter;
    return float4( dist, 0.0f, dist, 1.0f );
}
*/

cbuffer ConstantBuffer
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};


VS_OUTPUT VS( float4 Pos : POSITION, float4 Color : COLOR )
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul( Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Color = Color;
    return output;
}



float4 PS( VS_OUTPUT input ) : SV_Target
{
    return input.Color;
}