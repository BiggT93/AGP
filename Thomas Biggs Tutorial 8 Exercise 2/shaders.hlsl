Texture2D texture0;
SamplerState sampler0;

struct VOut
{
	float4 position : SV_POSITION;
	float4 color	: COLOR;
	float2 texcoord : TEXCOORD;
};

cbuffer CBuffer0
{
	matrix WVPMatrix;	 // 64bytes
	float scale;		 //4 bytes
	float red_fraction;  // 4 bytes
	float3 packing;		 // 3x4 bytes
	
};
VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
	VOut output;

	color.r *= red_fraction;
	output.position = mul(WVPMatrix, position);
	
	//output.position.x -= scale;
	output.color = color;
	output.texcoord = texcoord;
	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord2 : TEXCOORD) : SV_TARGET
{
	//texcoord.x = 0.9;
	//texcoord.y = 0.1;
	//return color * texture0.Sample(sampler0,texcoord);
	return texture0.Sample(sampler0, texcoord2);
    //return float4(1,0,0,0);
}