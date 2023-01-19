#include <Windows.h>

namespace Pipeline
{
	LRESULT CALLBACK Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter); // �Լ� ����
}

// WinMain ����� ( HINSTANCE ==  �������� �ü������ ����Ǵ� ���α׷����� �����ϱ� ���� ID��==���� , ���� �Ⱦ� , CHAR* , ���۽� �������� â ����)
int APIENTRY WinMain(_In_ HINSTANCE const hInstance, _In_opt_  HINSTANCE const prevhInstance, _In_ LPSTR const lpCmdLine, _In_ int const nCmdShow)
{
	HWND hWnd = nullptr; // �޼����� ���� ������ �ڵ��̴�.

	{
		WNDCLASSEX wndClass = WNDCLASSEX(); // ������Ŭ���� default�� ����

		wndClass.cbSize = sizeof(WNDCLASSEX); // cb == count of bytes , WNDCLASSEX�� ����Ʈ�� ����
		wndClass.lpfnWndProc = Pipeline::Procedure;     // �޼��� ó�� �Լ� (������ ���ν������� ó��)
		wndClass.hInstance = hInstance;       // HINSTANCE(ID�� ���� �޾ƿ���)
		wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);   // ������ â ������ ������  -> ����ڰ� ���� �������̸� NULL �� ����� hInstance 
		wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // �۾�ǥ���� �� ������	      -> ����ڰ� ���� �������̸� NULL �� ����� hInstance
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);     // Ŀ�� ������ ����	          -> ����ڰ� ���� �������̸� NULL �� ����� hInstance
		wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)); // Stock == ���ǰ,����ǰ  , GetStockObject == Stock Object�� �ڵ鰪�� ���� �� ��� 
		wndClass.lpszClassName = "Window"; // Ŭ���� �̸�����
		wndClass.style = CS_VREDRAW | CS_HREDRAW; // ������ âũ�Ⱑ ���μ��η� ����ɶ����� ������â �ٽ� �׷���

		RegisterClassEx(&wndClass); // ������Ŭ������ �������Ϳ� ���
	}

	{
		CREATESTRUCT window = CREATESTRUCT(); // ������ ����ü �����
		window.lpszClass = "Window";   // ������ Ŭ���� �̸�
		window.lpszName = "Game";      // ������ â(Ÿ��Ʋ����) �̸�
		window.style = WS_CAPTION | WS_SYSMENU; // CAPTION ==  Ÿ��Ʋ �ٸ� ���� �����츦 ����� WS_BORDER ��Ÿ���� �����Ѵ� , SYSMENU == �ý��۸޴��� ������ �������� [ X ] 
		window.cx = 500;               // ������ â ũ��  X
		window.cy = 500;			   // ������ â ũ��  Y
		window.hInstance = hInstance;  // HINSTANCE ( ID�� ���� �޾ƿ��� )

		{
			RECT rect{ 0 , 0 ,window.cx , window.cy }; // âũ�⸦ ���� RECT ����ü { LEFT , TOP , RIGHT ,BOTTOM }

			AdjustWindowRectEx(&rect, window.style, NULL, window.dwExStyle); // Adjust == �����ϴ� , ������ âũ�� ����(&rect , ������ ��Ÿ�� ,  �޴� , ex��Ÿ��)
			window.cx = rect.right - rect.left; // ������â width ��
			window.cy = rect.bottom - rect.top; // ������â height ��

																		// GetSystemMetrics -> �Ű������� ���޵� ���� �ش��ϴ� �ý��� ���� ���� ��ȯ
			window.x = (GetSystemMetrics(SM_CXSCREEN) - window.cx) / 2; // SM_CXSCREEN-> ���� ȭ�� �ػ� X�� ũ��(PIXEL ����)
			window.y = (GetSystemMetrics(SM_CYSCREEN) - window.cy) / 2; // SM_CYSCREEN-> ���� ȭ�� �ػ� Y�� ũ��(PIXEL ����)

		}

		hWnd = CreateWindowEx // �����쳪 ��Ʈ���� �����ϴ� �Լ�
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

		ShowWindow(hWnd, SW_RESTORE); // SW_RESTORE â��ġ �ּ�ȭ->�ٽ� ������ ���� �Ӽ� �״������
	}

	MSG msg;

	while (true)
	{																// GETMESSAGE�� �޼����� ���� ���� ����� �������� �ʰ� ���(DEAD TIME�̶����)�ϰ� �ֱ� ������ CPU�� ��� �ð��� ����.
		if (PeekMessage(&msg, HWND(), WM_NULL, WM_NULL, PM_REMOVE)) // PEEK == ����������,  PEEKMESSAGE�� �޼��� ť���� �޼����� �����ų� �˻��ϵ� �޼����� ������ �ﰢ �����Ѵ�. ��ȯ�� BOOL
		{
			if (msg.message == WM_QUIT) // WM_QUIT == ���α׷��� ������ �߻��ϴ� �޽���
				return static_cast<int>(msg.wParam);

			DispatchMessage(&msg); // �� �Լ��� ���� �޼��� ó���Լ�( ������ ���ν��� )�� ���޵�
		}

		else
		{
			SendMessage(hWnd, WM_APP, NULL, NULL); // WM_APP == ����� ���� �޼��� , ����ڰ� �������� �ʴ� �̻� ����� �亯���� �ʴ� �޽���
		}
	}
}
