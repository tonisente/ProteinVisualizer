////////////////////////////////////////////////////////////////////////////////
// Filename: proteinModel.cpp
////////////////////////////////////////////////////////////////////////////////
#include <vector>

#include "proteinModel.h"
#include "mathLib.h"

ProteinModel::ProteinModel()
{
    m_vertexBuffer = 0;
    m_indexBuffer = 0;
}

ProteinModel::ProteinModel(const ProteinModel& other)
{
}

ProteinModel::~ProteinModel()
{
}

bool ProteinModel::Initialize(ID3D11Device* device, LPCWSTR modelFilename, LPCWSTR textureFilename)
{
    bool result;

    // Initialize the vertex and index buffer that hold the geometry for the triangle.
    result = InitializeBuffers(device);
    if (!result)
    {
        return false;
    }
}

void ProteinModel::Shutdown()
{
    // Release the vertex and index buffers.
    releaseBuffers();

    return;
}

void ProteinModel::prepareForRender(ID3D11DeviceContext* deviceContext)
{
    // Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
    prepareBuffers(deviceContext);

    return;
}

int ProteinModel::GetIndexCount()
{
    return m_indexCount;
}

bool ProteinModel::InitializeBuffers(ID3D11Device* device)
{
    VertexType* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    int i;

    // Lesson 1-6 stuff 
    {
        // Set the number of vertices in the vertex array.
        m_vertexCount = 3;

        // Set the number of indices in the index array.
        m_indexCount = 3;

        // Create the vertex array.
        vertices = new VertexType[m_vertexCount];
        if (!vertices)
        {
            return false;
        }

        // Create the index array.
        indices = new unsigned long[m_indexCount];
        if (!indices)
        {
            return false;
        }

        // Load the vertex array with data.
        vertices[0].position = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
        vertices[0].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
        vertices[0].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

        vertices[1].position = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top center.
        vertices[1].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
        vertices[1].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

        vertices[2].position = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
        vertices[2].color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
        vertices[2].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

        // Load the index array with data.
        indices[0] = 0;  // Bottom left.
        indices[1] = 1;  // Top middle.
        indices[2] = 2;  // Bottom right.
    }

    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    // Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
    if (FAILED(result))
    {
        return false;
    }

    // Release the arrays now that the vertex and index buffers have been created and loaded.
    delete[] vertices;
    vertices = 0;

    delete[] indices;
    indices = 0;

    return true;
}

void ProteinModel::releaseBuffers()
{
    // Release the index buffer.
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = 0;
    }

    // Release the vertex buffer.
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = 0;
    }

    return;
}

void ProteinModel::prepareBuffers(ID3D11DeviceContext* deviceContext)
{
    unsigned int stride;
    unsigned int offset;


    // Set vertex buffer stride and offset.
    stride = sizeof(VertexType);
    offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}
