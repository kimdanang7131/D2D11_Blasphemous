// 확장 -> 확장관리 -> hlsl -> hlsl tools for visual studio 적용 하면 다운로드
#include "LayOut.hlsli"

namespace Shader
{
    Layout::Pixel Vertex(const Layout::Vertex Input)
    {
        Layout::Pixel Output =
        {
            Input.Position,
			Input.Color
        };
		
        return Output;
    }

}