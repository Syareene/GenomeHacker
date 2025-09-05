
#include "common.hlsl"


void main(in DEBUG_VSIN In, out DEBUG_PSIN Out)
{

	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	Out.Position = mul(In.Position, wvp);
}

