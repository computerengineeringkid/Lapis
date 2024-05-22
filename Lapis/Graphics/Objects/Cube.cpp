#include "Cube.h"
#include <iostream>
#include "Graphics/GraphicsManager.h"

Cube::Cube(ID3D11Device* device, int instanceCount)
    :m_instanceCount(instanceCount)
{
    m_VertexShader = std::make_shared<VertexShader>(L"d");
    m_PixelShader = std::make_shared<PixelShader>(L"d");
    if (!m_VertexShader->Initialize(device)) {
        // Handle errors
    }
    if (!m_PixelShader->Initialize(device)) {
        // Handle errors
    }
    if (!InitializeBuffers(device)) {
        std::cerr << "Error: Failed to initialize buffers." << std::endl;
    }
}

Cube::~Cube()
{
}

void Cube::Render(ID3D11DeviceContext* deviceContext)
{
    m_VertexShader->Bind(deviceContext);
    m_PixelShader->Bind(deviceContext);

    unsigned int strides[2] = { sizeof(Vertex), sizeof(InstanceData) };
    unsigned int offsets[2] = { 0, 0 };
    ID3D11Buffer* buffers[2] = { m_VertexBuffer.Get(), m_InstanceBuffer.Get() };

    deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
    deviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->DrawIndexedInstanced(36, m_instanceCount, 0, 0, 0);
}

void Cube::UpdateInstanceData(const InstanceData& data)
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
    auto deviceContext = GraphicsManager::Get().GetDeviceContext();
    deviceContext->Map(m_InstanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &data, sizeof(InstanceData));  
    deviceContext->Unmap(m_InstanceBuffer.Get(), 0);
}

bool Cube::InitializeBuffers(ID3D11Device* device)
{
    HRESULT hr;
    // Instance buffer setup
    if (m_instanceCount > 0) {
        D3D11_BUFFER_DESC instanceBufferDesc = {};
        instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        instanceBufferDesc.ByteWidth = sizeof(InstanceData) * m_instanceCount;
        instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        hr = device->CreateBuffer(&instanceBufferDesc, nullptr, m_InstanceBuffer.GetAddressOf());
        if (FAILED(hr)) {
            std::cerr << "Failed to create instance buffer. HRESULT: " << std::hex << hr << std::endl;
            return false;
        }
    }

    

    const Vertex vertices[] = {
        // Front face
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}, // Bottom left
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},  // Bottom right
        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},   // Top right
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},  // Top left

        // Back face
        {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},  // Bottom right
        {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},   // Bottom left
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},    // Top left
        {{-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f, 1.0f}, {1.0f, 0.0f}}     // Top right
    };
    unsigned int indices[] = {
    0, 1, 2, 0, 2, 3, // Front face
    4, 6, 5, 4, 7, 6, // Back face
    4, 5, 1, 4, 1, 0, // Bottom face
    3, 2, 6, 3, 6, 7, // Top face
    1, 5, 6, 1, 6, 2, // Right face
    4, 0, 3, 4, 3, 7  // Left face
    };
    D3D11_BUFFER_DESC vertexBufferDesc = {
       sizeof(vertices),
       D3D11_USAGE_DEFAULT,
       D3D11_BIND_VERTEX_BUFFER,
       0,
       0,
       sizeof(Vertex)
    };
    D3D11_SUBRESOURCE_DATA vertexData = { vertices, 0, 0 };

    hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_VertexBuffer.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "Failed to create vertex buffer. HRESULT: " << std::hex << hr << std::endl;
        return false;
    }

    D3D11_BUFFER_DESC indexBufferDesc = {
        sizeof(indices),
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_INDEX_BUFFER,
        0,
        0
    };
    D3D11_SUBRESOURCE_DATA indexData = { indices, 0, 0 };

    hr = device->CreateBuffer(&indexBufferDesc, &indexData, m_IndexBuffer.GetAddressOf());
    if (FAILED(hr)) {
        std::cerr << "Failed to create index buffer. HRESULT: " << std::hex << hr << std::endl;
        return false;
    }

    return true;
    
}
