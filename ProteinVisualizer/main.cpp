#pragma once
#include "system.h"

#include <windows.h>

std::string __proteinFilename;
Vec3 __helixRGB{ 1.0f, 0.3f, 0.3f };
Vec3 __sheetRGB{ 1.0f, 1.0f, 0.3f };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    System* system;
    bool result;

    // Create the system object.
    system = new System;
    if (!system)
    {
        return 0;
    }

    // Initialize and run the system object.
    result = system->Initialize(pScmdline);
    if (result)
    {
        system->Run();
    }

    // Shutdown and release the system object.
    system->Shutdown();
    delete system;
    system = 0;


    return 0;
}