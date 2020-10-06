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

    float2 pos[3] = { float2(0.0, 0.5), float2(-0.5, -0.5), float2(0.5, -0.5) };

    output.texcoord = float2(vertex_id & 2, (vertex_id << 1) & 2);
    output.position = float4(pos[vertex_id], 0.0, 1.0);

    return output;
}
