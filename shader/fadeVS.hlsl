
#include "common.hlsl"


void main(in POS_VSIN In, out POS_PSIN Out)
{

    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Out.Position = mul(In.Position, wvp);
}

