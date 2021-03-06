/* FILENAME: ANIM.H
 * PROGRAMMER: VG4
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 10.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "render.h"

/* ������������ ���������� ��������� �������� */
#define VG4_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagvg4UNIT vg4UNIT;

/* ��������� �������� ��������� �������� */
typedef struct
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HBITMAP hBmFrame;   /* ����� ����� */
  HDC hDC;            /* �������� ������ */

  /* ������ ��������� �������� � �� ���������� */
  vg4UNIT *Units[VG4_MAX_UNITS]; 
  INT NumOfUnits;

  /* ���������� ������������� */
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */

  /* ���������� ����� */
  BYTE
    Keys[256],       /* �������� ������ ���������� � ���� */
    KeysOld[256],    /* �������� ������ �� ���������� ����� */
    KeysClick[256];  /* ����� ������������ ������� */
  INT
    MsDeltaX, MsDeltaY, /* ������������� ���������� ������� ���� */
    MsX, MsY,        /* ���������� ���������� ������� ���� */
    MsWheel;         /* ��������� ������ ���� */
  BYTE
    JButs[32],       /* �������� ������ ��������� */
    JButsOld[32],    /* �������� ������ ��������� �� ���������� ����� */
    JButsClick[32],  /* ����� ������������ ������� ������ ��������� */
    JPOV;            /* ������������� POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* ��� ��������� (-1.0 .. 1.0*/
} vg4ANIM;

/* ��������� �������� �������� */
extern vg4ANIM VG4_Anim;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ������������� ��������:
 *     INT Id;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( vg4UNIT *Unit, vg4ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( vg4UNIT *Unit, vg4ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( vg4UNIT *Unit, vg4ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( vg4UNIT *Unit, vg4ANIM *Ani );
 */
#define VG4_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( vg4UNIT *Unit, vg4ANIM *Ani );       \
  VOID (*Close)( vg4UNIT *Unit, vg4ANIM *Ani );      \
  VOID (*Response)( vg4UNIT *Unit, vg4ANIM *Ani );   \
  VOID (*Render)( vg4UNIT *Unit, vg4ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagvg4UNIT
{
  VG4_UNIT_BASE_FIELDS;
};

/* ���������� ���������� - ������� ��������� ������ ���� */
extern INT VG4_MouseWheel;

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID VG4_AnimInit( HWND hWnd );

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID VG4_AnimResize( INT W, INT H );

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimRender( VOID );

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimCopyFrame( VOID );

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       vg4UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID VG4_AnimAddUnit( vg4UNIT *Unit );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimFlipFullScreen( VOID );

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (vg4UNIT *) ��������� �� ��������� ������ ��������.
 */
vg4UNIT * VG4_AnimUnitCreate( INT Size );

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID VG4_AnimDoExit( VOID );

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID VG4_AnimSetPause( BOOL NewPauseFlag );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
