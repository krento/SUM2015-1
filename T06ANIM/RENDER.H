/* FILENAME: RENDER.H
 * PROGRAMMER: VG4
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 10.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������� - ����� ������ � ������������� */
typedef struct tagvg4GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} vg4GOBJ;

/* ������� */
extern MATR
  VG4_RndMatrWorld,
  VG4_RndMatrView,
  VG4_RndMatrProj;

/* ��������� ������������� */
extern DBL
  VG4_RndWp, VG4_RndHp,    /* ������� ������� ������������� */
  VG4_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 */
POINT VG4_RndWorldToScreen( VEC P );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4GOBJ *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ������ ��-��� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID VG4_RndGObjFree( vg4GOBJ *GObj );

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID VG4_RndGObjDraw( vg4GOBJ *GObj );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
