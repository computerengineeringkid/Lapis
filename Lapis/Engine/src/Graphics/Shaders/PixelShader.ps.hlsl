// Pixel Shader
Texture2D shaderTexture : register(t0); // The texture
SamplerState sampleType : register(s0); // The sampler

cbuffer MaterialBuffer
{
    float4 materials[10]; // Assuming a maximum of 10 materials for simplicity
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    int materialIndex : MATERIALINDEX;
    row_major matrix instanceWorld : INSTANCEWORLD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // Sample the texture
    float4 textureColor = shaderTexture.Sample(sampleType, input.tex);

    // Use the material color based on the material index
    float4 materialColor = materials[input.materialIndex];

    // Blend the material color with the texture color
    float4 finalColor = input.color * textureColor;

    return finalColor;
}