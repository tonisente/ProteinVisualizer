////////////////////////////////////////////////////////////////////////////////
// Filename: proteinModel.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PROTEINMODEL_H_
#define _PROTEINMODEL_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////



////////////////////////////////////////////////////////////////////////////////
// Class name: ProteinModel
////////////////////////////////////////////////////////////////////////////////
class ProteinModel
{
private:
    struct VertexType
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT4 color;
        DirectX::XMFLOAT3 normal;
    };

public:
    ProteinModel();
    ProteinModel(const ProteinModel&);
    ~ProteinModel();
    bool Initialize(ID3D11Device*, LPCWSTR, LPCWSTR);
    void Shutdown();

    // The Render function puts the model geometry on the video card 
    // to prepare it for drawing by the color shader.
    void prepareForRender(ID3D11DeviceContext*);

    int GetIndexCount();

private:
    bool InitializeBuffers(ID3D11Device*);
    void releaseBuffers();
    void prepareBuffers(ID3D11DeviceContext*);

private:
    ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
    int m_vertexCount, m_indexCount;
    int m_curveStep;
    int m_curveStart;
    int m_curveEnd;
};

#endif
