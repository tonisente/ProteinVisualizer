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
    void constructWireframe();
    //void constructAlphaHelix();
    void constructTertiary();

    /** Puts all vertices and indices from source vertex/index buffer into original vertex/index buffer. */
    void bufferCombinder(const std::vector<Vertex>& srcVertex, const std::vector<uint>& srcIndex);

    Chain alphaCarbons(const Chain& chain) const;
    std::vector<Vec3> generateExtendedPoints(Vec3 p0, const std::vector<Vec3>& basePoints, Vec3 pn) const;
    void centralizePoints(std::vector<Vertex>& points) const;
    Vec3 getPreviousPoint(const std::vector<Vec3>& points, uint index);
    Vec3 getNextPoint(const std::vector<Vec3>& points, uint index); 
    int residueIndex(const Chain& chain, uint residueSeqNumber);

    const ProteinData& m_proteinData;
    std::vector<Vertex>& m_vertices;
    std::vector<uint>& m_indices;
    BuildType m_buildType;
    uint m_noChains;

    HelixBuilder m_helixBuilder;
    TubeBuilder m_tubeBuilder;

    uint m_maxIndex = 0;
};

