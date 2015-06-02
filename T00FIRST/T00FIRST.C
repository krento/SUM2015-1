#include <windows.h>

void main( void )
{
  int x, y;
  HDC hDC = GetDC(NULL);

  Rectangle(hDC, -1000, 0, 800, 800);

  for (y = 0; y < 768; y++)
    for (x = -1000; x < 1000; x++)
      SetPixel(hDC, x, y, RGB(x, y, 255 - (x + y) / 2));

  ReleaseDC(NULL, hDC);
  MessageBox(NULL, "Ask", "30", MB_OK);
}



