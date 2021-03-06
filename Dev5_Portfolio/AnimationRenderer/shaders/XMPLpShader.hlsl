struct OutputVertex
{
	float4 xyzw : SV_POSITION;
	float4 rgba : OCOLOR;
};

float4 main(OutputVertex inputPixel) : SV_TARGET
{
	return inputPixel.rgba;
}