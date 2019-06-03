#include "proteinBuilder.h"

#include <algorithm>


ProteinBuilder::ProteinBuilder(const ProteinData& proteinData, std::vector<Vertex>& vertices, std::vector<uint>& indices):
    m_proteinData{ proteinData },
    m_vertices{ vertices },
    m_indices{ indices }
{
    //m_noChains = proteinData.models[0].size();

    //std::vector<std::pair<uint, uint>> vpuiui;

    //m_helixSheetChainIndex.insert(m_helixSheetChainIndex.begin(), m_noChains, vpuiui);

    //// indexing helicis
    //for (int i = 0; i < proteinData.alphaHelicis.size(); ++i)
    //{
    //    Helix helix = proteinData.alphaHelicis[i];
    //    uint index = helix.chainID - 'A'; // ending chain is always the same as the starting chain, so you don't have to bother about ending
    //    m_helixSheetChainIndex[i].push_back(std::make_pair(HELIX, helix.residueSeqNumber));
    //}

    //// indexing sheets
    //for (int i = 0; i < proteinData.betaSheets.size(); ++i)
    //{
    //    Sheet sheet = proteinData.betaSheets[i];
    //    uint index = sheet.chainID - 'A'; // ending chain is always the same as the starting chain, so you don't have to bother about ending
    //    m_helixSheetChainIndex[index].push_back(std::make_pair(SHEET, sheet.residueSeqNumber));
    //}

    //// sort index
    //for (auto& chainStructures : m_helixSheetChainIndex)
    //{
    //    std::sort(chainStructures.begin(), chainStructures.end(), [](const std::pair<uint, uint>& a, const std::pair<uint, uint>& b) { return a.second < b.second; });
    //}

    //// get residue sequence number base for every chain so it could be index easier
    //Model model = m_proteinData.models[0];
    //residueBase.reserve(model.size());
    //for (const Chain& chain : model)
    //{
    //    Atom firstAtomInChain = chain[0];
    //    residueBase.push_back(firstAtomInChain.residueSeqNumber);
    //}
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
    case BuildType::TERTIARY:
        constructTertiary();
        break;
    }

    centralizePoints(m_vertices);
}

//
//void ProteinBuilder::constructWireframe()
//{
//    const Model& model = m_proteinData.models[0]; // todo: choose which model
//
//    for (int i = 0; i < model.size(); ++i)
//    {   
//        std::vector<Vec3> basePoints = alphaCarbons(model[i]);
//
//        std::vector<Vertex> wireframeVertices;
//        std::vector<uint> wireframeIndices;
//
//        if (m_buildType == BuildType::WIREFRAME)
//        {
//            m_tubeBuilder.buildWireframe(basePoints, wireframeVertices, wireframeIndices);
//        }
//        else if (m_buildType == BuildType::CURVEDWIREFRAME)
//        {
//            Vec3 p0 = getPreviousPoint(basePoints, 0);
//            Vec3 pn = getNextPoint(basePoints, basePoints.size() - 1);
//            std::vector<Vec3> extendedPoints = generateExtendedPoints(p0, basePoints, pn);
//            m_tubeBuilder.buildWireframe(extendedPoints, wireframeVertices, wireframeIndices);
//        }
//
//        bufferCombinder(wireframeVertices, wireframeIndices);
//    }
//}


void ProteinBuilder::constructTertiary()
{
    const Model& model = m_proteinData.models[0];

    // for every chain
    for (int chainIdx = 0; chainIdx < model.size(); ++chainIdx)
    {
        Chain chain = model[chainIdx];

        std::vector<Vec3> basePoints = alphaCarbons(chain);

        std::vector<Vertex> tempVertexBuffer;
        std::vector<uint> tempIndexBuffer;

        uint baseIdx = chain[0].residueSeqNumber;
        std::vector<std::pair<uint, uint>> begginingAndEndings;

        if (m_buildType == BuildType::TERTIARY)
        {
            // construct all alpha helicis for current chain
            for (const Helix& helix : m_proteinData.alphaHelicis)
            {
                tempVertexBuffer.clear();
                tempIndexBuffer.clear();

                if (helix.chainID != 'A' + chainIdx) continue;

                uint helixStart = helix.residueSeqNumber - baseIdx;
                uint helixEnd = helix.residueSeqNumber2 - baseIdx;

                assert(helixEnd - helixStart + 1 == helix.length);

                Vec3 p0 = getPreviousPoint(basePoints, helixStart);
                Vec3 pn = getNextPoint(basePoints, helixEnd);
                std::vector<Vec3> subPoints(basePoints.begin() + helixStart, basePoints.begin() + helixEnd + 1);

                m_helixBuilder.buildRibbon(p0, subPoints, pn, tempVertexBuffer, tempIndexBuffer, false);

                bufferCombinder(tempVertexBuffer, tempIndexBuffer);
                begginingAndEndings.emplace_back(std::make_pair(helixStart, helixEnd));
            }

            // construct all beta sheets for current chain
            for (const Sheet& sheet : m_proteinData.betaSheets)
            {
                tempVertexBuffer.clear();
                tempIndexBuffer.clear();

                if (sheet.chainID != 'A' + chainIdx) continue;

                uint sheetStart = sheet.residueSeqNumber - baseIdx;
                uint sheetEnd = sheet.residueSeqNumber2 - baseIdx;

                Vec3 p0 = getPreviousPoint(basePoints, sheetStart);
                Vec3 pn = getNextPoint(basePoints, sheetEnd);
                std::vector<Vec3> subPoints(basePoints.begin() + sheetStart, basePoints.begin() + sheetEnd + 1);

                m_helixBuilder.color = { 1.0f, 1.0f, 0.2f };
                m_helixBuilder.buildRibbon(p0, subPoints, pn, tempVertexBuffer, tempIndexBuffer, true);

                bufferCombinder(tempVertexBuffer, tempIndexBuffer);
                begginingAndEndings.emplace_back(std::make_pair(sheetStart, sheetEnd));
            }
        }

        // construct the rest of the wireframe
        {
            sort(begginingAndEndings.begin(), begginingAndEndings.end());
            uint startIdx = 0;
            for (int i = 0; i < begginingAndEndings.size(); ++i)
            {
                tempVertexBuffer.clear();
                tempIndexBuffer.clear();

                uint constructStart = begginingAndEndings[i].first;
                uint constructEnd = begginingAndEndings[i].second;

                if (startIdx == constructStart)
                {
                    startIdx = constructEnd;
                    continue;
                }

                std::vector<Vec3> subPoints(basePoints.begin() + startIdx, basePoints.begin() + constructStart + 1);
                Vec3 p0 = getPreviousPoint(basePoints, startIdx);
                Vec3 pn = getNextPoint(basePoints, constructStart);
                m_tubeBuilder.buildWireframe(generateExtendedPoints(p0, subPoints, pn) , tempVertexBuffer, tempIndexBuffer);

                bufferCombinder(tempVertexBuffer, tempIndexBuffer);

                startIdx = constructEnd;
            }
            // construct last wireframe peace

            tempVertexBuffer.clear();
            tempIndexBuffer.clear();

            uint endIdx = basePoints.size() - 1;
            std::vector<Vec3> subPoints(basePoints.begin() + startIdx, basePoints.begin() + endIdx);
            Vec3 p0 = getPreviousPoint(basePoints, startIdx);
            Vec3 pn = getNextPoint(basePoints, endIdx);
            m_tubeBuilder.buildWireframe(generateExtendedPoints(p0, subPoints, pn), tempVertexBuffer, tempIndexBuffer);
            bufferCombinder(tempVertexBuffer, tempIndexBuffer);
        }
    }
}


void ProteinBuilder::bufferCombinder(const std::vector<Vertex>& srcVertex, const std::vector<uint>& srcIndex)
{
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


Chain ProteinBuilder::alphaCarbons(const Chain& chain) const
{
    Chain alphaC;
    alphaC.reserve(128);

    for (const Atom& atom : chain)
    {
        if (strcmp("CA", atom.name) == 0)
        {
            alphaC.push_back(atom);
        }
    }

    return alphaC;
}


std::vector<Vec3> ProteinBuilder::generateExtendedPoints(Vec3 pBefore, const std::vector<Vec3>& basePoints, Vec3 pAfter) const
{
    if (basePoints.empty()) return std::vector<Vec3>();

    std::vector<Vec3> extendedPoints;
    uint n = basePoints.size();
    extendedPoints.reserve(n * partsPerCurveSegment);

    Vec3 p0, p1, p2, p3;
    for (uint i = 0; i < n - 1; ++i)
    {
        p0 = (i == 0) ? pBefore : basePoints[i - 1];
        p1 = basePoints[i];
        p2 = basePoints[i + 1];
        p3 = (i == n - 2) ? pAfter : basePoints[i + 2];

        for (int j = 0; j < partsPerCurveSegment; ++j)
        {
            float t = (float)j / float(partsPerCurveSegment);
            Vec3 point = Curve::catmullRom(t, curveTension, p0, p1, p2, p3);
            Vec3 tangent = Curve::catmullRomTangent(t, curveTension, p0, p1, p2, p3);

            extendedPoints.push_back(point);
        }
    }

    // add last point
    extendedPoints.push_back(basePoints[n - 1]);

    return extendedPoints;
}


void ProteinBuilder::centralizePoints(std::vector<Vertex>& points) const
{
    assert(points.size() > 0);

    Vec3 minValue;
    Vec3 maxValue;

    minValue = maxValue = points[0].position;

    for (auto vertex : points)
    {
        Vec3 p = vertex.position;

        if (p.x < minValue.x) minValue.x = p.x;
        if (p.y < minValue.y) minValue.y = p.y;
        if (p.z < minValue.z) minValue.z = p.z;

        if (p.x > maxValue.x) maxValue.x = p.x;
        if (p.y > maxValue.y) maxValue.y = p.y;
        if (p.z > maxValue.z) maxValue.z = p.z;
    }

    Vec3 midValue = (minValue + maxValue) / 2.0f;

    for (auto& p : points)
    {
        p.position = p.position - midValue;
    }

    return;
}


Vec3 ProteinBuilder::getPreviousPoint(const std::vector<Vec3>& points, uint index)
{
    assert(points.size() > index);

    if (index > 0)
    {
        return points[index - 1];
    }
    else
    {
        return points[0] * 2 - points[1];
    }
}


Vec3 ProteinBuilder::getNextPoint(const std::vector<Vec3>& points, uint index)
{
    assert(points.size() > index);

    if (index < points.size() - 1)
    {
        return points[index + 1];
    }
    else
    {
        return points[index] * 2 - points[index - 1];
    }
}


int residueIndex(const std::vector<Vec3>& points, uint residueSeqNumber)
{
    int lo = 0; 
    int hi = points.size();

    while (lo < hi)
    {
        if (lo == hi && points[lo])
    }


    return -1;
}

