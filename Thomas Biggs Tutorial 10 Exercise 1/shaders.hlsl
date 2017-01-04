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
	float4 directional_light_vector;
	float4 directional_light_colour;
	float4 ambient_light_colour;
	//float scale;		 //4 bytes
	//float red_fraction;  // 4 bytes
	//float3 packing;		 // 3x4 bytes
	
};
VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	//color.r *= red_fraction;
	output.position = mul(WVPMatrix, position);
	
	float diffuse_amount = dot(directional_light_vector, normal);

	diffuse_amount = saturate(diffuse_amount);
	output.color = ambient_light_colour + (directional_light_colour + diffuse_amount);


	//output.position.x -= scale;
	//output.color = color;

	output.texcoord = texcoord;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord2 : TEXCOORD) : SV_TARGET
{
	//texcoord.x = 0.9;
	//texcoord.y = 0.1;
	//return color * texture0.Sample(sampler0,texcoord);
	return  color * texture0.Sample(sampler0, texcoord2);
    //return float4(1,0,0,0);
}