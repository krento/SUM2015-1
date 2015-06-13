/* FILENAME: GEOM.C
 * PROGRAMMER: VG4
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       vg4GEOM *G;
 *   - указатель на добавляемый примитив:
 *       vg4PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT VG4_GeomAddPrim( vg4GEOM *G, vg4PRIM *Prim )
{
  vg4PRIM *new_bulk;

  /* Выделяем память под новый массив примитивов */
  if ((new_bulk = malloc(sizeof(vg4PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  /* Копируем старые примитивы */
  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(vg4PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  /* указываем на новый массив примитивлв */
  G->Prims = new_bulk;

  /* Сохраняем новый элемент */
  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'VG4_GeomAddPrim' function */

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       vg4GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_GeomFree( vg4GEOM *G )
{
  INT i;

  if (G->Prims != NULL)
  {
    for (i = 0; i < G->NumOfPrimitives; i++)
      VG4_PrimFree(&G->Prims[i]);
    free(G->Prims);
  }
  memset(G, 0, sizeof(vg4GEOM));
} /* End of 'VG4_GeomFree' function */

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       vg4GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_GeomDraw( vg4GEOM *G )
{
  INT i, loc;

  for (i = 0; i < G->NumOfPrimitives; i++)
  {
    glUseProgram(VG4_RndProg);
    loc = glGetUniformLocation(VG4_RndProg, "PartNo");
    if (loc != -1)
      glUniform1f(loc, i);
    glUseProgram(0);
    VG4_PrimDraw(&G->Prims[i]);
  }
} /* End of 'VG4_GeomDraw' function */

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       vg4GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL VG4_GeomLoad( vg4GEOM *G, CHAR *FileName )
{
  FILE *F;
  INT i, j, n;
  CHAR Sign[4];
  MATR M;
  static CHAR MtlName[300];

  memset(G, 0, sizeof(vg4GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(VG4_RndPrimMatrConvert));

  /* читаем сигнатуру */
  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  /* читаем количество примитивов в объекте */
  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  /* читаем примитивы */
  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    vg4VERTEX *Vert;
    vg4PRIM P;

    /* читаем количество вершин и индексов */
    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    /* читаем имя материала текущего примитива */
    fread(MtlName, 1, 300, F);

    /* выделяем память под вершины и индексы */
    if ((Vert = malloc(sizeof(vg4VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    /* читаем данные */
    fread(Vert, sizeof(vg4VERTEX), nv, F);
    /* конвертируем геометрию */
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = VecMulMatr(Vert[j].P, VG4_RndPrimMatrConvert);
      Vert[j].N = VecMulMatr3(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    /* заносим в примитив */
    VG4_PrimCreate(&P, VG4_PRIM_TRIMESH, nv, ni, Vert, Ind);

    free(Vert);

    /* добавляем примитив к объекту */
    VG4_GeomAddPrim(G, &P);
  }
  fclose(F);
  VG4_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'VG4_GeomDraw' function */

/* END OF 'GEOM.C' FILE */
