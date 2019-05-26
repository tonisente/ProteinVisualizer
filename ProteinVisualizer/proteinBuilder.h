#pragma once
#include "vertex.h"
#include "proteinData.h"
#include "tubeBuilder.h"
#include "curve.h"

#include <vector>

typedef unsigned int uint;

#define HELIX 0
#define SHEET 1

class ProteinBuilder
{
public:
    ProteinBuilder(const ProteinData& proteinData, std::vector<Vertex>& m_vertices, std::vector<uint>& indices);

    enum class BuildType
    {
        WIREFRAME,
        CURVEDWIREFRAME,
        TERTIARY
    };

    void buildProtein(BuildType buildType);

private:
    void constructCompleteWireframe();
    void constructAlphaHelix();
    void constructTertiary();

    /** Puts all vertices and indices from source vertex/index buffer into original vertex/index buffer. */
    void bufferCombinder(const std::vector<Vertex>& srcVertex, const std::vector<uint>& srcIndex);

    std::vector<Vec3> filterChain(const Chain& chain) const;

    TubeBuilder m_tubeBuilder;
    unsigned int m_maxIndex = 0;
    const ProteinData& m_proteinData;
    std::vector<Vertex>& m_vertices;
    std::vector<uint>& m_indices;
    BuildType m_buildType;
    std::vector<std::vector<std::pair<uint, uint>>> m_helixSheetChainIndex;
    uint m_noChains;
};

