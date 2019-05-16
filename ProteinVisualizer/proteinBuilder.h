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
    void bufferCombinder(std::vector<Vertex>& destVertex, const std::vector<Vertex>& srcVertex, std::vector<unsigned int>& destIndex, const std::vector<unsigned int> srdIndex);

    TubeBuilder m_tubeBuilder;
    

};

