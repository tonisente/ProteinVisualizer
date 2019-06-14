////////////////////////////////////////////////////////////////////////////////
// Filename: Camera.cpp
////////////////////////////////////////////////////////////////////////////////
#include "camera.h"

Camera::Camera()
{
    m_positionX = 0.0f;
    m_positionY = 0.0f;
    m_positionZ = -10.0f;

    m_rotationX = 0.0f;
    m_rotationY = 0.0f;
    m_rotationZ = 0.0f;

    m_upX = 0.0f;
    m_upY = 1.0f;
    m_upZ = 0.0f;

    m_focusPoint = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
    m_positionX = x;
    m_positionY = y;
    m_positionZ = z;
    return;
}

void Camera::SetRotation(float x, float y, float z)
{
    m_rotationX = x;
    m_rotationY = y;
    m_rotationZ = z;
    return;
}

void Camera::SetFocusPoint(float x, float y, float z)
{
    m_focusPoint = DirectX::XMFLOAT3(x, y, z);
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
    return DirectX::XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}

DirectX::XMFLOAT3 Camera::GetRotation()
{
    return DirectX::XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}

DirectX::XMFLOAT3 Camera::GetLookDirection()
{
    return { m_focusPoint.x - m_positionX, m_focusPoint.y - m_positionY, m_focusPoint.z - m_positionZ };
}

void Camera::Rotate(DirectX::XMINT2 mouseDelta)
{   // it doesn't matter if vectors are normalize or not
    if (mouseDelta.x == 0 && mouseDelta.y == 0) return;

    /* yaw rotation of target-to-camera vector (camera position) around cameraUp vector */
    DirectX::XMVECTOR cameraUp = DirectX::XMVectorSet(m_upX, m_upY, m_upZ, 1.0f);
    DirectX::XMMATRIX yawMat = DirectX::XMMatrixRotationAxis(cameraUp, mouseDelta.x * ROTATION_FACTOR);

    // step 1: translation of camera so that focus point is in origin. 
    DirectX::XMVECTOR focusPointToCameraVec = DirectX::XMVectorSet
    (
        m_positionX - m_focusPoint.x,
        m_positionY - m_focusPoint.y,
        m_positionZ - m_focusPoint.z,
        1.0f
    );

    // step 2.1: yaw rotation around up vector
    focusPointToCameraVec = DirectX::XMVector3Transform(focusPointToCameraVec, yawMat);

    /* pitch rotation of target-to-camera vector (camera position) around camera's right vector */
    DirectX::XMVECTOR cameraRightVector = DirectX::XMVector3Cross(cameraUp, DirectX::XMVectorScale(focusPointToCameraVec, -1.0f)); // todo: try without * -1
    DirectX::XMMATRIX pitchRotation = DirectX::XMMatrixRotationAxis(cameraRightVector, mouseDelta.y * ROTATION_FACTOR);

    // step 2.2: pitch rotation around camera's right vector
    focusPointToCameraVec = DirectX::XMVector3Transform(focusPointToCameraVec, pitchRotation);
    cameraUp = DirectX::XMVector3Transform(cameraUp, pitchRotation); // we also have to rotate up vector

    // convert to float4 and save new position and new up vector;
    DirectX::XMFLOAT3 placeholderVec;
    DirectX::XMStoreFloat3(&placeholderVec, focusPointToCameraVec);

    // step 3: reverse translation of the camera position
    placeholderVec = addVectors(placeholderVec, m_focusPoint);

    // store new position
    m_positionX = placeholderVec.x;
    m_positionY = placeholderVec.y;
    m_positionZ = placeholderVec.z;
    // update up vector
    DirectX::XMStoreFloat3(&placeholderVec, cameraUp);
    m_upX = placeholderVec.x;
    m_upY = placeholderVec.y;
    m_upZ = placeholderVec.z;
}

void Camera::Zoom(float amount)
{
    if (amount == 1.0f) return;

    DirectX::XMVECTOR cameraPosVec = DirectX::XMVectorSet(m_positionX, m_positionY, m_positionZ, 1.0f);
    cameraPosVec = DirectX::XMVectorScale(cameraPosVec, amount);
    // convert to float4 and save new position
    DirectX::XMFLOAT4 placeholderVec;
    DirectX::XMStoreFloat4(&placeholderVec, cameraPosVec);
    m_positionX = placeholderVec.x;
    m_positionY = placeholderVec.y;
    m_positionZ = placeholderVec.z;
}

void Camera::Translate(DirectX::XMINT2 mouseDelta)
{
    if (mouseDelta.x == 0 && mouseDelta.y == 0) return;

    DirectX::XMVECTOR up = DirectX::XMVectorSet(m_upX, m_upY, m_upZ, 1.0f);
    DirectX::XMVECTOR lookAt = DirectX::XMVectorSet
    (
        m_focusPoint.x - m_positionX,
        m_focusPoint.y - m_positionY,
        m_focusPoint.z - m_positionZ,
        1.0f
    );
    DirectX::XMFLOAT3 rightNormalized;
    DirectX::XMFLOAT3 upNormalized;
    DirectX::XMStoreFloat3(&rightNormalized, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(up, lookAt)));
    DirectX::XMStoreFloat3(&upNormalized, DirectX::XMVector3Normalize(up));

    mouseDelta.x *= -1;
    DirectX::XMFLOAT3 delta
    (
        (rightNormalized.x * mouseDelta.x + upNormalized.x * mouseDelta.y) * TRANSLATION_FACTOR,
        (rightNormalized.y * mouseDelta.x + upNormalized.y * mouseDelta.y) * TRANSLATION_FACTOR,
        (rightNormalized.z * mouseDelta.x + upNormalized.z * mouseDelta.y) * TRANSLATION_FACTOR
    );

    m_positionX += delta.x;
    m_positionY += delta.y;
    m_positionZ += delta.z;

    m_focusPoint = addVectors(m_focusPoint, delta);
}


void Camera::Render()
{
    DirectX::XMVECTOR up, position, lookAtPoint;
    //float yaw, pitch, roll;
    //DirectX::XMMATRIX rotationMatrix;

    // Setup the vector that points upwards.
    up = DirectX::XMVectorSet(m_upX, m_upY, m_upZ, 1.0f);

    // Setup the position of the camera in the world.
    position = DirectX::XMVectorSet(m_positionX, m_positionY, m_positionZ, 1.0f);

    // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
    //pitch = m_rotationX * 0.0174532925f;
    //yaw = m_rotationY * 0.0174532925f;
    //roll = m_rotationZ * 0.0174532925f;

    // Create the rotation matrix from the yaw, pitch, and roll values.
    //rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
    //DirectX::XMMatrixRotationRollPitchYawFromVector()

    // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
    lookAtPoint = DirectX::XMVectorSet(m_focusPoint.x, m_focusPoint.y, m_focusPoint.z, 1.0f);
    //up = DirectX::XMVector3TransformCoord(up, rotationMatrix);

    //using namespace DirectX; // trolling ... 
    // Translate the rotated camera position to the location of the viewer.
    //lookAt += position; // if it is vector, then it would have sense

    // Finally create the view matrix from the three updated vectors.
    m_viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAtPoint, up);
    //D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

    return;
}

void Camera::GetViewMatrix(DirectX::XMMATRIX& viewMatrix)
{
    viewMatrix = m_viewMatrix;
    return;
}

DirectX::XMFLOAT3 Camera::addVectors(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
    return DirectX::XMFLOAT3
    (
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    );
}

DirectX::XMFLOAT3 Camera::subVectors(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
    return DirectX::XMFLOAT3
    (
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z
    );
}
