////////////////////////////////////////////////////////////////////////////////
// Filename: input.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUT_H_
#define _INPUT_H_

//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <DirectXMath.h>

////////////////////////////////////////////////////////////////////////////////
// Class name: Input
////////////////////////////////////////////////////////////////////////////////
class Input
{
public:
    const float ZOOM_DELTA = 0.1f;

    Input();
    Input(const Input&);
    ~Input();

    void Initialize();

    void KeyDown(unsigned int);
    void KeyUp(unsigned int);
    void LClickDown();
    void LClickUp();
    void RClickDown();
    void RClickUp();
    void WheelRotation(int);
    bool IsKeyDown(unsigned int);

    DirectX::XMINT2 GetMouseRotation();
    DirectX::XMINT2 GetMouseTransition();
    float GetZoom();


private:
    

private:
    POINT m_lastMousePos;
    bool m_keys[256];
    bool m_lButtonDown;
    bool m_rButtonDown;
    float m_wheelRotation;
};

#endif