////////////////////////////////////////////////////////////////////////////////
// Filename: input.cpp
////////////////////////////////////////////////////////////////////////////////
#include "input.h"


Input::Input()
{
}


Input::Input(const Input& other)
{
}


Input::~Input()
{
}


void Input::Initialize()
{
    int i;

    // Initialize all the keys to being released and not pressed.
    for (i = 0; i < 256; i++)
    {
        m_keys[i] = false;
    }

    m_lButtonDown = false;
    m_rButtonDown = false;
    m_wheelRotation = 1.0f;

    return;
}

void Input::LClickDown()
{
    m_lButtonDown = true;
    GetCursorPos(&m_lastMousePos);
}

void Input::LClickUp()
{
    m_lButtonDown = false;
}

void Input::RClickDown()
{
    m_rButtonDown = true;
    GetCursorPos(&m_lastMousePos);
}

void Input::RClickUp()
{
    m_rButtonDown = false;
}

DirectX::XMINT2 Input::GetMouseRotation()
{
    if (!m_lButtonDown)
    {
        return DirectX::XMINT2(0, 0);
    }

    POINT currentPos;
    GetCursorPos(&currentPos);
    DirectX::XMINT2 delta = DirectX::XMINT2(currentPos.x - m_lastMousePos.x, currentPos.y - m_lastMousePos.y);
    m_lastMousePos = currentPos; // update 

    return delta;
}

float Input::GetZoom()
{
    // invert mouse wheel value 
    float ret = m_wheelRotation;
    m_wheelRotation = 1.0f;
    return ret;
}

DirectX::XMINT2 Input::GetMouseTransition()
{
    if (!m_rButtonDown)
    {
        return DirectX::XMINT2(0, 0);
    }

    POINT currentPos;
    GetCursorPos(&currentPos);
    DirectX::XMINT2 delta = DirectX::XMINT2(currentPos.x - m_lastMousePos.x, currentPos.y - m_lastMousePos.y);
    m_lastMousePos = currentPos; // update 

    return delta;
}

void Input::WheelRotation(int amount)
{
    if (amount < 0)
    {
        m_wheelRotation += ZOOM_DELTA;
    }
    else
    {
        m_wheelRotation -= ZOOM_DELTA;
    }
    return;
}

void Input::KeyDown(unsigned int input)
{
    // If a key is pressed then save that state in the key array.
    m_keys[input] = true;

    return;
}

void Input::KeyUp(unsigned int input)
{
    // If a key is released then clear that state in the key array.
    m_keys[input] = false;
    return;
}


bool Input::IsKeyDown(unsigned int key)
{
    // Return what state the key is in (pressed/not pressed).
    return m_keys[key];
}