float4x4 World;
float4x4 View;
float4x4 Projection;
float4x4 WorldInverseTranspose;

float4 AmbientColor = float4(1, 1, 1, 1);
float AmbientIntensity = 0.1;

float3 DiffuseLightDirection = float3(1, 0, 0);
float4 DiffuseColor = float4(1, 1, 1, 1);
float DiffuseIntensity = 1.0;

float Shininess = 200;
float4 SpecularColor = float4(1, 1, 1, 1);    
float SpecularIntensity = 1;
float3 ViewVector = float3(1, 0, 0);

texture ModelTexture;
sampler2D textureSampler = sampler_state {
    Texture = (ModelTexture);
    MagFilter = Linear;
    MinFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;       
};

/*
The first line will be a reference to the texture we are going to use for our model. The rest of this is what is 
called a sampler. A sampler is a way for the graphics card to determine exactly how it should extract the texture 
color from the texture. The body of the sampler lists five properties of the sampler that tell it how to extract 
this information. The first one, Texture = (ModelTexture);, says which texture to use, which we set to the texture 
we want. The next two properties, MagFilter and MinFilter, tell us how to handle the situation when the texture is 
magnified or "minified". For instance, if the texture is sort of stretched out across the model, there will be times 
when we need texture coordinates that are between pixels, and the sampler needs to know how to interpolate between 
the nearest actual pixels. Linear is a pretty decent filter, which interpolates between the nearest colors. There are 
others though, including None, Point, and Anisotropic.

The last two properties here, AddressU and AddressV deal with how the sampler should respond if it gets a value that 
is beyond the normal range of 0-1. In almost all models, this isn't going to happen. But the sampler should know how 
to handle it in case it comes up, because this could happen accidentally, or even intentionally. In our case, we are 
going to use Clamp, which says that if the value is less than 0, to just use the value at 0 instead, and if the value 
is more than one, to just use the texture color at 1. Once again, there are other choices that could be used. The value 
Border means that if a value beyond 0 or 1 is used, the "border color" is used instead, which is often just black. Wrap
means that if the value goes over 1, it should just start over and repeat at 0. So a value of 1.25 will have the same 
value as 0.25. The same is true if you go below 0. Also, there is a Mirror value, which means that if you go beyond 1, 
it starts going back the other direction instead. So 1.1 is mapped to 0.9. Note that you can have different values for 
the u-coordinate and the v-coordinate.
*/

struct VertexShaderInput
{
    float4 Position : POSITION0; // Per fare Ambient
    float4 Normal : NORMAL0; // Per fare Diffuse
    float2 TextureCoordinate : TEXCOORD0; // Texturized
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float3 Normal : TEXCOORD0; // Per fare Specular    
    float2 TextureCoordinate : TEXCOORD1; // Texturized    
};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);

    float4 normal = mul(input.Normal, WorldInverseTranspose);
    float lightIntensity = dot(normal, DiffuseLightDirection);
    output.Color = saturate(DiffuseColor * DiffuseIntensity * lightIntensity);
    
    output.Normal = normal;
    
    output.TextureCoordinate = input.TextureCoordinate;

    return output;
}

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
    //Tutto per il calcolo di "specular". Per le altre due basta l'ultima riga con specular = 0    
    float3 light = normalize(DiffuseLightDirection);
    float3 normal = normalize(input.Normal);
    float3 r = normalize(2 * dot(light, normal) * normal - light);
    float3 v = normalize(mul(normalize(ViewVector), World));
    float dotProduct = dot(r, v);
 
    float4 specular = SpecularIntensity * SpecularColor * max(pow(dotProduct, Shininess), 0) * length(input.Color);
 
    //prendo il colore della texture alle coordinate (me lo dà il sampler)
    float4 textureColor = tex2D(textureSampler, input.TextureCoordinate);
    //fisso il fattore alpha a 1
    textureColor.a = 1;
 
    return saturate(textureColor * (input.Color) + AmbientColor * AmbientIntensity + specular);
}

technique Texturized
{
    pass Pass1
    {
        VertexShader = compile vs_1_1 VertexShaderFunction();
        PixelShader = compile ps_2_0 PixelShaderFunction();
    }
}
