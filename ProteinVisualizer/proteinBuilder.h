#pragma once
#include "helixBuilder.h"
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
    enum class BuildType
    {
        WIREFRAME,
        CURVEDWIREFRAME,
        TERTIARY
    };

    const float curveTension = 1.0f;
    unsigned int partsPerCurveSegment = 10;

    ProteinBuilder(const ProteinData& proteinData, std::vector<Vertex>& m_vertices, std::vector<uint>& indices);
    void buildProtein(BuildType buildType);

private:
    void constructCompleteWireframe();
    void constructAlphaHelix();
    void constructTertiary();

    /** Puts all vertices and indices from source vertex/index buffer into original vertex/index buffer. */
    void bufferCombinder(const std::vector<Vertex>& srcVertex, const std::vector<uint>& srcIndex);

    std::vector<Vec3> filterChain(const Chain& chain) const;
    std::pair<std::vector<Vec3>, std::vector<Vec3>> generateExtendedPoints(const std::vector<Vec3>& basePoints) const;
    void centralizePoints(std::vector<Vec3>& points) const;

    const ProteinData& m_proteinData;
    std::vector<Vertex>& m_vertices;
    std::vector<uint>& m_indices;
    BuildType m_buildType;
    uint m_noChains;

    HelixBuilder m_helixBuilder;
    TubeBuilder m_tubeBuilder;

    unsigned int m_maxIndex = 0;
    std::vector<std::vector<std::pair<uint, uint>>> m_helixSheetChainIndex;
};

