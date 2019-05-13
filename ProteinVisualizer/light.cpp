////////////////////////////////////////////////////////////////////////////////
// Filename: Light.cpp
////////////////////////////////////////////////////////////////////////////////
#include "light.h"


Light::Light()
{
}


Light::Light(const Light& other)
{
}


Light::~Light()
{
}

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
    m_ambientColor = DirectX::XMFLOAT4(red, green, blue, alpha);
    return;
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
    m_diffuseColor = DirectX::XMFLOAT4(red, green, blue, alpha);
    return;
}


void Light::SetDirection(float x, float y, float z)
{
    m_direction = DirectX::XMFLOAT3(x, y, z);
    return;
}

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
    m_specularColor = DirectX::XMFLOAT4(red, green, blue, alpha);
    return;
}

void Light::SetSpecularPower(float power)
{
    m_specularPower = power;
    return;
}

DirectX::XMFLOAT4 Light::GetAmbientColor()
{
    return m_ambientColor;
}


DirectX::XMFLOAT4 Light::GetDiffuseColor()
{
    return m_diffuseColor;
}


DirectX::XMFLOAT3 Light::GetDirection()
{
    return m_direction;
}

DirectX::XMFLOAT4 Light::GetSpecularColor()
{
    return m_specularColor;
}


float Light::GetSpecularPower()
{
    return m_specularPower;
}
