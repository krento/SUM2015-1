/* FILENAME: ANIM.C
 * PROGRAMMER: VG4
 * PURPOSE: Animation system module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* ��������� �������� ��� ��������� */
#define VG4_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* ��������� �������� �������� */
vg4ANIM VG4_Anim;

/* ������ ��� ������������� �� ������� */
static INT64
  TimeFreq,  /* ������ ��������� � ������� */
  TimeStart, /* ����� ������ �������� */
  TimeOld,   /* ����� �������� ����� */
  TimePause, /* ����� ������� � ����� */
  TimeFPS;   /* ����� ��� ������ FPS */
static INT
  FrameCounter; /* ������� ������ */

/* ����������� ������� ���������� */
static INT
  VG4_MouseOldX, VG4_MouseOldY;

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID VG4_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd);
  LARGE_INTEGER li;
  POINT pt;

  memset(&VG4_Anim, 0, sizeof(vg4ANIM));
  VG4_Anim.hWnd = hWnd;
  /* �������������� ����� ����� */
  VG4_Anim.hDC = CreateCompatibleDC(hDC);
  VG4_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  SelectObject(VG4_Anim.hDC, VG4_Anim.hBmFrame);
  VG4_Anim.W = 30;
  VG4_Anim.H = 30;
  VG4_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* ������������� ������� */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  VG4_Anim.IsPause = FALSE;
  FrameCounter = 0;

  /* ������������� ����� */
  GetCursorPos(&pt);
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_MouseOldX = pt.x;
  VG4_MouseOldY = pt.y;
  GetKeyboardState(VG4_Anim.KeysOld);
} /* End of 'VG4_AnimInit' function */

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimClose( VOID )
{
  INT i;

  /* ������������ ������ ��-��� �������� �������� */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    VG4_Anim.Units[i]->Close(VG4_Anim.Units[i], &VG4_Anim);
    free(VG4_Anim.Units[i]);
    VG4_Anim.Units[i] = NULL;
  }

  /* ������� ������� GDI */
  DeleteObject(VG4_Anim.hBmFrame);
  DeleteDC(VG4_Anim.hDC);
} /* End of 'VG4_AnimClose' function */

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID VG4_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(VG4_Anim.hWnd);

  /* ������������� ������ � ������ ����� */
  DeleteObject(VG4_Anim.hBmFrame);
  VG4_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(VG4_Anim.hDC, VG4_Anim.hBmFrame);

  /* ���������� ������� */
  VG4_Anim.W = W;
  VG4_Anim.H = H;

  ReleaseDC(VG4_Anim.hWnd, hDC);


  /* ������������� ���������� ������������� */
  if (W > H)
    VG4_RndWp = (DBL)W / H * 3, VG4_RndHp = 3;
  else
    VG4_RndHp = (DBL)H / W * 3, VG4_RndWp = 3;

  VG4_RndMatrProj = MatrFrustum(-VG4_RndWp / 2, VG4_RndWp / 2,
                                -VG4_RndHp / 2, VG4_RndHp / 2,
                                VG4_RndProjDist, 800);

} /* End of 'VG4_AnimResize' function */

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  /*** ���������� ������� ***/
  QueryPerformanceCounter(&li);
  VG4_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  VG4_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  if (!VG4_Anim.IsPause)
    VG4_Anim.DeltaTime = VG4_Anim.GlobalDeltaTime;
  else
  {
    TimePause += li.QuadPart - TimeOld;
    VG4_Anim.DeltaTime = 0;
  }

  VG4_Anim.Time = (DBL)(li.QuadPart - TimePause - TimeStart) / TimeFreq;

  /* ��������� FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    VG4_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* ����� "��������" ����� */
  TimeOld = li.QuadPart;

  /*** ���������� ����� ***/

  /* ���������� */
  GetKeyboardState(VG4_Anim.Keys);
  for (i = 0; i < 256; i++)
    VG4_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    VG4_Anim.KeysClick[i] = VG4_Anim.Keys[i] && !VG4_Anim.KeysOld[i];
  memcpy(VG4_Anim.KeysOld, VG4_Anim.Keys, sizeof(VG4_Anim.KeysOld));

  /* ���� */
  /* ������ */
  VG4_Anim.MsWheel = VG4_MouseWheel;
  VG4_MouseWheel = 0;
  /* ���������� ������� */
  GetCursorPos(&pt);
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_Anim.MsX = pt.x;
  VG4_Anim.MsY = pt.y;
  /* ������������� ����������� */
  VG4_Anim.MsDeltaX = pt.x - VG4_MouseOldX;
  VG4_Anim.MsDeltaY = pt.y - VG4_MouseOldY;
  VG4_MouseOldX = pt.x;
  VG4_MouseOldY = pt.y;

  /* �������� */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* ��������� ����� ���������� � �������� */
    if (joyGetDevCaps(JOYSTICKID2, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* ��������� �������� ��������� */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID2, &ji) == JOYERR_NOERROR)
      {
        /* ������ */
        memcpy(VG4_Anim.JButsOld, VG4_Anim.JButs, sizeof(VG4_Anim.JButs));
        for (i = 0; i < 32; i++)
          VG4_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          VG4_Anim.JButsClick[i] = VG4_Anim.JButs[i] && !VG4_Anim.JButsOld[i];

        /* ��� */
        VG4_Anim.JX = VG4_GET_AXIS_VALUE(X);
        VG4_Anim.JY = VG4_GET_AXIS_VALUE(Y);
        if (jc.wCaps & JOYCAPS_HASZ)
          VG4_Anim.JZ = VG4_GET_AXIS_VALUE(Z);
        if (jc.wCaps & JOYCAPS_HASU)
          VG4_Anim.JU = VG4_GET_AXIS_VALUE(U);
        if (jc.wCaps & JOYCAPS_HASV)
          VG4_Anim.JV = VG4_GET_AXIS_VALUE(V);
        if (jc.wCaps & JOYCAPS_HASR)
          VG4_Anim.JR = VG4_GET_AXIS_VALUE(R);

        if (jc.wCaps & JOYCAPS_HASPOV)
        {
          if (ji.dwPOV == 0xFFFF)
            VG4_Anim.JPOV = 0;
          else
            VG4_Anim.JPOV = ji.dwPOV / 4500 + 1;
        }
      }
    }
  }

  /* ����� �� ��������� ��������� �������� */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
    VG4_Anim.Units[i]->Response(VG4_Anim.Units[i], &VG4_Anim);

  /* ������� ���� */
  SelectObject(VG4_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(VG4_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(VG4_Anim.hDC, RGB(0, 0, 0));
  Rectangle(VG4_Anim.hDC, 0, 0, VG4_Anim.W + 1, VG4_Anim.H + 1);

  /* ��������� �������� */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    SelectObject(VG4_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(VG4_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(VG4_Anim.hDC, RGB(155, 155, 155));
    SetDCPenColor(VG4_Anim.hDC, RGB(255, 255, 255));

    VG4_RndMatrWorld = MatrIdentity();

    VG4_Anim.Units[i]->Render(VG4_Anim.Units[i], &VG4_Anim);
  }
  FrameCounter++;
} /* End of 'VG4_AnimRender' function */

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* ����� ����� */
  hDC = GetDC(VG4_Anim.hWnd);
  BitBlt(hDC, 0, 0, VG4_Anim.W, VG4_Anim.H, VG4_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(VG4_Anim.hWnd, hDC);
} /* End of 'VG4_AnimCopyFrame' function */

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       vg4UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID VG4_AnimAddUnit( vg4UNIT *Unit )
{
  if (VG4_Anim.NumOfUnits < VG4_MAX_UNITS)
  {
    VG4_Anim.Units[VG4_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &VG4_Anim);
  }
} /* End of 'VG4_AnimAddUnit' function */


/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* ������� ����� */
  static RECT SaveRC;               /* ����������� ������ */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* ��������� ������ ������ ���� */
    GetWindowRect(VG4_Anim.hWnd, &SaveRC);

    /* ���������� � ����� �������� ��������� ���� */
    hmon = MonitorFromWindow(VG4_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* �������� ���������� ��� �������� */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* ��������� � ������ ����� */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(VG4_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(VG4_Anim.hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* ��������������� ������ ���� */
    SetWindowPos(VG4_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'VG4_AnimFlipFullScreen' function */

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimDoExit( VOID )
{
  PostMessage(VG4_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'VG4_AnimDoExit' function */

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID VG4_AnimSetPause( BOOL NewPauseFlag )
{
  VG4_Anim.IsPause = NewPauseFlag;
} /* End of 'VG4_AnimSetPause' function */

/* END OF 'ANIM.C' FILE */
