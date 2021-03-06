// 1512562_QLCT.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512562_QLCT.h"
#include "CItem.h"
#include <string>
#include "Chart.h"
#include <CommCtrl.h>
#include <locale>
#include <vector>
#include <fstream>
#include <codecvt>
#include <windowsx.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
using namespace std;
#define MAX_LOADSTRING 100
#define COLOR_A			RGB(72,133,237)
#define COLOR_B			RGB(219,50,54)
#define COLOR_C			RGB(156,39,176)
#define COLOR_D			RGB(244,194,13)
#define COLOR_E			RGB(60,186,84)
#define COLOR_F			RGB(141,110,99)
#define DEFAULT_COLOR	RGB(255,255,255)

// Global Variables:
COLORREF color[7] = { COLOR_A, COLOR_B, COLOR_C, COLOR_D, COLOR_E, COLOR_F, DEFAULT_COLOR };
HINSTANCE hInst;                                // current instance
HWND ghWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR* types[] =
{
	L"Ăn uống", L"Di chuyển", L"Nhà cửa",
	L"Xe cộ", L"Nhu yếu phẩm", L"Dịch vụ",
};
long long g_totalMoney;
vector<CItem*> listItem;
int currX[7];
int currY;
long long totaltypeMoney[6];
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void readFileToSaveListView(wstring path); 
void loadItemToScreen(HWND Listview); 
bool insertItemToList(HWND Listview); 
void saveListItemToFile(wstring path);
void drawchart(HDC hdc);
int getTypeIndex(CItem* item); 

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1512562QLCT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512562QLCT));

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512562QLCT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1512562QLCT);
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
      CW_USEDEFAULT, 0, 650,700, nullptr, nullptr, hInstance, nullptr);

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
HWND g_Listview;
HWND hTotalMoney;
HWND hInputMoney;
HWND hDescription;
HWND BtnAdd;
HWND Combobox;
int defaultX, defaultY;
HWND hPercentA, hPercentB, hPercentC, hPercentD, hPercentE, hPercentF;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc = NULL;
    switch (message)
    {
	case WM_CREATE:
	{
		//Init extended common controls
		INITCOMMONCONTROLSEX icc;
		icc.dwSize = sizeof(icc);
		icc.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&icc);
		//Get main window DC
		hdc = GetDC(hWnd);
		ghWnd = hWnd;
		//lay toa do mac dinh
		defaultX = 20;
		defaultY = 20;
		//lay font he thong
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);
		HWND groupbox = CreateWindow(L"BUTTON", L"Thêm chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, defaultX, defaultY, 600, 100, hWnd, NULL, hInst, NULL);
		SendMessage(groupbox, WM_SETFONT, (WPARAM)hFont, TRUE);
		//group box loai chi tieu
		HWND statictype = CreateWindow(L"STATIC", L"Loại chi tiêu:", WS_CHILD | WS_VISIBLE, defaultX+30, defaultY + 20, 70, 16, hWnd, NULL, hInst, NULL);
		SendMessage(statictype, WM_SETFONT, (WPARAM)hFont, TRUE);

		//loai chi tieu
		Combobox = CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | CBS_DROPDOWNLIST | CBS_HASSTRINGS, defaultX + 30, defaultY + 40, 120, 100, hWnd, NULL, hInst, NULL);
		SendMessage(Combobox, WM_SETFONT, (WPARAM)hFont, TRUE);

		for (int i = 0; i < 6; i++)
		{
			SendMessage(Combobox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)types[i]);
		}

		//nhap tien:
		statictype = CreateWindow(L"STATIC", L"Số tiền:", WS_CHILD | WS_VISIBLE, defaultX + 200, defaultY + 20, 70, 16, hWnd, NULL, hInst, NULL);
		SendMessage(statictype, WM_SETFONT, (WPARAM)hFont, TRUE);

		hInputMoney = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | ES_NUMBER | WS_BORDER, defaultX + 200, defaultY + 40, 100, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hInputMoney, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hInputMoney, EM_SETCUEBANNER, TRUE, (LPARAM)L"Nhập số tiền...");

		statictype = CreateWindow(L"STATIC", L"Nội dung chi tiêu:", WS_CHILD | WS_VISIBLE, defaultX + 350, defaultY + 20, 120, 16, hWnd, NULL, hInst, NULL);
		SendMessage(statictype, WM_SETFONT, (WPARAM)hFont, TRUE);

		hDescription = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, defaultX + 350, defaultY+40, 120, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hDescription, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hDescription, EM_SETCUEBANNER, TRUE, (LPARAM)L"Nhập nội dung...");

		//them button Them
		BtnAdd = CreateWindow(L"BUTTON", L"Thêm", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, defaultX + 500, defaultY + 40, 80, 20, hWnd, (HMENU)IDC_BUTTON_ADD, hInst, NULL);
		SendMessage(BtnAdd, WM_SETFONT, (WPARAM)hFont, TRUE);

		//tao groupbox danh sach chi tieu
		groupbox = CreateWindow(L"BUTTON", L"Danh sách chi tiêu", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, defaultX, defaultY+110, 600, 300, hWnd, NULL, hInst, NULL);
		SendMessage(groupbox, WM_SETFONT, (WPARAM)hFont, TRUE);
		/////////////////////////////////////////////////////////////
		//Tao mot listview
		g_Listview= CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, L"List View", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | LVS_REPORT | LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS, defaultX+10, defaultY+140, 575,250, hWnd, NULL, hInst, NULL);
		//Tao 3 cot
		LVCOLUMN lvCol;

		//
		lvCol.cx = 150;
		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.pszText = L"Loại";
		ListView_InsertColumn(g_Listview, 0, &lvCol);

		lvCol.cx = 250;
		lvCol.pszText = L"Nội dung";
		ListView_InsertColumn(g_Listview, 1, &lvCol);

		lvCol.cx = 170;
		lvCol.pszText = L"Số tiền";
		ListView_InsertColumn(g_Listview, 2, &lvCol);

		/////////////////////////////////////////////
		//doc file vao luu vao listview
		readFileToSaveListView(L"input.txt");
		loadItemToScreen(g_Listview);
		groupbox = CreateWindow(L"BUTTON", L"Biểu đồ thống kê", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, defaultX, defaultY+420, 600, 150, hWnd, NULL, hInst, NULL);
		SendMessage(groupbox, WM_SETFONT, (WPARAM)hFont, TRUE);
		statictype = CreateWindow(L"STATIC", L"Tổng cộng:", WS_CHILD | WS_VISIBLE, defaultX + 350, defaultY + 450, 120, 20, hWnd, NULL, hInst, NULL);
		SendMessage(statictype, WM_SETFONT, (WPARAM)hFont, TRUE);

		hTotalMoney = CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_BORDER | WS_CHILD | ES_READONLY, defaultX + 420, defaultY + 450, 100, 20, hWnd, NULL, hInst, NULL);
		SendMessage(hTotalMoney, WM_SETFONT, (WPARAM)hFont, TRUE);
		SetWindowText(hTotalMoney, to_wstring(g_totalMoney).c_str());
		hPercentA = CreateWindowEx(0, L"STATIC", L"", WS_CHILD, defaultX, defaultY + 500, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentA, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentB = CreateWindowEx(0, L"STATIC", L"", WS_CHILD, defaultX, defaultY + 500, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentB, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentC = CreateWindowEx(0, L"STATIC", L"", WS_CHILD, defaultX, defaultY + 500, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentC, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentD = CreateWindowEx(0, L"STATIC", L"", WS_CHILD, defaultX, defaultY + 500, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentD, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentE = CreateWindowEx(0, L"STATIC", L"", WS_CHILD, defaultX, defaultY + 500, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentE, WM_SETFONT, WPARAM(hFont), TRUE);

		hPercentF = CreateWindowEx(0, L"STATIC", L"", WS_CHILD, defaultX, defaultY + 475, 30, 15, hWnd, (HMENU)NULL, hInst, NULL);
		SendMessage(hPercentF, WM_SETFONT, WPARAM(hFont), TRUE);

	}
	break;
    case WM_COMMAND:
        {
            wmId = LOWORD(wParam);

            // Parse the menu selections:
            switch (wmId)
            {
			case IDC_BUTTON_ADD:
			{
				insertItemToList(g_Listview);
				SetWindowText(hDescription, L"");
				SetWindowText(hInputMoney, L"");
				SetWindowText(hTotalMoney, to_wstring(g_totalMoney).c_str());
			}
			break;
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
            // TODO: Add any drawing code that uses hdc here...
			drawchart(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		saveListItemToFile(L"input.txt");
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
void readFileToSaveListView(wstring path)
{
	const locale utf8_locale = locale(locale(), new codecvt_utf8<wchar_t>());
	wfstream f;
	f.imbue(utf8_locale);
	f.open(path, ios::in);

	wstring buffer;
	if (f.is_open())
	{
		
		if (getline(f, buffer))
		{
			g_totalMoney = _wtoi64(buffer.c_str());
		}
	}
	while (getline(f, buffer))
	{
		CItem *item = new CItem();
		wcscpy_s(item->mType, buffer.c_str());
		getline(f, buffer);
		item->mMoney = _wtoi64(buffer.c_str());
		getline(f, buffer);
		item->mDescription = buffer;
		listItem.push_back(item);
	}
	f.close();
}
void loadItemToScreen(HWND Listview)
{
	LV_ITEM lv;
	WCHAR *buffer = new WCHAR[20];
	for (int i = 0; i < listItem.size(); i++)
	{
		//load type
		lv.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;
		lv.iItem = 0;
		lv.iSubItem = 0;
		lv.pszText = listItem[i]->mType;

		ListView_InsertItem(Listview, &lv);

		
		lv.mask = LVIF_TEXT;
		lv.iSubItem = 1;
		lv.pszText = (WCHAR*)listItem[i]->mDescription.c_str();
		ListView_SetItem(Listview, &lv);

		lv.iSubItem = 2;
		wsprintf(buffer, L"%I64d", listItem[i]->mMoney);
		lv.pszText = buffer;
		ListView_SetItem(Listview, &lv);

	}
}

bool insertItemToList(HWND Listview)
{
	CItem *item = new CItem();
	WCHAR*buffer;

	int size = GetWindowTextLength(hDescription);
	if (size > 0)
	{
		buffer = new WCHAR[size + 1];
		GetWindowText(hDescription, buffer, size + 1);
		item->mDescription = wstring(buffer);
	}
	else
	{
		MessageBox(ghWnd, L"Vui lòng nhập nội dung", L"Lỗi", MB_ICONWARNING | MB_OK);
		return false;
	}
	size = GetWindowTextLength(hInputMoney);
	if (size > 0)
	{
		buffer = new WCHAR[size + 1];
		GetWindowText(hInputMoney, buffer, size + 1);
		item->mMoney = _wtoi64(buffer);
	}
	else
	{
		MessageBox(ghWnd, L"Vui lòng nhập số tiền", L"Lỗi", MB_ICONWARNING | MB_OK);
		return false;
	}
	
	size = GetWindowTextLength(Combobox);
	if (size < 0)
	{
		MessageBox(ghWnd, L"Vui lòng chọn loại chi tiêu", L"Lỗi", MB_ICONWARNING | MB_OK);
		return false;
	}
	buffer = new  WCHAR[size + 1];
	GetWindowText(Combobox, buffer, size + 1);
	wcscpy_s(item->mType, buffer);

	listItem.push_back(item);

	g_totalMoney += item->mMoney;

	LV_ITEM lv;
	lv.mask = LVIF_TEXT | LVIF_PARAM | LVIF_IMAGE;

	
	lv.iItem = listItem.size()-1;
	lv.iSubItem = 0;
	lv.pszText = item->mType;
	ListView_InsertItem(Listview, &lv);

	lv.mask = LVIF_TEXT;
	lv.iSubItem = 2;
	buffer = new WCHAR[20];
	wsprintf(buffer, L"%I64d", item->mMoney);
	lv.pszText = buffer;
	ListView_SetItem(Listview, &lv);
	
	lv.iSubItem = 1;
	lv.pszText = (WCHAR*)item->mDescription.c_str();
	ListView_SetItem(Listview, &lv);

	RedrawWindow(ghWnd, NULL, NULL, RDW_INTERNALPAINT | RDW_UPDATENOW | RDW_ERASENOW | RDW_INVALIDATE);
	return true;
}
void saveListItemToFile(wstring path)
{
	//mo file
	const locale utf8_locale = locale(locale(), new codecvt_utf8<wchar_t>());
	wofstream f(path);
	f.imbue(utf8_locale);

	//luu tong so tien
	f << g_totalMoney << endl;

	for (int i = 0; i < listItem.size(); i++)
	{
		f << wstring(listItem[i]->mType) << endl;
		f << listItem[i]->mMoney << endl;
		f << wstring(listItem[i]->mDescription) << endl;
	}
	f.close();
}
HWND temp;
void drawchart(HDC hdc)
{
	//If total amount of money is 0, hide the Charts
	if (g_totalMoney <= 0)
	{
		//Clear the charts
		//fillRectangle(hdc, defaultX + 20, defaultY + 375, defaultX + 20 + CHART_WIDTH, defaultY + 375 + CHART_HEIGHT, color[6]);
		ShowWindow(hPercentA, SW_HIDE);
		ShowWindow(hPercentB, SW_HIDE);
		ShowWindow(hPercentC, SW_HIDE);
		ShowWindow(hPercentD, SW_HIDE);
		ShowWindow(hPercentE, SW_HIDE);
		ShowWindow(hPercentF, SW_HIDE);
		return;
	}

	//Init
	for (int i = 0; i < 6; i++)
	{
		totaltypeMoney[i] = 0;
	}

	int graphWidth =560;
	WCHAR buffer[5];
	currY = defaultY + 500;
	currX[7];
	currX[0] = defaultX + 20;
	float percent;

	//Calculate the total money of each type
	for (int i = 0; i < listItem.size(); i++)
	{
		//Why switch case can be used with string T.T
		totaltypeMoney[getTypeIndex(listItem[i])] += listItem[i]->mMoney;
	}


	//Draw the chart
	//A
	for (int i = 0; i < 6; i++)
	{
		percent = (totaltypeMoney[i] * 1.0 / g_totalMoney); //The percentage of the first type in total amount of money
		currX[i + 1] = currX[i] + percent*graphWidth; //Calculate the start X and end X of percentA chart

		switch (i)
		{
		case 0:
			temp = hPercentA;
			break;
		case 1:
			temp = hPercentB;
			break;
		case 2:
			temp = hPercentC;
			break;
		case 3:
			temp = hPercentD;
			break;
		case 4:
			temp = hPercentE;
			break;
		case 5:
			temp = hPercentF;
			break;


		}

		if (percent >= 1)
		{
			ShowWindow(temp, SW_SHOW);
			MoveWindow(temp, currX[i] + percent*graphWidth / 2 - 15, currY + 7, 27, 15, FALSE);
			SetWindowText(temp, to_wstring(percent * 100).c_str());
		}
		else
		{
			ShowWindow(temp, SW_HIDE);
		}
	}
	for (int i = 0; i < 6; i++)
	{
		//fillRectangle(hdc, currX[i], currY, currX[i + 1], currY + CHART_HEIGHT, color[i]);
		HBRUSH hbrush = CreateSolidBrush(color[i]);
		RECT* rect = new RECT;
		rect->left = currX[i];
		rect->top = currY;
		rect->right = currX[i+1];
		rect->bottom = currY+30;
		FillRect(hdc, rect , hbrush);
	}
}
	int getTypeIndex(CItem* item)
	{
		if (wcscmp(item->mType, L"Ăn uống") == 0)
		{
			return 0;
		}
		else if (wcscmp(item->mType, L"Di chuyển") == 0)
		{
			return 1;
		}
		else if (wcscmp(item->mType, L"Nhà cửa") == 0)
		{
			return 2;
		}
		else if (wcscmp(item->mType, L"Xe cộ") == 0)
		{
			return 3;
		}
		else if (wcscmp(item->mType, L"Nhu yếu phẩm") == 0)
		{
			return 4;
		}
		else
		{
			return 5;
		}
	}