/* FILE NAME: T02DBLB.C
 * PROGRAMMER: VG4
 * DATE: 02.06.2015
 * PURPOSE: WinAPI windowed application sample.
 */

#pragma warning(disable: 4244)

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <windows.h>

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
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,          /* ����� ���� - ���� ������ ���� */
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
  {
    /* ��������� ��������� �� ���������� */
    TranslateMessage(&msg);
    /* �������� ��������� � ������� ���� */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

#define sqr(X) ((X) * (X))

/* ��������� ����.
 * ���������:
 *   - ���������� ��������� ���������:
 *       HDC hDC;
 *   - ������� ����:
 *       INT W, H;
 *   - ������� ����:
 *       INT Xc, Yc;
 * ������������ ��������: ���.
 */
VOID DrawEye( HDC hDC, INT W, INT H, INT Xc, INT Yc )
{
  FLOAT
    len = sqrt(sqr(Xc - W / 2) + sqr(Yc - H / 2)),
    co = (Xc - W / 2) / len, si = (Yc - H / 2) / len;
  INT l = 30, x = W / 2 + co * l, y = H / 2 + si * l;

  l = len;
  if (l > W / 2 - W / 8)
    l = W / 2 - W / 8;
  x = W / 2 + co * l;
  y = H / 2 + si * l;


  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(NULL_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, 0, 0, W, H);
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(2, 55, 5));
  Ellipse(hDC, x - W / 8, y - H / 8, x + W / 8, y + H / 8);

} /* End of 'DrawEye' function */


/* ��������� ������� �� ����� */
VOID DrawHand( HDC hDC, INT Xc, INT Yc, INT L, INT W, FLOAT Angle )
{
  INT i;
  POINT pnts[] =
  {
    {0, -W}, {-W, 0}, {0, L}, {W, 0}
  }, pntdraw[sizeof pnts / sizeof pnts[0]];
  FLOAT si = sin(Angle), co = cos(Angle);

  for (i = 0; i < sizeof pnts / sizeof pnts[0]; i++)
  {
    pntdraw[i].x = Xc + pnts[i].x * co - pnts[i].y * si;
    pntdraw[i].y = Yc + pnts[i].x * si + pnts[i].y * co;
  }
  Polygon(hDC, pntdraw, sizeof pnts / sizeof pnts[0]);
} /* End of 'DrawHand' function */


/* ������� ������������ �/�� �������������� ������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* Go to full screen */

    /* ��������� ������ ������ ���� */
    GetWindowRect(hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);

    IsFullScreen = TRUE;
  }
  else
  {
    /* Return windowed mode */

    /* ��������������� ������ ���� */
    SetWindowPos(hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);

    IsFullScreen = FALSE;
  }
} /* End of 'FlipFullScreen' function */



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
  INT x, y, i;
  CREATESTRUCT *cs;
  POINT pt;
  SYSTEMTIME st;
  CHAR Buf[100];
  HFONT hFnt, hOldFnt;
  RECT rc;
  static INT xx[100], yy[100];
  static BITMAP bm;
  static HBITMAP hBm, hBmLogo;
  static HDC hMemDC, hMemDCLogo;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 111, 50, NULL);

    for (i = 0; i < 100; i++)
      xx[i] = rand() % 1000, yy[i] = rand() % 768;

    hBmLogo = LoadImage(NULL, "logo.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(bm), &bm);

    /* ������� �������� � ������ */
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCLogo, hBmLogo);
    SetPixel(hMemDCLogo, 10, 10, RGB(255, 0, 0));
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

    /* ������� �������� �������� � ���� */
    if (hBm != NULL)
      DeleteObject(hBm);

    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 111, 0);
    return 0;

  case WM_KEYDOWN:
    if (wParam == 'W')
      FlipFullScreen(hWnd);
    return 0;

  case WM_TIMER:
    /* Clear Background */
    SelectObject(hMemDC, GetStockObject(NULL_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hMemDC, RGB(255, 200, 30));
    Rectangle(hMemDC, 0, 0, w + 1, h + 1);

    StretchBlt(hMemDC, 0, 0, w, h,
      hMemDCLogo, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    /* Draw picture */
    x = w / 2 + 300 * sin(clock() / 1000.0);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    DrawEye(hMemDC, w, h, pt.x, pt.y);
    /*
    Ellipse(hDC, 10, 10, w - 10, h - 10);
    Ellipse(hDC, 10 + x, h / 2 - 10, x - 10, h / 2 + 10);
    */

    hFnt = CreateFont(130, 0, 0, 0, FW_BOLD, FALSE, FALSE,
      FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_ROMAN, "");
    hOldFnt = SelectObject(hMemDC, hFnt);
    GetLocalTime(&st);
    SetTextColor(hMemDC, RGB(0, 255, 0));
    SetBkColor(hMemDC, RGB(255, 255, 0));
    SetBkMode(hMemDC, TRANSPARENT);
    TextOut(hMemDC, 100, 100, Buf,
      sprintf(Buf, "%02d:%02d:%02d (%02d.%02d.%d)",
        st.wHour, st.wMinute, st.wSecond,
        st.wDay, st.wMonth, st.wYear));

    DeleteObject(hFnt);

    hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE,
      TRUE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_SWISS, "");
    SelectObject(hMemDC, hFnt);

    rc.left = 5;
    rc.top = 5;
    rc.right = w - 5;
    rc.bottom = h - 5;
    SetTextColor(hMemDC, RGB(222, 55, 0));
    DrawText(hMemDC, Buf, strlen(Buf), &rc,
      DT_SINGLELINE | DT_VCENTER | DT_CENTER);

    SelectObject(hMemDC, hOldFnt);
    DeleteObject(hFnt);

    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    srand(30);
    for (i = 0; i < 100; i++)
    {
      SetDCBrushColor(hMemDC, RGB(rand() % 256, rand() % 256, rand() % 256));
      SetDCPenColor(hMemDC, RGB(rand() % 256, rand() % 256, rand() % 256));
      Ellipse(hMemDC, xx[i] - 30, yy[i] - 30, xx[i] + 30, yy[i] + 30);
    }

    SetDCBrushColor(hMemDC, RGB(rand() % 256, rand() % 256, rand() % 256));
    SetDCPenColor(hMemDC, RGB(rand() % 256, rand() % 256, rand() % 256));
    DrawHand(hMemDC, w / 2, h / 2, 300, 30, (180 + st.wSecond * 6) * PI / 180);
    SetDCBrushColor(hMemDC, RGB(rand() % 256, rand() % 256, rand() % 256));
    SetDCPenColor(hMemDC, RGB(rand() % 256, rand() % 256, rand() % 256));
    DrawHand(hMemDC, w / 2, h / 2, 100, 45, (180 + st.wMinute * 6) * PI / 180);

    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you shure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
      return 0;
    break;

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);

    x = LOWORD(lParam);
    y = HIWORD(lParam);
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(255, 0, 0));
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, x, y);
    return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;

  case WM_MOUSEMOVE:
    x = (SHORT)LOWORD(lParam);
    y = (SHORT)HIWORD(lParam);
    if (wParam & MK_LBUTTON)
    {
      Ellipse(hMemDC, x - 5, y - 5, x + 5, y + 5);
    }
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(255, 0, 0));
    MoveToEx(hMemDC, w / 2, h / 2, NULL);
    LineTo(hMemDC, x, y);
    return 0;

  case WM_ERASEBKGND:
    BitBlt((HDC)wParam, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    return 0;

  case WM_DESTROY:
    DeleteDC(hMemDC);
    DeleteObject(hBm);
    DeleteDC(hMemDCLogo);
    DeleteObject(hBmLogo);
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T02DBLB.C' FILE */
