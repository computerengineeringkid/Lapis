cbuffer MatrixBuffer
{
    matrix view;
    matrix projection;
    matrix world;
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    float4 worldPosition = mul(float4(input.pos, 1.0f), world); // Apply world matrix
    worldPosition = mul(worldPosition, view); // Apply view matrix
    output.pos = mul(worldPosition, projection); // Apply projection matrix
    output.color = input.color;
    return output;
}
