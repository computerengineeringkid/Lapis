#include "Cube.h"
#include <iostream>
#include "Graphics/GraphicsManager.h"
#include "Graphics/Texture.h"
#include "Graphics/Sampler.h"
#include "Graphics/Surface.h"
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Buffers/VertexBuffer.h"
#include "Graphics/Buffers/InstanceBuffer.h"
#include "Graphics/Buffers/Topology.h"
#include "Graphics/Buffers/InputLayout.h"


Cube::Cube(ID3D11Device* device, int instanceCount)
    :m_instanceCount(instanceCount)
{
    m_VertexShader = std::make_shared<VertexShader>(L"VertexShader.vs.cso");
    m_PixelShader = std::make_shared<PixelShader>(L"PixelShader.ps.cso");
    std::vector<Vertex> vertices = {
    {{-1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 1.0f},  {1.0f, 1.0f}},  // Bottom right
    {{ 1.0f, -1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f, 1.0f, 1.0f},  {0.0f, 1.0f}},  // Bottom left
    {{ 1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f, 1.0f, 1.0f},  {0.0f, 0.0f}},  // Top left
    {{-1.0f,  1.0f,  1.0f}, { 0.0f,  0.0f,  1.0f}, {0.5f, 0.5f, 0.5f, 1.0f},  {1.0f, 0.0f}},  // Top right
    // Front face
    {{-1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 1.0f},  {0.0f, 1.0f}},  // Bottom left
    {{ 1.0f, -1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f},  {1.0f, 1.0f}},  // Bottom right
    {{ 1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f, 1.0f, 1.0f},  {1.0f, 0.0f}},  // Top right
    {{-1.0f,  1.0f, -1.0f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f},  {0.0f, 0.0f}},  // Top left

    };
    std::vector<unsigned short> indices = {
    0, 1, 2, 0, 2, 3, // Front face
    4, 6, 5, 4, 7, 6, // Back face

    4, 5, 1, 4, 1, 0, // Bottom face
    3, 2, 6, 3, 6, 7, // Top face
    1, 5, 6, 1, 6, 2, // Right face
    4, 0, 3, 4, 3, 7  // Left face


    };
    m_VertexBuffer = std::make_shared<VertexBuffer>(vertices);
    m_IndexBuffer = std::make_shared<IndexBuffer>(indices);
    m_InstanceBuffer = std::make_shared<InstanceBuffer>(m_instanceCount);
    

   
   /* if (!InitializeBuffers(device)) {
        std::cerr << "Error: Failed to initialize buffers." << std::endl;
    }*/
    SetTexture("Engine\\src\\Graphics\\Images\\DefaultTexture.png");
    
}

Cube::~Cube()
{
}

void Cube::Update(float deltaTime)
{
    if (canRotate)
    {
        float rotationSpeed = 1.0f;  // Define rotation speed
        DirectX::XMFLOAT3 rotation = this->GetRotation();
        rotation.y += deltaTime * rotationSpeed;  // Increment Y rotation based on elapsed time
        this->SetRotation(rotation);
    }
}

void Cube::Render(ID3D11DeviceContext* deviceContext)
{
    m_VertexShader->Bind(deviceContext);
    m_PixelShader->Bind(deviceContext);

    if (m_Texture)
    {
        m_Texture->Bind();
        m_Sampler->Bind();

    }

    unsigned int strides[2] = { sizeof(Vertex), sizeof(InstanceData) };
    unsigned int offsets[2] = { 0, 0 };
    ID3D11Buffer* buffers[2] = { m_VertexBuffer->GetVertexBuffer().Get(), m_InstanceBuffer->GetInstanceBuffer().Get()};

    //m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
    const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"MATERIALINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0},



        {"INSTANCEWORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        {"INSTANCEWORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        {"INSTANCEWORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        {"INSTANCEWORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1}

    };
    auto top = std::make_shared<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    auto Input = std::make_shared<InputLayout>(ied,m_VertexShader->GetBytecode());
    top->Bind();
    Input->Bind();
    deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
    //deviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    //deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->DrawIndexedInstanced(36, m_instanceCount, 0, 0, 0);
}

void Cube::UpdateInstanceData(const InstanceData& data)
{
    if (m_instanceCount > 0) {
        D3D11_MAPPED_SUBRESOURCE mappedResource;
        ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
        auto deviceContext = GraphicsManager::Get().GetDeviceContext();
        deviceContext->Map(m_InstanceBuffer->GetInstanceBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
        memcpy(mappedResource.pData, &data, sizeof(InstanceData));
        deviceContext->Unmap(m_InstanceBuffer->GetInstanceBuffer().Get(), 0);
    }
}

void Cube::SetTexture(const std::string& path)
{
    m_Texture = std::make_shared<Texture>(Surface::FromFile(path));
    m_Sampler = std::make_shared<Sampler>();
}


bool Cube::InitializeBuffers(ID3D11Device* device)
{
    //HRESULT hr;
    //// Instance buffer setup
    //if (m_instanceCount > 0) {
    //    D3D11_BUFFER_DESC instanceBufferDesc = {};
    //    instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    //    instanceBufferDesc.ByteWidth = sizeof(InstanceData) * m_instanceCount;
    //    instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    //    instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    //    hr = device->CreateBuffer(&instanceBufferDesc, nullptr, m_InstanceBuffer.GetAddressOf());
    //    if (FAILED(hr)) {
    //        std::cerr << "Failed to create instance buffer. HRESULT: " << std::hex << hr << std::endl;
    //        return false;
    //    }
    //}

    //

    //
    //D3D11_BUFFER_DESC vertexBufferDesc = {
    //   sizeof(vertices),
    //   D3D11_USAGE_DEFAULT,
    //   D3D11_BIND_VERTEX_BUFFER,
    //   0,
    //   0,
    //   sizeof(Vertex)
    //};
    //D3D11_SUBRESOURCE_DATA vertexData = { vertices, 0, 0 };

    //hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_VertexBuffer.GetAddressOf());
    //if (FAILED(hr)) {
    //    std::cerr << "Failed to create vertex buffer. HRESULT: " << std::hex << hr << std::endl;
    //    return false;
    //}
    //
    //D3D11_BUFFER_DESC indexBufferDesc = {
    //    sizeof(indices),
    //    D3D11_USAGE_DEFAULT,
    //    D3D11_BIND_INDEX_BUFFER,
    //    0,
    //    0
    //};
    //D3D11_SUBRESOURCE_DATA indexData = { indices, 0, 0 };

    //hr = device->CreateBuffer(&indexBufferDesc, &indexData, m_IndexBuffer.GetAddressOf());
    //if (FAILED(hr)) {
    //    std::cerr << "Failed to create index buffer. HRESULT: " << std::hex << hr << std::endl;
    //    return false;
    //}

    return true;
    
}
