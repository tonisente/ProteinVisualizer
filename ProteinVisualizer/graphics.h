////////////////////////////////////////////////////////////////////////////////
// Filename: graphics.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3d.h"
#include "camera.h"
#include "proteinModel.h"
#include "lightshader.h"
#include "light.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

////////////////////////////////////////////////////////////////////////////////
// Class name: Graphics
////////////////////////////////////////////////////////////////////////////////
class Graphics
{
public:
    Graphics();
    Graphics(const Graphics&);
    ~Graphics();

    bool Initialize(int, int, HWND);
    void Shutdown();
    bool Frame(DirectX::XMINT2 rotationDelta, DirectX::XMINT2 translationDelta, float zoomDelta);

private:
    bool Render(DirectX::XMINT2 rotationDelta, DirectX::XMINT2 translationDelta, float zoomDelta);

private:
    D3D* m_D3D;
    Camera* m_Camera;
    ProteinModel* m_proteinModel;
    LightShader* m_LightShader;
    Light* m_Light;
};

#endif