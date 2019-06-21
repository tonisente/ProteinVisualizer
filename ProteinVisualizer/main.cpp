#pragma once
#include "system.h"
#include "inputParameterParser.h"

#include <windows.h>

std::string __proteinFilename = "Toni";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    System* system;
    bool result;
    InputParameterParser inputParser;

    // parse input parameters
    inputParser.parse(pScmdline);

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