#include "proteinBuilder.h"

#include <algorithm>

void ProteinBuilder::buildProtein(const ProteinData& proteinData,  BuildType type, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
    switch (type)
    {
    case BuildType::WIREFRAME:
    {
        constructCompleteWireframe(proteinData, vertices, indices);
        break;
    }
    }
}


void ProteinBuilder::constructCompleteWireframe(const ProteinData& proteinData, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
    const Model& model = proteinData.models[0]; // todo: choose which model
    for (int i = 0; i < model.size(); ++i)
    {   
        Chain chain = model[i];
        Chain atoms;
        atoms.reserve(128);
        std::copy_if(chain.begin(), chain.end(), std::back_inserter(atoms),
            [](Atom& atom) { return strcmp("CA", atom.name) == 0; });

        std::vector<Vertex> wireframeVertices;
        std::vector<unsigned int> wireframeIndices;

        m_tubeBuilder.buildWireframe(atoms, wireframeVertices, wireframeIndices);

        bufferCombinder(vertices, wireframeVertices, indices, wireframeIndices);
    }
}


void ProteinBuilder::bufferCombinder(std::vector<Vertex>& destVertex, const std::vector<Vertex>& srcVertex, std::vector<unsigned int>& destIndex, const std::vector<unsigned int>& srcIndex)
{
    // todo: memcopy?
    destVertex.insert(destVertex.end(), srcVertex.begin(), srcVertex.end());
    destIndex.reserve(destIndex.size() + srcIndex.size());
    unsigned int newMaxIdx = 0;
    for (const auto idx : srcIndex)
    {
        auto x = m_maxIndex + idx;
        destIndex.push_back(x);
        if (newMaxIdx < x) newMaxIdx = x;
    }
    m_maxIndex = newMaxIdx + 1;
}




