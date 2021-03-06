/* FILENAME: GEOMHAND.C
 * PROGRAMMER: VG4
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 14.06.2014
 */

#include <stdlib.h>

#include "render.h"

/* ������� ���������� ��������� � �������.
 * ���������:
 *   - �������������� ������:
 *       vg4GEOM *G;
 *   - ����������� ��������:
 *       vg4MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� ��� -1
 *         ��� ������.
 */
INT VG4_GeomAddMaterial( vg4GEOM *G, vg4MATERIAL *Mtl )
{
  INT i;
  vg4MATERIAL *M;

  /* ���� �������� � ���������� */
  for (i = 0; i < G->NumOfMtls; i++)
    if (strcmp(G->Mtls[i].Name, Mtl->Name)== 0)
    {
      G->Mtls[i] = *Mtl;
      return i;
    }

  if ((M = malloc(sizeof(vg4MATERIAL) * (G->NumOfMtls + 1))) == NULL)
    return -1;
  if (G->Mtls != NULL)
  {
    /* �������� ������ ������ */
    memcpy(M, G->Mtls, sizeof(vg4MATERIAL) * G->NumOfMtls);
    /* ����������� ������ */
    free(G->Mtls);
  }
  /* ��������� */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'VG4_GeomAddMaterial' function */

/* ������� ���������� ��������� � �������.
 * ���������:
 *   - �������������� ������:
 *       vg4GEOM *G;
 *   - ����������� ��������:
 *       vg4PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� ��� -1
 *         ��� ������.
 */
INT VG4_GeomAddPrim( vg4GEOM *G, vg4PRIM *Prim )
{
  vg4PRIM *P;

  if ((P = malloc(sizeof(vg4PRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* �������� ������ ������ */
    memcpy(P, G->Prims, sizeof(vg4PRIM) * G->NumOfPrims);
    /* ����������� ������ */
    free(G->Prims);
  }
  /* ��������� */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'VG4_GeomAddPrim' function */

/* ������� ������������.
 * ���������:
 *   - �������������� ������:
 *       vg4GEOM *G;
 * ������������ ��������: ���.
 */
VOID VG4_GeomFree( vg4GEOM *G )
{
  INT i;

  for (i = 0; i < G->NumOfPrims; i++)
    VG4_PrimFree(G->Prims + i);
  free(G->Prims);
  //for (i = 0; i < G->NumOfMtls; i++)
  //  glDeleteTextures(1, &G->Mtls[i].TexNo);
  free(G->Mtls);
  memset(G, 0, sizeof(vg4GEOM));
} /* End of 'VG4_GeomFree' function */

/* ������� �������������� ������.
 * ���������:
 *   - �������������� ������:
 *       vg4GEOM *G;
 *   - ������� ��������������:
 *       MATR M;
 * ������������ ��������: ���.
 */
VOID VG4_GeomTransform( vg4GEOM *G, MATR M )
{
  INT i, j;
  MATR InvM = MatrTranspose(MatrInverse(M));

  InvM.A[3][0] = InvM.A[3][1] = InvM.A[3][2] = 0;

  for (i = 0; i < G->NumOfPrims; i++)
    for (j = 0; j < G->Prims[i].NumOfV; j++)
    {
      G->Prims[i].V[j].P = VecMulMatr(G->Prims[i].V[j].P, M);
      G->Prims[i].V[j].N = VecMulMatr(G->Prims[i].V[j].N, InvM);
    }
} /* End of 'VG4_GeomTransform' function */

/* ������� ��������� ���������.
 * ���������:
 *   - �������������� ������:
 *       vg4GEOM *G;
 *   - ������� ������ ��������������� ������:
 *       VEC *Min, *Max;
 * ������������ ��������: ���.
 */
VOID VG4_GeomGetMinMax( vg4GEOM *G, VEC *Min, VEC *Max )
{
  INT i, j;

  if (G->NumOfPrims < 1 || G->Prims[0].NumOfV < 1)
    return;
  *Min = *Max = G->Prims[0].V[0].P;
  for (i = 0; i < G->NumOfPrims; i++)
    for (j = 0; j < G->Prims[i].NumOfV; j++)
    {
      if (Min->X > G->Prims[i].V[j].P.X)
        Min->X = G->Prims[i].V[j].P.X;
      if (Min->Y > G->Prims[i].V[j].P.Y)
        Min->Y = G->Prims[i].V[j].P.Y;
      if (Min->Z > G->Prims[i].V[j].P.Z)
        Min->Z = G->Prims[i].V[j].P.Z;
      if (Max->X < G->Prims[i].V[j].P.X)
        Max->X = G->Prims[i].V[j].P.X;
      if (Max->Y < G->Prims[i].V[j].P.Y)
        Max->Y = G->Prims[i].V[j].P.Y;
      if (Max->Z < G->Prims[i].V[j].P.Z)
        Max->Z = G->Prims[i].V[j].P.Z;
    }
} /* End of 'VG4_GeomGetMinMax' function */

/* END OF 'GEOMHAND.C' FILE */
