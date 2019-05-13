////////////////////////////////////////////////////////////////////////////////
// Filename: camera.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_


//////////////
// INCLUDES //
//////////////
#include <DirectXMath.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: Camera
////////////////////////////////////////////////////////////////////////////////
class Camera
{
public:
    const float ROTATION_FACTOR = 0.01f;
    const float ZOOM_FACTOR = 0.1f;
    const float TRANSLATION_FACTOR = 0.01f;

    Camera();
    Camera(const Camera&);
    ~Camera();

    void SetPosition(float, float, float);
    void SetFocusPoint(float, float, float);
    void SetRotation(float, float, float);

    DirectX::XMFLOAT3 GetPosition();
    DirectX::XMFLOAT3 GetRotation();

    void Rotate(DirectX::XMINT2 mouseDelta);
    void Zoom(float zoom);
    void Translate(DirectX::XMINT2 mouseDelta);

    void Render();
    void GetViewMatrix(DirectX::XMMATRIX&);

private:
    DirectX::XMFLOAT3 addVectors(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);
    DirectX::XMFLOAT3 subVectors(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2);

private:
    DirectX::XMFLOAT3 m_focusPoint;
    float m_positionX, m_positionY, m_positionZ;
    float m_rotationX, m_rotationY, m_rotationZ;
    float m_upX, m_upY, m_upZ;
    DirectX::XMMATRIX m_viewMatrix;
};

#endif