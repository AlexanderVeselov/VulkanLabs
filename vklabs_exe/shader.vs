struct VS_INPUT
{
    float3 position : POSITION0;
    float3 color    : COLOR0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 color : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.position = float4(input.position, 1.0);
    output.color = input.color;

    return output;
}
