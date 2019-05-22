#include "proteinBuilder.h"

#include <algorithm>


void ProteinBuilder::buildProtein(const ProteinData& proteinData,  BuildType type, std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
    switch (type)
    {
    case BuildType::WIREFRAME:
    case BuildType::CURVEDWIREFRAME:
    {
        constructCompleteWireframe(proteinData, type, vertices, indices);
        break;
    }
    }
}


void ProteinBuilder::constructCompleteWireframe(const ProteinData& proteinData, BuildType type, std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
    const Model& model = proteinData.models[0]; // todo: choose which model
    for (int i = 0; i < model.size(); ++i)
    {   
        std::vector<Vec3> atoms = filterChain(model[i]);

        std::vector<Vertex> wireframeVertices;
        std::vector<uint> wireframeIndices;

        if (type == BuildType::WIREFRAME)
        {
            m_tubeBuilder.buildWireframe(atoms, wireframeVertices, wireframeIndices);
        }
        else if (type == BuildType::CURVEDWIREFRAME)
        {
            m_tubeBuilder.buildCurvedWireframe(atoms, wireframeVertices, wireframeIndices);
        }

        bufferCombinder(vertices, wireframeVertices, indices, wireframeIndices);
    }
}


void ProteinBuilder::bufferCombinder(std::vector<Vertex>& destVertex, const std::vector<Vertex>& srcVertex, std::vector<uint>& destIndex, const std::vector<uint>& srcIndex)
{
    // todo: memcopy?
    destVertex.insert(destVertex.end(), srcVertex.begin(), srcVertex.end());
    destIndex.reserve(destIndex.size() + srcIndex.size());
    uint newMaxIdx = 0;
    for (const auto idx : srcIndex)
    {
        auto x = m_maxIndex + idx;
        destIndex.push_back(x);
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



