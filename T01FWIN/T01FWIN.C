/* T01FWIN.C
 * Пример базового макета программы под WinAPI.
 * Создание и поддержка простейшего окна.
 */

#include <windows.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Имя класса окна */
#define WND_CLASS_NAME "My window class"

/* Ссылка вперед */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam );

/* Главная функция программы.
 *   - дескриптор экземпляра приложения:
 *       HINSTANCE hInstance;
 *   - дескриптор предыдущего экземпляра приложения
 *     (не используется и должно быть NULL):
 *       HINSTANCE hPrevInstance;
 *   - командная строка:
 *       CHAR *CmdLine;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) код возврата в операционную систему.
 *   0 - при успехе.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  /* HINSTANCE hIns = LoadLibrary("shell32"); */

  /* Регистрация класса окна */
  wc.style = CS_VREDRAW | CS_HREDRAW; /* Стиль окна: полностью перерисовывать
                                       * при изменении вертикального или
                                       * горизонтального размеров
                                       * еще можно CS_DBLCLKS для добавления
                                       * отработки двойного нажатия */
  wc.cbClsExtra = 0; /* Дополнительное количество байт для класса */
  wc.cbWndExtra = 0; /* Дополнительное количество байт для окна */
  wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
  wc.hCursor = LoadCursor(NULL, IDC_HAND); /* Загрузка курсора (системного) */
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK); /* Загрузка пиктограммы (системной) */
  wc.hInstance = hInstance; /* Дескриптор приложения, регистрирующего класс */
  wc.lpszMenuName = NULL; /* Имя ресурса меню */
  wc.lpfnWndProc = MyWindowFunc; /* Указатель на функцию обработки */
  wc.lpszClassName = WND_CLASS_NAME;

  /* Регистрация класса в системе */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  /* Создание окна */
  hWnd =
    CreateWindow(WND_CLASS_NAME,    /* Имя класса окна */
      "Title",                      /* Заголовок окна */
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,          /* Стили окна - окно общего вида */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Позиция окна (x, y) - по умолчанию */
      CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - по умолчанию */
      NULL,                         /* Дескриптор родительского окна */
      NULL,                         /* Дескриптор загруженного меню */
      hInstance,                    /* Дескриптор приложения */
      NULL);                        /* Указатель на дополнительные параметры */

  CreateWindow("Button",    /* Имя класса окна */
    "Hello",                      /* Заголовок окна */
    WS_CHILD | WS_VISIBLE,          /* Стили окна - окно общего вида */
    10, 10, 80, 30, /* Размеры окна (w, h) - по умолчанию */
    hWnd,                         /* Дескриптор родительского окна */
    (HMENU)1,                         /* Дескриптор загруженного меню */
    hInstance,                    /* Дескриптор приложения */
    NULL);                        /* Указатель на дополнительные параметры */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /* Запуск цикла сообщений окна */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    /* Обработка сообщений от клавиатуры */
    TranslateMessage(&msg);
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

#define sqr(X) ((X) * (X))

/* Рисование глаз.
 * АРГУМЕНТЫ:
 *   - дескриптор контекста рисования:
 *       HDC hDC;
 *   - размеры окна:
 *       INT W, H;
 *   - позиция мыши:
 *       INT Xc, Yc;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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
  SelectObject(hDC, GetStockObject(DC_BRUSH));
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));
  Ellipse(hDC, 0, 0, W, H);
  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(2, 55, 5));
  Ellipse(hDC, x - W / 8, y - H / 8, x + W / 8, y + H / 8);

} /* End of 'DrawEye' function */


/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения (см. WM_***):
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg,
                               WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  INT x, y;
  CREATESTRUCT *cs;
  POINT pt;
  PAINTSTRUCT ps;
  SYSTEMTIME st;
  CHAR Buf[100];
  HFONT hFnt;
  RECT rc;
  static INT w, h;

  switch (Msg)
  {
  case WM_CREATE:
    cs = (CREATESTRUCT *)lParam;
    SetTimer(hWnd, 111, 50, NULL);
    return 0;

  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);
    return 0;

  case WM_TIMER:
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_COMMAND:
    if (LOWORD(wParam) == 1)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;

  case WM_CLOSE:
    if (MessageBox(hWnd, "Are you shure to exit from program?",
          "Exit", MB_YESNO | MB_ICONQUESTION) == IDNO)
       return 0;
    break;
  case WM_KEYDOWN:
    if (wParam == VK_F6)
      DestroyWindow(hWnd);
    return 0;
  case WM_KEYUP:
    if (wParam == VK_F1)
      DestroyWindow(hWnd);
    return 0;

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    x = LOWORD(lParam);
    y = HIWORD(lParam);
    hDC = GetDC(hWnd);
    SelectObject(hDC, GetStockObject(DC_PEN));
    SetDCPenColor(hDC, RGB(255, 0, 0));
    MoveToEx(hDC, w / 2, h / 2, NULL);
    LineTo(hDC, x, y);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;

  case WM_MOUSEMOVE:
    x = (SHORT)LOWORD(lParam);
    y = (SHORT)HIWORD(lParam);
    hDC = GetDC(hWnd);
    if (wParam & MK_LBUTTON)
    {
      Ellipse(hDC, x - 5, y - 5, x + 5, y + 5);
    }
    SelectObject(hDC, GetStockObject(DC_PEN));
    SetDCPenColor(hDC, RGB(255, 0, 0));
    MoveToEx(hDC, w / 2, h / 2, NULL);
    LineTo(hDC, x, y);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_CHAR:
    if (wParam == 'a')
      DestroyWindow(hWnd);
    return 0;

  case WM_ERASEBKGND:
    hDC = (HDC)wParam;
    MoveToEx(hDC, 0, 0, NULL);
    LineTo(hDC, w, h);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    x = w / 2 + 300 * sin(clock() / 1000.0);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    DrawEye(hDC, w, h, pt.x, pt.y);
    /*
    Ellipse(hDC, 10, 10, w - 10, h - 10);
    Ellipse(hDC, 10 + x, h / 2 - 10, x - 10, h / 2 + 10);
    */

    hFnt = CreateFont(130, 0, 0, 0, FW_BOLD, FALSE, FALSE,
      FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_ROMAN, "");
    SelectObject(hDC, hFnt);
    GetLocalTime(&st);
    SetTextColor(hDC, RGB(0, 255, 0));
    SetBkColor(hDC, RGB(255, 255, 0));
    SetBkMode(hDC, TRANSPARENT);
    TextOut(hDC, 100, 100, Buf,
      sprintf(Buf, "%02d:%02d:%02d (%02d.%02d.%d)",
        st.wHour, st.wMinute, st.wSecond,
        st.wDay, st.wMonth, st.wYear));

    DeleteObject(hFnt);

    hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, TRUE, FALSE,
      TRUE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
      CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
      VARIABLE_PITCH | FF_SWISS, "");
    SelectObject(hDC, hFnt);

    rc.left = 5;
    rc.top = 5;
    rc.right = w - 5;
    rc.bottom = h - 5;
    SetTextColor(hDC, RGB(222, 55, 0));
    DrawText(hDC, Buf, strlen(Buf), &rc,
      DT_SINGLELINE | DT_VCENTER | DT_CENTER);

    DeleteObject(hFnt);

    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    KillTimer(hWnd, 111);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T01FWIN.C' FILE */
