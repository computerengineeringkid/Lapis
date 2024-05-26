cbuffer MatrixBuffer
{
    matrix view;
    matrix projection;
}

struct VS_INPUT
{
    
    float3 pos : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    matrix instanceWorld : INSTANCEWORLD; // Added per-instance world matrix
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    float4 worldPosition = mul(float4(input.pos, 1.0f), input.instanceWorld); // Apply instance-specific world matrix
    worldPosition = mul(worldPosition, view); // Apply view matrix
    output.pos = mul(worldPosition, projection); // Apply projection matrix
    output.color = input.color;
    output.tex = input.tex;
    return output;
}