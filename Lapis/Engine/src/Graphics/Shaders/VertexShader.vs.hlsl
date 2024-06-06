cbuffer MatrixBuffer
{
    matrix view;
    matrix projection;
}

struct VS_INPUT
{
    
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    int materialIndex : MATERIALINDEX;
    matrix instanceWorld : INSTANCEWORLD; // Added per-instance world matrix
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    int materialIndex : MATERIALINDEX;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    float4 worldPosition = mul(float4(input.pos, 1.0f), input.instanceWorld); // Apply instance-specific world matrix
    worldPosition = mul(worldPosition, view); // Apply view matrix
    output.pos = mul(worldPosition, projection); // Apply projection matrix
    output.normal = mul(input.normal, (float3x3) input.instanceWorld);
    output.color = input.color;
    output.tex = input.tex;
    output.materialIndex = input.materialIndex;
    return output;
}