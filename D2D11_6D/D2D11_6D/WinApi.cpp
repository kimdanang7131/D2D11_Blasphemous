#include <Windows.h>

namespace Pipeline
{
	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter); // 함수 선언
}

// WinMain 만들기 ( HINSTANCE ==  윈도우즈 운영체제에서 실행되는 프로그램들을 구별하기 위한 ID값==정수 , 지금 안씀 , CHAR* , 시작시 윈도우의 창 형태)
int APIENTRY WinMain(_In_ HINSTANCE const hInstance, _In_opt_  HINSTANCE const prevhInstance, _In_ LPSTR const lpCmdLine, _In_ int const nCmdShow)
{
	HWND hWnd = nullptr; // 메세지를 받을 윈도우 핸들이다.

	{
		WNDCLASSEX wndClass = WNDCLASSEX(); // 윈도우클래스 default로 생성

		wndClass.cbSize = sizeof(WNDCLASSEX); // cb == count of bytes , WNDCLASSEX의 바이트수 받음
		wndClass.lpfnWndProc = Pipeline::Procedure;     // 메세지 처리 함수 (윈도우 프로시저에서 처리)
		wndClass.hInstance = hInstance;       // HINSTANCE(ID값 정수 받아오기)
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // 윈도우 창 왼쪽위 아이콘  -> 사용자가 만든 아이콘이면 NULL 을 지우고 hInstance 
		wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // 작업표시줄 쪽 아이콘	      -> 사용자가 만든 아이콘이면 NULL 을 지우고 hInstance
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);     // 커서 아이콘 설정	          -> 사용자가 만든 아이콘이면 NULL 을 지우고 hInstance
		wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // Stock == 재고품,저장품  , GetStockObject == Stock Object의 핸들값을 얻을 때 사용 
		wndClass.lpszClassName = "Window"; // 클래스 이름지정
		wndClass.style = CS_VREDRAW | CS_HREDRAW; // 윈도우 창크기가 가로세로로 변경될때마다 윈도우창 다시 그려줌

		RegisterClassEx(&wndClass); // 윈도우클래스를 레지스터에 등록
	}

	{
		CREATESTRUCT window = CREATESTRUCT(); // 윈도우 구조체 만들기
		window.lpszClass = "Window";   // 윈도우 클래스 이름
		window.lpszName = "Game";      // 윈도우 창(타이틀바쪽) 이름
		window.style = WS_CAPTION | WS_SYSMENU; // CAPTION ==  타이틀 바를 가진 윈도우를 만들며 WS_BORDER 스타일을 포함한다 , SYSMENU == 시스템메뉴를 가진다 오른쪽위 [ X ] 
		window.cx = 500;               // 윈도우 창 크기  X
		window.cy = 500;			   // 윈도우 창 크기  Y
		window.hInstance = hInstance;  // HINSTANCE ( ID값 정수 받아오기 )

		{
			RECT rect{ 0 , 0 ,window.cx , window.cy }; // 창크기를 위한 RECT 구조체 { LEFT , TOP , RIGHT ,BOTTOM }

			AdjustWindowRectEx(&rect, window.style, NULL, window.dwExStyle); // Adjust == 조정하다 , 윈도우 창크기 조정(&rect , 윈도우 스타일 ,  메뉴 , ex스타일)
			window.cx = rect.right - rect.left; // 윈도우창 width 값
			window.cy = rect.bottom - rect.top; // 윈도우창 height 값

																		// GetSystemMetrics -> 매개변수로 전달된 값에 해당하는 시스템 설정 정보 반환
			window.x = (GetSystemMetrics(SM_CXSCREEN) - window.cx) / 2; // SM_CXSCREEN-> 현재 화면 해상도 X축 크기(PIXEL 단위)
			window.y = (GetSystemMetrics(SM_CYSCREEN) - window.cy) / 2; // SM_CYSCREEN-> 현재 화면 해상도 Y축 크기(PIXEL 단위)

		}

		hWnd = CreateWindowEx // 윈도우나 컨트롤을 생성하는 함수
		(
			window.dwExStyle,
			window.lpszClass,
			window.lpszName,
			window.style,
			window.x,
			window.y,
			window.cx,
			window.cy,
			window.hwndParent,
			window.hMenu,
			window.hInstance,
			window.lpCreateParams
		);

		ShowWindow(hWnd, SW_RESTORE); // SW_RESTORE 창위치 최소화->다시 켰을때 이전 속성 그대로유지
	}

	MSG msg;

	while (true)
	{																// GETMESSAGE는 메세지를 받지 전에 절대로 리턴하지 않고 대기(DEAD TIME이라고함)하고 있기 때문에 CPU가 노는 시간이 많다.
		if (PeekMessage(&msg, HWND(), WM_NULL, WM_NULL, PM_REMOVE)) // PEEK == 몰래엿보다,  PEEKMESSAGE는 메세지 큐에서 메세지를 꺼내거나 검사하되 메세지가 없더라도 즉각 리턴한다. 반환형 BOOL
		{
			if (msg.message == WM_QUIT) // WM_QUIT == 프로그램이 끝날때 발생하는 메시지
				return static_cast<int>(msg.wParam);

			DispatchMessage(&msg); // 이 함수에 의해 메세지 처리함수( 윈도우 프로시저 )에 전달됨
		}

		else
		{
			SendMessage(hWnd, WM_APP, NULL, NULL); // WM_APP == 사용자 정의 메세지 , 사용자가 보내주지 않는 이상 절대로 답변하지 않는 메시지
		}
	}
}
