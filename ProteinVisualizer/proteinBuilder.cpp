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
        std::vector<Vec3> basePoints = filterChain(model[i]);

        std::vector<Vertex> wireframeVertices;
        std::vector<uint> wireframeIndices;

        if (m_buildType == BuildType::WIREFRAME)
        {
            m_tubeBuilder.buildWireframe(basePoints, wireframeVertices, wireframeIndices);
        }
        else if (m_buildType == BuildType::CURVEDWIREFRAME)
        {
            std::vector<Vec3> extendedPoints = generateExtendedPoints(basePoints).first;
            m_tubeBuilder.buildWireframe(extendedPoints, wireframeVertices, wireframeIndices);
        }

        bufferCombinder(wireframeVertices, wireframeIndices);
    }
}


void ProteinBuilder::constructTertiary()
{
    const Model& model = m_proteinData.models[0];

    for (int i = 0; i < model.size(); ++i)
    {
        Chain chain = model[i];

        std::vector<Vertex> wireframeVertices;
        std::vector<uint> wireframeIndices;

        std::vector<Vec3> basePoints = filterChain(chain);
        std::pair<std::vector<Vec3>, std::vector<Vec3>> extended = generateExtendedPoints(basePoints);
        std::vector<Vec3> extendedPoints = extended.first;
        std::vector<Vec3> extendedPointsNormals = extended.second;

        uint wireframeStartIndex = 0;
        for (const std::vector<std::pair<uint, uint>> index : m_helixSheetChainIndex)
        {
            // step 1 - build previous wireframe (till helix/sheet start)


            // step 2 - build structure


            // step 3 - update wireframe start index

        }

        // step 4 - build the rest of the wireframe


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


std::pair<std::vector<Vec3>, std::vector<Vec3>> ProteinBuilder::generateExtendedPoints(const std::vector<Vec3>& basePoints) const
{
    std::vector<Vec3> extendedPoints;
    std::vector<Vec3> tangents;
    uint n = basePoints.size();
    extendedPoints.reserve(n * partsPerCurveSegment);
    tangents.reserve(n * partsPerCurveSegment);

    Vec3 p0, p1, p2, p3;
    for (uint i = 0; i < n - 1; ++i)
    {
        p0 = (i == 0) ? basePoints[1].opposite(basePoints[0]) : basePoints[i - 1];
        p1 = basePoints[i];
        p2 = basePoints[i + 1];
        p3 = (i == n - 2) ? basePoints[n - 2].opposite(basePoints[n - 1]) : basePoints[i + 2];

        for (int j = 0; j < partsPerCurveSegment; ++j)
        {
            float t = (float)j / float(partsPerCurveSegment);
            Vec3 point = Curve::catmullRom(t, curveTension, p0, p1, p2, p3);
            Vec3 tangent = Curve::catmullRom(t, curveTension, p0, p1, p2, p3);

            extendedPoints.push_back(point);
            tangents.push_back(tangent);
        }
    }

    // add last point
    extendedPoints.push_back(basePoints[n - 1]);
    tangents.push_back(Curve::catmullRomTangent(1.0f, curveTension, p0, p1, p2, p3));

    return std::make_pair(extendedPoints, tangents);
}

