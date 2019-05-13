////////////////////////////////////////////////////////////////////////////////
// Filename: lightShader.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTSHADER_H_
#define _LIGHTSHADER_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;


////////////////////////////////////////////////////////////////////////////////
// Class name: LightShader
////////////////////////////////////////////////////////////////////////////////
class LightShader
{
private:
    struct MatrixBufferType
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    struct LightBufferType
    {
        DirectX::XMFLOAT4 ambientColor;
        DirectX::XMFLOAT4 diffuseColor;
        DirectX::XMFLOAT3 lightDirection;
        float specularPower;
        DirectX::XMFLOAT4 specularColor;
    };

    struct CameraBufferType
    {
        DirectX::XMFLOAT3 cameraPosition;
        float padding;
    };

public:
    LightShader();
    LightShader(const LightShader&);
    ~LightShader();

    bool Initialize(ID3D11Device*, HWND);
    void Shutdown();
    bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMFLOAT3,
        DirectX::XMFLOAT4, DirectX::XMFLOAT4, DirectX::XMFLOAT3, DirectX::XMFLOAT4, float);

private:
    bool InitializeShader(ID3D11Device*, HWND, LPCWSTR, LPCWSTR);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3D10Blob*, HWND, LPCWSTR);

    bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMFLOAT3,
        DirectX::XMFLOAT4, DirectX::XMFLOAT4, DirectX::XMFLOAT3, DirectX::XMFLOAT4, float);
    void RenderShader(ID3D11DeviceContext*, int);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_lightBuffer;
    ID3D11Buffer* m_cameraBuffer;
};

#endif