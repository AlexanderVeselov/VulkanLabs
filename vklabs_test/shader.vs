struct VS_INPUT
{
//    float3 position : POSITION0;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

VS_OUTPUT main(uint vertex_id : SV_VERTEXID)//VS_INPUT input)
{
    VS_OUTPUT output;
    output.texcoord = float2(vertex_id & 2, (vertex_id << 1) & 2);
    output.position = float4(output.texcoord * 2.0f - 1.0f, 0.0f, 1.0f);

    return output;
}
