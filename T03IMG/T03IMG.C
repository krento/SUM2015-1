/* FILE NAME: T03IMG
*  PROGRAMMER: VG4
*  DATE: 03.06.2015
*  PURPOSE: Image drawing smaples.
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

  UpdateWindow(hWnd);
  ShowWindow(hWnd, ShowCmd);

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
  PAINTSTRUCT ps;
  HDC hDC, hMemDC;
  BITMAP bm;
  static HBITMAP hBm, hBmXor, hBmAnd;

  switch (Msg)
  {
  case WM_CREATE:
    hBm = LoadImage(NULL, "M.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmXor = LoadImage(NULL, "XOR.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmAnd = LoadImage(NULL, "AND.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);  
    hMemDC = CreateCompatibleDC(hDC);
    SelectObject(hMemDC, hBm);
    GetObject(hBm, sizeof(bm), &bm);
    BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight,
      hMemDC, 0, 0, SRCCOPY);

    /* AND */
    SelectObject(hMemDC, hBmAnd);
    GetObject(hBmAnd, sizeof(bm), &bm);
    BitBlt(hDC, 30, 30, bm.bmWidth, bm.bmHeight,
      hMemDC, 0, 0, SRCAND);

    /* XOR */
    SelectObject(hMemDC, hBmXor);
    GetObject(hBmAnd, sizeof(bm), &bm);
    BitBlt(hDC, 30, 30, bm.bmWidth, bm.bmHeight,
      hMemDC, 0, 0, SRCINVERT);

    DeleteDC(hMemDC);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    DeleteObject(hBm);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T01FWIN.C' FILE */
