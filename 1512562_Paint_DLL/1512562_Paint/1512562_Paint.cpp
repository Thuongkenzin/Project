// 1512562_Paint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512562_Paint.h"
#include"Shape.h"
#include <vector>
using namespace std;
#include <windowsx.h>
#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")
#include "RibbonFramework.h"
#include "RibbonIDs.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	CoInitialize(NULL);
    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1512562PAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512562PAINT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	CoUninitialize();
    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          =0;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512562PAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    //ex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1512562PAINT);
	wcex.lpszMenuName = 0;

    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiPlusToken;
vector<Shape*> image;	//Lưu vào mảng những đường thẳng đã vẽ
bool LeftClick = false;			//Biến dùng báo hiệu đã nhấn chuột trái
Point Point_Start;					//Lưu điểm bất đầu
Point Point_End;					//Lưu điểm ketthuc
Point Point_Move;		// trong quá trình duy chuyển thì sẽ lưu điểm
Shape *ObjectPaint = NULL;			// đối tượng vẽ trong quá trình vẽ khi di chuyển chuột.

int Image_Paint = 0; //bien cho biet doi tuong ve la gi tu menu nguoi dung chon
					//0-Line, 1-Rectangle,2-Ellipse,3-Square

Graphics *graphics = NULL;
void Square()
{
	if (abs(Point_Start.X - Point_End.X) > abs(Point_Start.Y - Point_End.Y))
	{
		if (Point_Start.X > Point_End.X)
			Point_End.X = Point_Start.X - abs(Point_Start.Y - Point_End.Y);
		else
			Point_End.X = Point_Start.X + abs(Point_Start.Y - Point_End.Y);
	}
	else
	{
		if (Point_Start.Y > Point_End.Y)
			Point_End.Y = Point_Start.Y - abs(Point_Start.X - Point_End.X);
		else
			Point_End.Y = Point_Start.Y + abs(Point_Start.X - Point_End.X);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	static int i = 0;
	PAINTSTRUCT ps;
	HDC hdc;
    switch (message)
    {
	case WM_CREATE:
		InitializeFramework(hWnd);
		GdiplusStartup(&gdiPlusToken, &gdiplusStartupInput, NULL);
		break;
	case WM_LBUTTONDOWN:
	{
		Point_Start.X = GET_X_LPARAM(lParam);
		Point_Start.Y = GET_Y_LPARAM(lParam);
		LeftClick = true;
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (LeftClick == true)
		{
			Point_Move.X = GET_X_LPARAM(lParam); //luu lai toa do di chuyen cua con chuot
			Point_Move.Y = GET_Y_LPARAM(lParam);
			InvalidateRect(hWnd, NULL, TRUE); //goi ham WM_PAINT de ve
		}
		break;
	}
	case WM_LBUTTONUP:
	{
		
		Point_End.X = GET_X_LPARAM(lParam);
		Point_End.Y = GET_Y_LPARAM(lParam);
		
		if (Image_Paint == 0)
		{
			ObjectPaint = new Line(Point_Start, Point_End);
		}
		else if(Image_Paint == 1)
		{
			
			ObjectPaint = new CRect(Point_Start, Point_End);
		}
		else if (Image_Paint == 2)
		{
			ObjectPaint = new CElip(Point_Start, Point_End);
		}
		else
		{
			Square();
			ObjectPaint = new CRect(Point_Start, Point_End);
		}
		LeftClick = false;
		image.push_back(ObjectPaint);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_MENU_NEW:
				image.clear();
				break;
			case ID_MENU_EXIT:
				DestroyWindow(hWnd);
				break;
			case ID_SHAPES_RECT :
			{
				Image_Paint = 1;
				break;
			}
			break;
			case ID_SHAPES_LINE:
			{
				Image_Paint = 0;
				break;
			}
			case ID_SHAPES_ELIP:
			{
				Image_Paint = 2;
				break;
			}
			case ID_SHAPES_CIRCLE:
			{
				Image_Paint = 2;
				break;
			}
			case ID_SHAPES_SQUARE:
			{
				Image_Paint = 3;
				break;
			}
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
		
		hdc = BeginPaint(hWnd, &ps);
		RECT rect;
		GetClientRect(hWnd, &rect);
		graphics = new Graphics(hdc);
		
		for (int i = 0; i < image.size(); i++)
		{
			image[i]->Draw(graphics);
		}
		//ve hinh ao cho nguoi dung thay hinh dang hinh dang muon ve
		if (LeftClick)
		{
			
			if (Image_Paint == 0)
			{
				ObjectPaint = new Line(Point_Start, Point_Move);
			}
			else if (Image_Paint == 1)
			{
				ObjectPaint = new CRect(Point_Start, Point_Move);
			}
			else if (Image_Paint == 2)
			{
				ObjectPaint = new CElip(Point_Start, Point_Move);
			}
			else {
				Square();
				ObjectPaint = new CRect(Point_Start, Point_Move);
			}
			ObjectPaint->Draw(graphics);
		}
		
		
        EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		GdiplusShutdown(gdiPlusToken);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
