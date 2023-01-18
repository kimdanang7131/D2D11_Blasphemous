#include <d3d11.h>
#include <cassert> // 런타임 도중에 에러를 검출하여 프로그램 폭파시키는 라이브러리
//#pragma comment(lib, "d3d11.lib") // USB느낌 너무 무거워서 .lib로 놓음 ( 누구나 접근하면 안되기도 하고) -> 속성 -> 링커 -> 모든옵션 -> 추가종속성 -> d3d11.lib; 하거나 이거
														// 헤더는 솔루션에 외부 종속성에 이미 선언되있고, cpp느낌
#if not defined _DEBUG
#define MUST(Expression) (      (         (Expression)))
#else
#define MUST(Expression) (assert(SUCCEEDED(Expression)))
#endif


namespace Pipeline
{
	// Rendering Pipeline
	// 화면에 그래픽이 그려지는 단계를 의미합니다. (총 11가지)
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
		case WM_CREATE: // 이미지 여기에 넣어줌
		{
			{
				DXGI_SWAP_CHAIN_DESC descriptor = DXGI_SWAP_CHAIN_DESC();

				//Descriptor.BufferDesc.Width                   = 500;  // 버퍼를 몇개를 스왑체인할꺼나 , 그 관리할 내용은 무엇이냐
				//Descriptor.BufferDesc.Height                  = 500;  // 버퍼에 관한 작업영역 == 그려준대로 화면에 출력
																	  // 버퍼사이즈는 무조건 창크기와 같아야함(작으면 그 영역만 적용됨) , 0값 들어가면 처음 설정한 사이즈로 width height 설정됨
				//Descriptor.BufferDesc.RefreshRate.Numerator   = 0;    // RefreshRate == 갱신률 -> 분자
				//Descriptor.BufferDesc.RefreshRate.Denominator = 0;    // 분모 -> 0으로 둘다 설정하면 자동프레임 ->분모를 1로생각함(분자를 1로하면 무한)
				//Descriptor.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 어떻게 출력할지 결정 어차피 PROGRESSIVE로 됨 알아서 설정이라
				//Descriptor.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;        // WM_SIZE와 비슷한느낌
				descriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;           // ★★★ 어떤 데이터를 관리하고, 어떻게 읽어야되는지 결정? UNSIGNED NORMALIZE = 1로정규화한다.
				descriptor.SampleDesc.Count = 1;          // 어떻게 안티앨리어싱을 결정할거냐 슈퍼샘플링/ -> 멀티샘플링 몇개로 이용할거냐
				//Descriptor.SampleDesc.Quality                 = 0;	        // 샘플링 질?
				descriptor.OutputWindow = hWindow;                          // 어디에 출력할거냐
				descriptor.Windowed = true;                             // 창모드 사용할거냐
				descriptor.BufferCount = 1;                                // 이중버퍼링 = 1 (기본적으로 버퍼가 포함되어있기 때문에)
				descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // 버퍼의 사용 용도가 무엇이냐 , 완성한 렌더 타겟을 출력하는 용도
				//Descriptor.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;         // 넘겨주는 정보를 가지고 있지 않겠다. discard == 버리다
				//Descriptor.Flags        = 0;                                // 추가 옵션 설정 ( 추가 옵션 한개만 가능 ) 부가옵션이 맞는듯

				//HRESULT hr = D3D11CreateDeviceAndSwapChain(); // RESULT에 대한 HANDLE 값
				MUST(D3D11CreateDeviceAndSwapChain             // 성공하면 넘겨주고 실패하면 런타임도중 폭파 , releas로 하면 매크로때문에 0으로 넣어짐
				(
					nullptr, // adpater 그래픽카드 연결해주는곳 2D는 안쓰고 3D에서 사용 , nullptr == 내장그래픽으로 사용
					D3D_DRIVER_TYPE_HARDWARE, // 디바이스 ,디바이스 컨텍스트가 누구로부터 만들어주나?
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
		case WM_SIZE: // 창의 크기가 변경될때 ,창 최대화할때 캐릭터 크기변경 이나 캐릭터
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
	}
}