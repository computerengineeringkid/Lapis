cbuffer MatrixBuffer
{
    matrix view;
    matrix projection;
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
    float4 worldPosition = float4(input.pos, 1.0); // Assuming model space is world space
    output.pos = mul(worldPosition, view); // Apply view matrix
    output.pos = mul(output.pos, projection); // Apply projection matrix
    output.color = input.color;
    return output;
}
