/* FILENAME: RENDER.C
 * PROGRAMMER: VG4
 * PURPOSE: Rendering system module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

/* ������� */
MATR
  VG4_RndMatrWorld = VG4_UNIT_MATR,
  VG4_RndMatrView = VG4_UNIT_MATR,
  VG4_RndMatrProj = VG4_UNIT_MATR,
  VG4_RndMatrWorldViewProj = VG4_UNIT_MATR;

/* ��������� ������������� */
DBL
  VG4_RndWp = 3, VG4_RndHp = 3,     /* ������� ������� ������������� */
  VG4_RndProjDist = 5;              /* ���������� �� ��������� �������� */

/* ������� �������������� �� ������� ������� ��������� � ����.
 * ���������:
 *   - �������� �����:
 *       VEC P;
 * ������������ ��������:
 *   (POINT) ���������� � �����.
 */
POINT VG4_RndWorldToScreen( VEC P )
{
  POINT Ps;
  VEC Pp;

  /* �������������� �� */
  Pp = VecMulMatr(P, VG4_RndMatrWorldViewProj);

  Ps.x = ((Pp.X + 1) / 2) * VG4_Anim.W;
  Ps.y = ((1 - Pp.Y) / 2) * VG4_Anim.H;

  return Ps;
} /* End of 'VG4_RndWorldToScreen' function */

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4GOBJ *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName )
{
  FILE *F;
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(vg4GOBJ));
  /* Open file */
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertices */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  /* Allocate memory for data */
  if ((GObj->V = malloc(sizeof(VEC) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  GObj->F = (INT (*)[3])(GObj->V + nv);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf",
        &GObj->V[nv].X, &GObj->V[nv].Y, &GObj->V[nv].Z);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }

  fclose(F);

  GObj->NumOfV = nv;
  GObj->NumOfF = nf;
  return TRUE;
} /* End of 'VG4_RndGObjLoad' function */

/* ������� ��������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID VG4_RndGObjDraw( vg4GOBJ *GObj )
{
  INT i;
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * GObj->NumOfV)) == NULL)
    return;

  /* ���������� ��� ����� */
  VG4_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(VG4_RndMatrWorld, VG4_RndMatrView), VG4_RndMatrProj);
  for (i = 0; i < GObj->NumOfV; i++)
    pnts[i] = VG4_RndWorldToScreen(GObj->V[i]);

  /* ������ ������������ */
  for (i = 0; i < GObj->NumOfF; i++)
  {
    INT
      n1 = GObj->F[i][0],
      n2 = GObj->F[i][1],
      n3 = GObj->F[i][2];
              
    MoveToEx(VG4_Anim.hDC, pnts[n1].x, pnts[n1].y, NULL);
    LineTo(VG4_Anim.hDC, pnts[n2].x, pnts[n2].y);
    LineTo(VG4_Anim.hDC, pnts[n3].x, pnts[n3].y);
    LineTo(VG4_Anim.hDC, pnts[n1].x, pnts[n1].y);
  }

  free(pnts);
} /* End of 'VG4_RndGObjDraw' function */

/* ������� ������������ ������ ��-��� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4GOBJ *GObj;
 * ������������ ��������: ���.
 */
VOID VG4_RndGObjFree( vg4GOBJ *GObj )
{
  free(GObj->V);
  memset(GObj, 0, sizeof(vg4GOBJ));
} /* End of 'VG4_RndGObjFree' function */

/* END OF 'RENDER.C' FILE */

