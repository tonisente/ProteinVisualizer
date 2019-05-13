////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
/////////////


cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
};

//////////////
// TYPEDEFS //
//////////////

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET
{
    float4 baseColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflectionVector;
    float4 specular;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location
    baseColor = input.color;

    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

    // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Invert the light direction for calculation
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
        // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
        color += saturate(diffuseColor * lightIntensity);

        // Calculate the amount of specular light based on the reflection vector, viewing direction and specular power
        reflectionVector = normalize(2 * lightIntensity * input.normal - lightDir);

        // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflectionVector, input.viewDirection)), specularPower);
    }
    else
    {
        //color = float4(1, 1, 1, 1);
    }

    // Multiply the texture pixel and the final diffuse color to get the final pixel color result.
    color = color * baseColor;

    // Add the specular component last to the output color.
    color = saturate(color + specular);

    return color;
}
