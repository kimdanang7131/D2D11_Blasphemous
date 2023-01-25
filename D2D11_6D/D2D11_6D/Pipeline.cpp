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
		ID3D11RenderTargetView* RenderTargetView; // 만들어놓은 윈도우 창 위에 띄워주는 실제 게임창?인듯


		namespace Buffer
		{
			ID3D11Buffer* Vertex; // GPU에서 사용할 데이터
		}
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
					nullptr, // adapter 그래픽카드 연결해주는곳 2D는 안쓰고 3D에서 사용 , nullptr == 내장그래픽으로 사용
					D3D_DRIVER_TYPE_HARDWARE, // 디바이스 ,디바이스 컨텍스트가 누구로부터 만들어주나?
					nullptr,
					0,
					nullptr,
					0,
					D3D11_SDK_VERSION, // 보내줘야 값을 검사해서 검사용도로 써줘야댐
					&descriptor,
					&SwapChain,
					&Device,
					nullptr,
					&DeviceContext
				));
			}

			{
				struct Vertex
				{
					float Position[4];
					float    Color[4];
				};

				Vertex const Vertecies[4] =
				{
					{ { -0.5f , 0.5f, 0.0f , 1.0f }, { 1.0f , 0.0f, 0.0f , 1.0f } },
					{ {  0.5f , 0.5f, 0.0f , 1.0f }, { 0.0f , 1.0f, 0.0f , 1.0f } },
					{ { -0.5f ,-0.5f, 0.0f , 1.0f }, { 0.0f , 0.0f, 1.0f , 1.0f } },
					{ {  0.5f ,-0.5f, 0.0f , 1.0f }, { 1.0f , 1.0f, 1.0f , 1.0f } }  // 좌표계 반시계 안그려줌
				};
				// CPU에서 읽을 수 있는 데이터

				D3D11_BUFFER_DESC descriptor = D3D11_BUFFER_DESC(); 
				descriptor.ByteWidth = sizeof(Vertecies); // 공간설정
				descriptor.Usage = D3D11_USAGE_IMMUTABLE; // 활용 용도 결정 CPU에 대한 접근 자체를 막아줌 , GPU에서만 읽고 쓸 수 있는 데이터
				descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 해당 공간에 대한 식별자를 지정해줌
				descriptor.CPUAccessFlags = 0; // USAGE의 DYNAMIC == CPU 쓰기가능 읽기X , STAGING 반대 여기서 한번더 CPU 접근맞는지 검사

				D3D11_SUBRESOURCE_DATA subResource = D3D11_SUBRESOURCE_DATA(); // 버퍼라는 공간을 만들때 초깃값을 설정할 수 있게 도와주는 데이터( GPU관련이기때문에 여기서는 초기화할수없으므로)
				subResource.pSysMem = Vertecies; // 메모리의 존재하는 시스템메모리는 0과 1로 치환한 후에(비트로 치환되어있으면 어디에서나 공용어이기때문에 컴퓨터입장에서) 그 치환된 걸 그대로 넘겨줌 
												 // ★ 메모리 주소 자체를 넘겨줌 ★ -> 대신 CPU에서는 읽을 수 없는 메모리
				MUST(Device->CreateBuffer(&descriptor, &subResource, &Buffer::Vertex)); // &Buffer::Vertex -> GPU에서만 사용가능해서 여기서 설정하면 오류 -> 설정할 수 있는게 없다.
				const UINT Stride = sizeof(Vertex);  // stride 큰걸음 GPU에 좌표값을 비트로 치환해서 보내주긴 했는데, 얼만큼 끊어 읽어야 되는지 모름 , 그래서 어디서 끊어야 되는지 알려줌
				const UINT offset = 0; // 어디서부터 읽어야되나 -> 처음부터라서 0 (처음시작위치 알려주는중)
				DeviceContext->IASetVertexBuffers(0, 1, &Buffer::Vertex, &Stride, &offset); // set get 구분 버텍스 버퍼는 최대 32개 -> 0~31

				DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			}
			return 0;
		}
		case WM_SIZE: // 창의 크기가 변경될때 ,창 최대화할때 캐릭터 크기변경 이나 캐릭터
		{
			//렌더 타겟
			//디바이스에서 할 수 있다.
			//
			// 버퍼 관리 -> 스왑체인에서 관리하고있음
			ID3D11Texture2D* texture = nullptr; // 텍스쳐에 있는 아이디 식별번호 외우기 힘드니 따로 만들어준듯
			MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&texture))); //이중버퍼중 백버퍼 먼저쓰니 0 , 텍스쳐에 있는 아이디 식별번호를 파악하여 백버퍼에서 일하게
			{
				Device->CreateRenderTargetView(texture, nullptr, &RenderTargetView);
			}
			texture->Release(); // 계속 남아있지 않도록 해제

			DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr); // OM에서 출력하므로 OM사용 , 렌더타겟 1개사용하므로 1 ,  , 3D때배움 깊이자원

			return 0;
		}
		case WM_APP:
		{
			//초록 -> 검정 -> 초록 -> 검정
			MUST(SwapChain->Present(0, 0)); // 백버퍼가 그리고있는 완성된 걸 보내주는 구문 , first == 딜레이주는거

			static float a = 0.f;
			static float delta = 0.0001f;


			a += delta;

			if (a >= 1.0f || a <= 0.f)
			{
				delta *= -1;
			}


			float Color[4] = { 0.0f, a , 0.0f, 1.0f };


			DeviceContext->ClearRenderTargetView(RenderTargetView, Color); // 이 색상으로 계속 채워줌 [[안해주면 잔상처럼]]



			return 0;
		}
		case WM_DESTROY:
		{
			//창이 파괴되면 동적할당된 애들을 delete해주겠다.
			Device->Release();
			DeviceContext->Release();
			SwapChain->Release();

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