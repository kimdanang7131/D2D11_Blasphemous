// Ȯ�� -> Ȯ����� -> hlsl -> hlsl tools for visual studio ���� �ϸ� �ٿ�ε�
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