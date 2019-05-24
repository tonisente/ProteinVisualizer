#include "proteinBuilder.h"

#include <algorithm>


ProteinBuilder::ProteinBuilder(const ProteinData& proteinData, std::vector<Vertex>& vertices, std::vector<uint>& indices):
    m_proteinData{ proteinData },
    m_vertices{ vertices },
    m_indices{ indices }
{
}


void ProteinBuilder::buildProtein(BuildType type)
{
    m_buildType = type;

    // clear buffers
    m_vertices.clear();
    m_indices.clear();

    switch (type)
    {
    case BuildType::WIREFRAME:
    case BuildType::CURVEDWIREFRAME:
    {
        constructCompleteWireframe();
        break;
    }
    case BuildType::TERTIARY:
        constructTertiary();
        break;
    }
}


void ProteinBuilder::constructCompleteWireframe()
{
    const Model& model = m_proteinData.models[0]; // todo: choose which model
    for (int i = 0; i < model.size(); ++i)
    {   
        std::vector<Vec3> points = filterChain(model[i]);

        std::vector<Vertex> wireframeVertices;
        std::vector<uint> wireframeIndices;

        if (m_buildType == BuildType::WIREFRAME)
        {
            m_tubeBuilder.buildWireframe(points, wireframeVertices, wireframeIndices);
        }
        else if (m_buildType == BuildType::CURVEDWIREFRAME)
        {
            m_tubeBuilder.buildCurvedWireframe(points, wireframeVertices, wireframeIndices);
        }

        bufferCombinder(wireframeVertices, wireframeIndices);
    }
}


void ProteinBuilder::bufferCombinder(const std::vector<Vertex>& srcVertex, const std::vector<uint>& srcIndex)
{
    // todo: memcopy?
    m_vertices.insert(m_vertices.end(), srcVertex.begin(), srcVertex.end());
    m_indices.reserve(m_indices.size() + srcIndex.size());
    uint newMaxIdx = 0;
    for (const uint idx : srcIndex)
    {
        uint x = m_maxIndex + idx;
        m_indices.push_back(x);
        if (newMaxIdx < x) newMaxIdx = x;
    }
    m_maxIndex = newMaxIdx + 1;
}


std::vector<Vec3> ProteinBuilder::filterChain(const Chain& chain) const
{
    std::vector<Vec3> atomPositions;
    atomPositions.reserve(128);

    for (const Atom atom : chain)
    {
        if (strcmp("CA", atom.name) == 0)
        {
            atomPositions.push_back(Vec3(atom.xCoord, atom.yCoord, atom.zCoord));
        }
    }

    return atomPositions;
}


void ProteinBuilder::constructTertiary()
{

}
