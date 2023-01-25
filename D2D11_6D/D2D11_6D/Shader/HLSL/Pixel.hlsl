#include "Layout.hlsli"

namespace Shader
{
    Layout::Color Pixel(const Layout::Pixel Input) : SV_Target
    {
        Layout::Color Output = Input.Color;
		
        return Output;
    }
}