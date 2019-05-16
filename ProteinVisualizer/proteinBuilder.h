#pragma once
#include <vector>

#include "proteinData.h"
#include "tubeBuilder.h"

struct Vertex
{
    Vec3 position;
    Vec3 normal;
    Vec3 color; // alpha of color is always 1
};

class ProteinBuilder
{
public:
    
    void buildProtein(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const ProteinData& proteinData) const;

private:
    void bufferCombinder(std::vector<Vertex>& destBuff, const std::vector<Vertex>& srcBuffer, std::vector<UINT> )

    TubeBuilder m_tubeBuilder;
    

};

