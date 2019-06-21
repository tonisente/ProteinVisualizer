////////////////////////////////////////////////////////////////////////////////
// Filename: proteinModel.cpp
////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <algorithm>

#include "proteinModel.h"
#include "mathLib.h"
#include "curve.h"
#include "inputVariables.h"

extern std::string __proteinFilename;


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

    {   // build model
        PDBParser& parser = PDBParser::getInstance();
        //ProteinData proteinData = parser.parse("1_helix_6niv.pdb"); // ok
        //ProteinData proteinData = parser.parse("long_helix_6obi.pdb"); // ok
        //ProteinData proteinData = parser.parse("2sheets_6qkf.pdb"); // ok
        //ProteinData proteinData = parser.parse("5zx1.pdb"); // ok
        //ProteinData proteinData = parser.parse("sheet_test_6dz9.pdb"); // circular problem
        //ProteinData proteinData = parser.parse("1rnu.pdb"); // wierd wireframe connection
        //ProteinData proteinData = parser.parse("simple_helix_sheet_3nir.pdb"); //ok
        //ProteinData proteinData = parser.parse("simple_2_sheets_5zv6.pdb"); // ok
        //ProteinData proteinData = parser.parse("sheetTest6nfr.pdb"); // assert (points.size() == 0)
        //ProteinData proteinData = parser.parse("helixTest6hl1.pdb"); // ok
        //ProteinData proteinData = parser.parse("wireframeTest3.txt"); // ok
        //ProteinData proteinData = parser.parse("complete_5zsy.pdb"); // 
        ProteinData proteinData = parser.parse(__proteinFilename); // 


        std::vector<Vertex> generatedVertecis;
        std::vector<unsigned int> generatedIndices;

        ProteinBuilder proteinBuilder{ proteinData, generatedVertecis, generatedIndices };
        proteinBuilder.buildProtein(ProteinBuilder::BuildType::TERTIARY);

        m_vertexCount = generatedVertecis.size();
        m_indexCount = generatedIndices.size();
        vertices = new DirectXVertex[m_vertexCount];
        indices = new unsigned long[m_indexCount];

        for (int i = 0; i < m_vertexCount; ++i)
        {
            Vertex& vertex{ generatedVertecis[i] };
            vertices[i].position = vertex.position.toXMFLOAT3();
            vertices[i].normal = vertex.normal.toXMFLOAT3();
            Vec3& color = vertex.color;
            vertices[i].color = DirectX::XMFLOAT4(color.x, color.y, color.z, 1.0f);
        }
        for (int i = 0; i < m_indexCount; ++i)
        {
            indices[i] = generatedIndices[i];
        }
    }


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
