/* FILENAME: GEOMHAND.C
 * PROGRAMMER: VG4
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 14.06.2014
 */

#include <stdlib.h>

#include "render.h"

/* Функция добавления материала к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       vg4GEOM *G;
 *   - добавляемый материал:
 *       vg4MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного материала или -1
 *         при ошибке.
 */
INT VG4_GeomAddMaterial( vg4GEOM *G, vg4MATERIAL *Mtl )
{
  INT i;
  vg4MATERIAL *M;

  /* ищем материал в библиотеке */
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
    /* копируем старые данные */
    memcpy(M, G->Mtls, sizeof(vg4MATERIAL) * G->NumOfMtls);
    /* освобождаем память */
    free(G->Mtls);
  }
  /* добавляем */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'VG4_GeomAddMaterial' function */

/* Функция добавления примитива к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       vg4GEOM *G;
 *   - добавляемый примитив:
 *       vg4PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива или -1
 *         при ошибке.
 */
INT VG4_GeomAddPrim( vg4GEOM *G, vg4PRIM *Prim )
{
  vg4PRIM *P;

  if ((P = malloc(sizeof(vg4PRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* копируем старые данные */
    memcpy(P, G->Prims, sizeof(vg4PRIM) * G->NumOfPrims);
    /* освобождаем память */
    free(G->Prims);
  }
  /* добавляем */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'VG4_GeomAddPrim' function */

/* Функция освобождения.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       vg4GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция преобразования вершин.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       vg4GEOM *G;
 *   - матрица преобразования:
 *       MATR M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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

/* Функция получения габаритов.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       vg4GEOM *G;
 *   - вектора границ ограничевающего объема:
 *       VEC *Min, *Max;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
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
