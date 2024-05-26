// Pixel Shader
Texture2D shaderTexture : register(t0); // The texture
SamplerState sampleType : register(s0); // The sampler
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    row_major matrix instanceWorld : INSTANCEWORLD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 textureColor = shaderTexture.Sample(sampleType, input.tex);
    //return textureColor; // Only texture color
    return input.color * textureColor; // Blend vertex color with texture color
}