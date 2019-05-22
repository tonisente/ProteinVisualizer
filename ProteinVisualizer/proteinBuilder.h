#pragma once
#include "vertex.h"
#include "proteinData.h"
#include "tubeBuilder.h"
#include "curve.h"

#include <vector>

typedef unsigned int uint;

class ProteinBuilder
{
public:
    enum class BuildType
    {
        WIREFRAME,
        CURVEDWIREFRAME,
        TERTIARY
    };

    void buildProtein(const ProteinData& proteinData, BuildType type, std::vector<Vertex>& vertices, std::vector<uint>& indices);

private:
    void constructCompleteWireframe(const ProteinData& proteinData, BuildType type, std::vector<Vertex>& vertices, std::vector<uint>& indices);
    void constructAlphaHelix(const ProteinData& proteinData, std::vector<Vertex>& vertices, std::vector<uint>& indices);

    void bufferCombinder(std::vector<Vertex>& destVertex, const std::vector<Vertex>& srcVertex, std::vector<uint>& destIndex, const std::vector<uint>& srcIndex);

    std::vector<Vec3> filterChain(const Chain& chain) const;

    TubeBuilder m_tubeBuilder;
    unsigned int m_maxIndex = 0;
};

