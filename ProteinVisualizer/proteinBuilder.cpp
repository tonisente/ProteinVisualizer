#include "proteinBuilder.h"

#include <algorithm>


ProteinBuilder::ProteinBuilder(const ProteinData& proteinData, std::vector<Vertex>& vertices, std::vector<uint>& indices):
    m_proteinData{ proteinData },
    m_vertices{ vertices },
    m_indices{ indices }
{
    m_noChains = proteinData.models[0].size();

    std::vector<std::pair<uint, uint>> vpuiui;
    m_helixSheetChainIndex.insert(m_helixSheetChainIndex.begin(), m_noChains, vpuiui);

    // indexing helicis
    for (int i = 0; i < proteinData.alphaHelicis.size(); ++i)
    {
        Helix helix = proteinData.alphaHelicis[i];
        uint index = helix.chainID - 'A';
        m_helixSheetChainIndex[i].push_back(std::make_pair(HELIX, helix.serialNumber - 1));
    }
    //// indexing sheets
    //for (int i = 0; i < proteinData.betaSheets.size(); ++i)
    //{
    //    Sheet sheet = proteinData.betaSheets[i];
    //    // todo .. to be continued.
    //}
    for (auto& elem : m_helixSheetChainIndex)
    {
        std::sort(elem.begin(), elem.end(), [](const std::pair<uint, uint>& a, const std::pair<uint, uint>& b) { return a.second < b.second; });
    }
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

        std::vector<float> dist;
        for (int j = 1; j < points.size(); ++j)
        { 
            Vec3 a = points[j - 1];
            Vec3 b = points[j];
            dist.push_back(sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2)));
        }

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
    const Model& model = m_proteinData.models[0];

    for (const Chain& chain : model)
    {
        std::vector<Vec3> filteredChain = filterChain(chain);

    }

}
