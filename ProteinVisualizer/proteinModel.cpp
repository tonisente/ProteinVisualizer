////////////////////////////////////////////////////////////////////////////////
// Filename: proteinModel.cpp
////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>

#include "proteinModel.h"
#include "mathLib.h"
#include "curve.h"

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
    DirectXVertex* vertices;
    unsigned long* indices;
    D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
    HRESULT result;
    int i;




    {   // wireframe
        PDBParser parser; // todo: static?
        std::vector<ProteinData::Atom> atoms = parser.parse("test.pdb");
        std::vector<ProteinData::Atom> filtered;
        filtered.reserve(128);
        std::copy_if(atoms.begin(), atoms.end(), std::back_inserter(filtered),
            [](ProteinData::Atom& atom) { return strcmp("CA", atom.name) == 0; });
        

        std::vector<std::pair<Vec3, Vec3>> vVertices;
        std::vector<unsigned int> vIndices;

        m_tubeBuilder.buildWireframe(filtered, vVertices, vIndices);

        m_vertexCount = vVertices.size();
        m_indexCount = vIndices.size();
        vertices = new DirectXVertex[m_vertexCount];
        indices = new unsigned long[m_indexCount];

        for (int i = 0; i < m_vertexCount; ++i)
        {
            vertices[i].position = vVertices[i].first.toXMFLOAT3();
            vertices[i].normal = vVertices[i].second.toXMFLOAT3();
            vertices[i].color = DirectX::XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0f);
        }
        for (int i = 0; i < m_indexCount; ++i)
        {
            indices[i] = vIndices[i];
        }
    }




    //{   // tube generator
    //    std::vector<std::pair<Vec3, Vec3>> vVertices;
    //    std::vector<unsigned int> vIndices;


    //    Curve curve;
    //    m_tubeBuilder.buildCurvedTube(curve, 100, vVertices, vIndices);

    //    m_vertexCount = vVertices.size();
    //    m_indexCount = vIndices.size();

    //    vertices = new VertexType[m_vertexCount];
    //    indices = new unsigned long[m_indexCount];

    //    for (int i = 0; i < m_vertexCount; ++i)
    //    {
    //        vertices[i].position = vVertices[i].first.toXMFLOAT3();
    //        vertices[i].normal = vVertices[i].second.toXMFLOAT3();
    //        vertices[i].color = DirectX::XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0f);
    //    }
    //    for (int i = 0; i < m_indexCount; ++i)
    //    {
    //        indices[i] = vIndices[i];
    //    }
    //}









    // Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(DirectXVertex) * m_vertexCount;
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
    stride = sizeof(DirectXVertex);
    offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return;
}
