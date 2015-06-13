/* FILENAME: ANIM.C
 * PROGRAMMER: VG4
 * PURPOSE: Animation system module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Получение значения оси джойстика */
#define VG4_GET_AXIS_VALUE(Axis) \
  (2.0 * (ji.dw ## Axis ## pos - jc.w ## Axis ## min) / (jc.w ## Axis ## max - jc.w ## Axis ## min) - 1.0)

/* Системный контекст анимации */
vg4ANIM VG4_Anim;

/* Данные для синхронизации по времени */
static INT64
  TimeFreq,  /* единиц измерения в секунду */
  TimeStart, /* время начала анимации */
  TimeOld,   /* время прошлого кадра */
  TimePause, /* время простоя в паузе */
  TimeFPS;   /* время для замера FPS */
static INT
  FrameCounter; /* счетчик кадров */

/* Сохраненные мышиные координаты */
static INT
  VG4_MouseOldX, VG4_MouseOldY;

/* Функция инициализации анимации.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL VG4_AnimInit( HWND hWnd )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;
  PIXELFORMATDESCRIPTOR pfd = {0};

  memset(&VG4_Anim, 0, sizeof(vg4ANIM));
  VG4_Anim.hWnd = hWnd;
  VG4_Anim.hDC = GetDC(hWnd);

  /* Инициализируем буфер кадра */
  VG4_Anim.W = 30;
  VG4_Anim.H = 30;
  VG4_Anim.NumOfUnits = 0;

  /*** Инициализация OpenGL ***/

  /* описываем формат точки */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(VG4_Anim.hDC, &pfd);
  DescribePixelFormat(VG4_Anim.hDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(VG4_Anim.hDC, i, &pfd);

  /* создаем контекст построения */
  VG4_Anim.hGLRC = wglCreateContext(VG4_Anim.hDC);
  /* делаем текущими контексты */
  wglMakeCurrent(VG4_Anim.hDC, VG4_Anim.hGLRC);

  /* инициализируем расширения */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(VG4_Anim.hGLRC);
    ReleaseDC(VG4_Anim.hWnd, VG4_Anim.hDC);
    memset(&VG4_Anim, 0, sizeof(vg4ANIM));
    return FALSE;
  }

  VG4_RndProg = VG4_ShaderLoad("TEST");

  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glActiveTexture(GL_TEXTURE1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  /* Инициализация таймера */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  VG4_Anim.IsPause = FALSE;
  FrameCounter = 0;

  /* Инициализация ввода */
  GetCursorPos(&pt);
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_MouseOldX = pt.x;
  VG4_MouseOldY = pt.y;
  GetKeyboardState(VG4_Anim.KeysOld);
  return TRUE;
} /* End of 'VG4_AnimInit' function */

/* Функция деинициализации анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimClose( VOID )
{
  INT i;

  /* Освобождение памяти из-под объектов анимации */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    VG4_Anim.Units[i]->Close(VG4_Anim.Units[i], &VG4_Anim);
    free(VG4_Anim.Units[i]);
    VG4_Anim.Units[i] = NULL;
  }

  VG4_ShaderFree(VG4_RndProg);
  VG4_RndProg = 0;
  /* делаем текущими контексты */
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(VG4_Anim.hGLRC);
  /* Удаляем объекты GDI */
  ReleaseDC(VG4_Anim.hWnd, VG4_Anim.hDC);
} /* End of 'VG4_AnimClose' function */

/* Функция обработки изменения размера области вывода.
 * АРГУМЕНТЫ:
 *   - новый размер области вывода:
 *       INT W, H;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimResize( INT W, INT H )
{
  /* Сохранение размера */
  VG4_Anim.W = W;
  VG4_Anim.H = H;

  glViewport(0, 0, W, H);

  /* корректировка параметров проецирования */
  if (W > H)
    VG4_RndWp = (DBL)W / H * 3, VG4_RndHp = 3;
  else
    VG4_RndHp = (DBL)H / W * 3, VG4_RndWp = 3;

  VG4_RndMatrProj = MatrFrustum(-VG4_RndWp / 2, VG4_RndWp / 2,
                                -VG4_RndHp / 2, VG4_RndHp / 2,
                                VG4_RndProjDist, 800);

} /* End of 'VG4_AnimResize' function */

/* Функция построения кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimRender( VOID )
{
  INT i;
  LARGE_INTEGER li;
  POINT pt;

  /*** Обновление таймера ***/
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

  /* вычисляем FPS */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    VG4_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* время "прошлого" кадра */
  TimeOld = li.QuadPart;

  /*** Обновление ввода ***/

  /* Клавиатура */
  GetKeyboardState(VG4_Anim.Keys);
  for (i = 0; i < 256; i++)
    VG4_Anim.Keys[i] >>= 7;
  for (i = 0; i < 256; i++)
    VG4_Anim.KeysClick[i] = VG4_Anim.Keys[i] && !VG4_Anim.KeysOld[i];
  memcpy(VG4_Anim.KeysOld, VG4_Anim.Keys, sizeof(VG4_Anim.KeysOld));

  /* Мышь */
  /* колесо */
  VG4_Anim.MsWheel = VG4_MouseWheel;
  VG4_MouseWheel = 0;
  /* абсолютная позиция */
  GetCursorPos(&pt);
  ScreenToClient(VG4_Anim.hWnd, &pt);
  VG4_Anim.MsX = pt.x;
  VG4_Anim.MsY = pt.y;
  /* относительное перемещение */
  VG4_Anim.MsDeltaX = pt.x - VG4_MouseOldX;
  VG4_Anim.MsDeltaY = pt.y - VG4_MouseOldY;
  VG4_MouseOldX = pt.x;
  VG4_MouseOldY = pt.y;

  /* Джойстик */
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS jc;

    /* получение общей информации о джостике */
    if (joyGetDevCaps(JOYSTICKID2, &jc, sizeof(jc)) == JOYERR_NOERROR)
    {
      JOYINFOEX ji;

      /* получение текущего состояния */
      ji.dwSize = sizeof(JOYCAPS);
      ji.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID2, &ji) == JOYERR_NOERROR)
      {
        /* Кнопки */
        memcpy(VG4_Anim.JButsOld, VG4_Anim.JButs, sizeof(VG4_Anim.JButs));
        for (i = 0; i < 32; i++)
          VG4_Anim.JButs[i] = (ji.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          VG4_Anim.JButsClick[i] = VG4_Anim.JButs[i] && !VG4_Anim.JButsOld[i];

        /* Оси */
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

  /* опрос на изменение состояний объектов */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
    VG4_Anim.Units[i]->Response(VG4_Anim.Units[i], &VG4_Anim);

  /* очистка фона */
  glClearColor(0.3, 0.5, 0.7, 1);
  glClearDepth(1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColorMask(TRUE, TRUE, TRUE, TRUE);


  /*
  glColor3d(0.3, 0.5, 0.7);
  glRectd(-2, -2, 2, 2);

  glColor3d(1, 0, 0);
  glRectd(0, 0, 0.88, 0.30);
  */

  /* рисование объектов */
  for (i = 0; i < VG4_Anim.NumOfUnits; i++)
  {
    static DBL time = 5;

    time += VG4_Anim.GlobalDeltaTime;
    if (time > 5)
    {
      time = 0;
      VG4_ShaderFree(VG4_RndProg);
      VG4_RndProg = VG4_ShaderLoad("TEST");
    }

    VG4_RndMatrWorld = MatrIdentity();

    VG4_Anim.Units[i]->Render(VG4_Anim.Units[i], &VG4_Anim);
  }
  glFinish();
  FrameCounter++;
} /* End of 'VG4_AnimRender' function */

/* Функция вывода кадра анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimCopyFrame( VOID )
{
  /* вывод буфера кадра */
  SwapBuffers(VG4_Anim.hDC);
} /* End of 'VG4_AnimCopyFrame' function */

/* Функция добавления в систему объекта анимации.
 * АРГУМЕНТЫ:
 *   - добавляемый объект анимации:
 *       vg4UNIT *Unit;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimAddUnit( vg4UNIT *Unit )
{
  if (VG4_Anim.NumOfUnits < VG4_MAX_UNITS)
  {
    VG4_Anim.Units[VG4_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &VG4_Anim);
  }
} /* End of 'VG4_AnimAddUnit' function */


/* Функция переключения в/из полноэкранного режима
 * с учетом нескольких мониторов.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* текущий режим */
  static RECT SaveRC;               /* сохраненный размер */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* сохраняем старый размер окна */
    GetWindowRect(VG4_Anim.hWnd, &SaveRC);

    /* определяем в каком мониторе находится окно */
    hmon = MonitorFromWindow(VG4_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* получаем информацию для монитора */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* переходим в полный экран */
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
    /* восстанавливаем размер окна */
    SetWindowPos(VG4_Anim.hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
} /* End of 'VG4_AnimFlipFullScreen' function */

/* Функция выхода из анимации.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimDoExit( VOID )
{
  PostMessage(VG4_Anim.hWnd, WM_CLOSE, 0, 0);
} /* End of 'VG4_AnimDoExit' function */

/* Функция установки паузы анимации.
 * АРГУМЕНТЫ:
 *   - флаг паузы:
 *       BOOL NewPauseFlag;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_AnimSetPause( BOOL NewPauseFlag )
{
  VG4_Anim.IsPause = NewPauseFlag;
} /* End of 'VG4_AnimSetPause' function */

/* END OF 'ANIM.C' FILE */
