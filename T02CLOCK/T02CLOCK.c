/* FILE NAME: T02CLOCK
*  PROGRAMMER: AO5
*  DATE: 02.06.2015
*  PURPOSE: WinAPI windowed application sample*/

#include <windows.h>
#include <math.h>

/* �� */
#define PI 3.14159265358979323846

/* ��� ������ ���� */
#define WND_CLASS_NAME "My window class"

/* ������ ������ */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* ������� ������� ���������.
 *   - ���������� ���������� ����������:
 *       HINSTANCE hInstance;
 *   - ���������� ����������� ���������� ����������
 *     (�� ������������ � ������ ���� NULL):
 *       HINSTANCE hPrevInstance;
 *   - ��������� ������:
 *       CHAR *CmdLine;
 * ������������ ��������:
 *   (INT) ��� �������� � ������������ �������.
 *   0 - ��� ������.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* ����������� ������ ���� */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* ����� ����: ��������� ��������������
                                       * ��� ��������� ������������� ���
                                       * ��������������� ��������
                                       * ��� ����� CS_DBLCLKS ��� ����������
                                       * ��������� �������� ������� */
  wc.cbClsExtra = 0; /* �������������� ���������� ���� ��� ������ */
  wc.cbWndExtra = 0; /* �������������� ���������� ���� ��� ���� */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* �������� ������� (����������) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* �������� ����������� (���������) */
  wc.hInstance = hInstance; /* ���������� ����������, ��������������� ����� */
  wc.lpszMenuName = NULL; /* ��� ������� ���� */
  wc.lpfnWndProc = MyWindowFunc; /* ��������� �� ������� ��������� */
  wc.lpszClassName = WND_CLASS_NAME;

  /* ����������� ������ � ������� */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* �������� ���� */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* ��� ������ ���� */
      "Title",                      /* ��������� ���� */
      WS_OVERLAPPEDWINDOW,          /* ����� ���� - ���� ������ ���� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (x, y) - �� ��������� */
      CW_USEDEFAULT, CW_USEDEFAULT, /* ������� ���� (w, h) - �� ��������� */
      NULL,                         /* ���������� ������������� ���� */
      NULL,                         /* ���������� ������������ ���� */
      hInstance,                    /* ���������� ���������� */
      NULL);                        /* ��������� �� �������������� ��������� */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* ������ ����� ��������� ���� */
  while (GetMessage(&msg, NULL, 0, 0))
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */

/* ��������� ������� */
VOID DrawArrow( HDC hDC, INT X1, INT Y1, INT Len, DOUBLE Angle )
{
  DOUBLE si = sin(Angle * PI / 180), co = cos(Angle * PI / 180);

  MoveToEx(hDC, X1, Y1, NULL);
  LineTo(hDC, X1 + Len * si, Y1 - Len * co); 
} /* End of 'DrawArrow' function */

/* ������� ��������� ��������� ����.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 *   - ����� ��������� (��. WM_***):
 *       UINT Msg;
 *   - �������� ��������� ('word parameter'):
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter'):
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{ 
  HDC hDC;
  PAINTSTRUCT ps;
  SYSTEMTIME st;
  static INT w, h;
  static BITMAP bm;
  static HDC hMemDC, hMemDCClock;
  static HBITMAP hBm, hBmClock;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 1, 100, NULL);
    hBmClock = LoadImage(NULL, "clock2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmClock, sizeof(bm), &bm);

    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCClock = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCClock, hBmClock);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    if(hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 1, 0);
    return 0;
  
  case WM_TIMER:
    //SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 255, 255));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    StretchBlt(hMemDC, 0, 0, w, h,
      hMemDCClock, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    Ellipse(hMemDC, 10, 10, h / 2, h / 2);
    
    GetLocalTime(&st);
    DrawArrow(hMemDC, w / 2, h / 2, 300, st.wSecond * 6);
    DrawArrow(hMemDC, w / 2, h / 2, 250, st.wMinute * 6);
    DrawArrow(hMemDC, w / 2, h / 2, 150, (st.wHour % 12) * 30);



    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_ERASEBKGND:
    BitBlt((HDC)wParam, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    return 0;

  /*case WM_CLOSE:
    if (MessageBox(hWnd, "Are you shure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;  */
  
  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    DeleteDC(hMemDCClock);
    DeleteObject(hBmClock);
    KillTimer(hWnd, 1);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */


/* END OF 'T02CLOCK.C' FILE */