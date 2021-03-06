/* FILENAME: PRIM.C
 * PROGRAMMER: VG4
 * PURPOSE: Primtive handle functions.
 * LAST UPDATE: 16.06.2014
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "render.h"

/* ������� */
#define power(A, B) ((A) < 0 ? -pow(-A, B) : pow(A, B))

/* ���� ��-��������� */
COLOR VG4_DefaultColor = {1, 1, 1, 1};

/* ������� �������� ���������.
 * ���������:
 *   - ����������� ��������:
 *       vg4PRIM *P;
 *   - ��� ���������:
 *       vg4PRIM_TYPE Type;
 *   - ��������� ����� (������, ������) ���
 *     Type == VG4_PRIM_GRID ��� ���������� ������
 *     � ��������:
 *       INT NumOfV_GW, NumOfI_GH;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL VG4_PrimCreate( vg4PRIM *P, vg4PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH )
{
  INT i, j, nv, ni, size;

  memset(P, 0, sizeof(vg4PRIM));
  /* ����������� ��������� */
  switch (Type)
  {
  case VG4_PRIM_TRIMESH:
    nv = NumOfV_GW;
    ni = NumOfI_GH;
    break;
  case VG4_PRIM_GRID:
    nv = NumOfV_GW * NumOfI_GH;
    ni = NumOfV_GW * 2;
    break;
  }
  /* �������� ������ */
  size = sizeof(vg4VERTEX) * nv + sizeof(INT) * ni;
  P->V = malloc(size);
  if (P->V == NULL)
    return FALSE;
  memset(P->V, 0, size);
  
  /* ��������� �������� */
  P->Size = size;
  P->Type = Type;
  P->NumOfV = nv;
  P->NumOfI = ni;
  P->GW = NumOfV_GW;
  P->GH = NumOfI_GH;
  P->I = (INT *)(P->V + nv);
  /* ��������� ��� ������� ��-��������� */
  for (i = 0; i < nv; i++)
  {
    P->V[i].C = VG4_DefaultColor;
    P->V[i].N = VecSet(0, 1, 0);
  }

  /* ��������� ������� � �������� ��� ���. ����� */
  if (Type == VG4_PRIM_GRID)
  {
    for (i = 0; i < NumOfI_GH; i++)
      for (j = 0; j < NumOfV_GW; j++)
        P->V[i * NumOfV_GW + j].T =
          VG4_UVSet(j / (NumOfV_GW - 1.0),
                    i / (NumOfI_GH - 1.0));
    for (i = 0; i < NumOfV_GW; i++)
    {
      P->I[2 * i + 0] = NumOfV_GW + i;
      P->I[2 * i + 1] = i;
    }
  }
  return TRUE;
} /* End of 'VG4_PrimCreate' function */

/* ������� �������� ���������.
 * ���������:
 *   - ��������� ��������:
 *       vg4PRIM *P;
 * ������������ ��������: ���.
 */
VOID VG4_PrimFree( vg4PRIM *P )
{
  if (P->V != NULL)
    free(P->V);
  memset(P, 0, sizeof(vg4PRIM));
} /* End of 'VG4_PrimFree' function */

/* ������� �������������� �������� � ���������.
 * ���������:
 *   - ��������:
 *       vg4PRIM *P;
 * ������������ ��������: ���.
 */
VOID VG4_PrimAutoNormals( vg4PRIM *P )
{
  INT i;

  if (P->Type == VG4_PRIM_TRIMESH)
  {
    /* �������� ��� ������� ������ */
    for (i = 0; i < P->NumOfV; i++)
      P->V[i].N = VecSet(0, 0, 0);

    /* ��������� ������� ���� ������ � �������� �� � ���������� �� �������� */
    for (i = 0; i < P->NumOfI / 3; i++)
    {
      INT *n = P->I + i * 3;
      VEC norm;

      norm =
        VecNormalize(VecCrossVec(VecSubVec(P->V[n[1]].P, P->V[n[0]].P),
                                 VecSubVec(P->V[n[2]].P, P->V[n[0]].P)));
      P->V[n[0]].N = VecAddVec(P->V[n[0]].N, norm);
      P->V[n[1]].N = VecAddVec(P->V[n[1]].N, norm);
      P->V[n[2]].N = VecAddVec(P->V[n[2]].N, norm);
    }

    /* ��������� */
    for (i = 0; i < P->NumOfV; i++)
      P->V[i].N = VecNormalize(P->V[i].N);
  }
  else
  {
  }
} /* End of 'VG4_PrimAutoNormals' function */

/* END OF 'PRIM.C' FILE */
