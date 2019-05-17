#pragma once
#include <vector>

#include "vertex.h"
#include "proteinData.h"
#include "tubeBuilder.h"


class ProteinBuilder
{
public:
    enum class BuildType
    {
        WIREFRAME,
        CURVEDWIREFRAME,
        TERTIARY
    };

    void buildProtein(const ProteinData& proteinData, BuildType type, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

private:
    void constructCompleteWireframe(const ProteinData& proteinData, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    void bufferCombinder(std::vector<Vertex>& destVertex, const std::vector<Vertex>& srcVertex, std::vector<unsigned int>& destIndex, const std::vector<unsigned int>& srcIndex);

    TubeBuilder m_tubeBuilder;
    unsigned int m_maxIndex = 0;
};

