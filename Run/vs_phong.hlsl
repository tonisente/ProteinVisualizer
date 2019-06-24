////////////////////////////////////////////////////////////////////////////////
// Filename: vs_phong.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////

cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer
{
    float3 cameraPosition;
    float padding;
};

//////////////
// TYPEDEFS //
//////////////

struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    float4 worldPosition;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix); // todo: output.position
    output.position = mul(worldPosition, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Calculate the normal vector against the world matrix only
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix));

    // Determine the viewing direction base on the position of the camera and the position of the vertex in the world.
    output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);

	// Pass color to PS
    output.color = input.color;

    return output;
}







