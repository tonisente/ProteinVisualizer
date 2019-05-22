////////////////////////////////////////////////////////////////////////////////
// Filename: graphics.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphics.h"


Graphics::Graphics()
{
    m_D3D = 0;
    m_Camera = 0;
    m_proteinModel = 0;
    m_LightShader = 0;
    m_Light = 0;
}


Graphics::Graphics(const Graphics& other)
{
}


Graphics::~Graphics()
{
}


bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    bool result;


    // Create the Direct3D object.
    m_D3D = new D3D;
    if (!m_D3D)
    {
        return false;
    }

    // Initialize the Direct3D object.
    result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }


    // Create the camera object.
    m_Camera = new Camera;
    if (!m_Camera)
    {
        return false;
    }

    // Set the initial position of the camera.
    m_Camera->SetPosition(0.0f, 0.0f, -50.0f);

    // Create the model object.
    m_proteinModel = new ProteinModel;
    if (!m_proteinModel)
    {
        return false;
    }

    // Initialize the model object.
    //result = m_Model->Initialize(m_D3D->GetDevice());
    result = m_proteinModel->Initialize(m_D3D->GetDevice(), L"Resources/cube.txt", L"Resources/seafloor.dds");
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    // Create the light shader object.
    m_LightShader = new LightShader;
    if (!m_LightShader)
    {
        return false;
    }

    // Initialize the light shader object.
    result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
    if (!result)
    {
        MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
        return false;
    }

    // Create the light object.
    m_Light = new Light;
    if (!m_Light)
    {
        return false;
    }

    // Initialize the light object.
    m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
    m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_Light->SetDirection(0.0f, 0.0f, 1.0f);
    m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_Light->SetSpecularPower(32.0f);

    return true;
}


void Graphics::Shutdown()
{

    // Release the light object.
    if (m_Light)
    {
        delete m_Light;
        m_Light = 0;
    }

    // Release the light shader object.
    if (m_LightShader)
    {
        m_LightShader->Shutdown();
        delete m_LightShader;
        m_LightShader = 0;
    }

    // Release the model object.
    if (m_proteinModel)
    {
        m_proteinModel->Shutdown();
        delete m_proteinModel;
        m_proteinModel = 0;
    }

    // Release the camera object.
    if (m_Camera)
    {
        delete m_Camera;
        m_Camera = 0;
    }

    if (m_D3D)
    {
        m_D3D->Shutdown();
        delete m_D3D;
        m_D3D = 0;
    }

    return;
}


bool Graphics::Frame(DirectX::XMINT2 rotationDelta, DirectX::XMINT2 translationDelta, float zoomDelta)
{
    bool result;

    result = Render(rotationDelta, translationDelta, zoomDelta);
    if (!result)
    {
        return false;
    }

    return true;
}

bool Graphics::Render(DirectX::XMINT2 rotationDelta, DirectX::XMINT2 translationDelta, float zoomDelta)
{
    DirectX::XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
    bool result;

    // Clear the buffers to begin the scene.
    m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 0.0f);

    // Generate the view matrix based on the camera's position.
    m_Camera->Rotate(rotationDelta);
    m_Camera->Zoom(zoomDelta);
    m_Camera->Translate(translationDelta);
    m_Camera->Render();

    //Get the world view and projection matrices from the camera and d3d objects
    m_Camera->GetViewMatrix(viewMatrix);
    m_D3D->GetWorldMatrix(worldMatrix);
    m_D3D->GetProjectionMatrix(projectionMatrix);

    //// Update light to point in the same direction as camera
    //auto dir = m_Camera->GetLookDirection();
    //m_Light->SetDirection(dir.x, dir.y, dir.z);

    // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
    m_proteinModel->prepareForRender(m_D3D->GetDeviceContext());

    // Render the model using the light shader.
    result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_proteinModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
                                   m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), 
                                   m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
    if (!result)
    {
        return false;
    }

    // Present the rendered scene to the screen.
    m_D3D->EndScene();
    return true;
}
