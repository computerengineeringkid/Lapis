#include "InstanceBuffer.h"
#include "Graphics/GraphicsManager.h"
#include <iostream>
#include "Graphics/Objects/GameObject.h"
InstanceBuffer::InstanceBuffer(int count)
    :m_InstanceCount(count)
{
    
        D3D11_BUFFER_DESC instanceBufferDesc = {};
        instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        instanceBufferDesc.ByteWidth = sizeof(InstanceData) * m_InstanceCount;
        instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        instanceBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = GraphicsManager::Get().GetDevice()->CreateBuffer(&instanceBufferDesc, nullptr, m_InstanceBuffer.GetAddressOf());
        if (FAILED(hr)) {
            std::cerr << "Failed to create instance buffer. HRESULT: " << std::hex << hr << std::endl;

        }
    
}

void InstanceBuffer::UpdateBuffer()
{
}
