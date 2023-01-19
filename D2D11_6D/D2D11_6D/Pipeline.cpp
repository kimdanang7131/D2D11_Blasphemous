#include <d3d11.h>
#include <cassert> // ��Ÿ�� ���߿� ������ �����Ͽ� ���α׷� ���Ľ�Ű�� ���̺귯��
//#pragma comment(lib, "d3d11.lib") // USB���� �ʹ� ���ſ��� .lib�� ���� ( ������ �����ϸ� �ȵǱ⵵ �ϰ�) -> �Ӽ� -> ��Ŀ -> ���ɼ� -> �߰����Ӽ� -> d3d11.lib; �ϰų� �̰�
														// ����� �ַ�ǿ� �ܺ� ���Ӽ��� �̹� ������ְ�, cpp����
#if not defined _DEBUG
#define MUST(Expression) (      (         (Expression)))
#else
#define MUST(Expression) (assert(SUCCEEDED(Expression)))
#endif


namespace Pipeline
{
	// Rendering Pipeline
	// ȭ�鿡 �׷����� �׷����� �ܰ踦 �ǹ��մϴ�. (�� 11����)
	//  
	// IA -> VS -> RS -> PS -> OM
	//

	namespace
	{
		ID3D11Device* Device;
		ID3D11DeviceContext* DeviceContext;
		IDXGISwapChain* SwapChain;
	}

	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
		case WM_CREATE: // �̹��� ���⿡ �־���
		{
			{
				DXGI_SWAP_CHAIN_DESC descriptor = DXGI_SWAP_CHAIN_DESC();

				//Descriptor.BufferDesc.Width                   = 500;  // ���۸� ��� ����ü���Ҳ��� , �� ������ ������ �����̳�
				//Descriptor.BufferDesc.Height                  = 500;  // ���ۿ� ���� �۾����� == �׷��ش�� ȭ�鿡 ���
																	  // ���ۻ������ ������ âũ��� ���ƾ���(������ �� ������ �����) , 0�� ���� ó�� ������ ������� width height ������
				//Descriptor.BufferDesc.RefreshRate.Numerator   = 0;    // RefreshRate == ���ŷ� -> ����
				//Descriptor.BufferDesc.RefreshRate.Denominator = 0;    // �и� -> 0���� �Ѵ� �����ϸ� �ڵ������� ->�и� 1�λ�����(���ڸ� 1���ϸ� ����)
				//Descriptor.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // ��� ������� ���� ������ PROGRESSIVE�� �� �˾Ƽ� �����̶�
				//Descriptor.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;        // WM_SIZE�� ����Ѵ���
				descriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;           // �ڡڡ� � �����͸� �����ϰ�, ��� �о�ߵǴ��� ����? UNSIGNED NORMALIZE = 1������ȭ�Ѵ�.
				descriptor.SampleDesc.Count = 1;          // ��� ��Ƽ�ٸ������ �����Ұų� ���ۻ��ø�/ -> ��Ƽ���ø� ��� �̿��Ұų�
				//Descriptor.SampleDesc.Quality                 = 0;	        // ���ø� ��?
				descriptor.OutputWindow = hWindow;                          // ��� ����Ұų�
				descriptor.Windowed = true;                             // â��� ����Ұų�
				descriptor.BufferCount = 1;                                // ���߹��۸� = 1 (�⺻������ ���۰� ���ԵǾ��ֱ� ������)
				descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // ������ ��� �뵵�� �����̳� , �ϼ��� ���� Ÿ���� ����ϴ� �뵵
				//Descriptor.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;         // �Ѱ��ִ� ������ ������ ���� �ʰڴ�. discard == ������
				//Descriptor.Flags        = 0;                                // �߰� �ɼ� ���� ( �߰� �ɼ� �Ѱ��� ���� ) �ΰ��ɼ��� �´µ�

				//HRESULT hr = D3D11CreateDeviceAndSwapChain(); // RESULT�� ���� HANDLE ��
				MUST(D3D11CreateDeviceAndSwapChain             // �����ϸ� �Ѱ��ְ� �����ϸ� ��Ÿ�ӵ��� ���� , releas�� �ϸ� ��ũ�ζ����� 0���� �־���
				(
					nullptr, // adpater �׷���ī�� �������ִ°� 2D�� �Ⱦ��� 3D���� ��� , nullptr == ����׷������� ���
					D3D_DRIVER_TYPE_HARDWARE, // ����̽� ,����̽� ���ؽ�Ʈ�� �����κ��� ������ֳ�?
					nullptr,
					0,
					nullptr,
					0,
					0,
					&descriptor,
					&SwapChain,
					&Device,
					nullptr,
					&DeviceContext
				));
			}
			return 0;
		}
		case WM_SIZE: // â�� ũ�Ⱑ ����ɶ� ,â �ִ�ȭ�Ҷ� ĳ���� ũ�⺯�� �̳� ĳ����
		{
			return 0;
		}
		case WM_APP:
		{
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return DefWindowProc(hWindow, uMessage, wParameter, lParameter);
		}
		}
		// GIt Kraken Test//
		//
	}
}