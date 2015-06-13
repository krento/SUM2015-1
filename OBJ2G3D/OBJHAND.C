/* FILENAME: OBJHAND.C
 * PROGRAMMER: VG4
 * PURPOSE: Geometry object load functions.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "render.h"

/***
 * Вспомогательные функции разбиения потока ввода на части
 ***/

/* Размер буфера */
#define MAX_BUF_SIZE 10000

/* Буфер ввода */
static CHAR Buf[MAX_BUF_SIZE];

/* Части буфера */
static INT NumOfParts;
static CHAR *Parts[MAX_BUF_SIZE / 2];

/* Функция разбиения строки на части.
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) количество частей.
 */
static INT Split( VOID )
{
  INT i = 0;

  NumOfParts = 0;
  Parts[0] = Buf;
  while (Buf[i] != 0)
  {
    /* пропускаем пробелы */
    while (isspace(Buf[i]))
      i++;
    /* уходим по окончании строки */
    if (Buf[i] == 0)
      return NumOfParts;

    /* запоминаем начало новой части */
    Parts[NumOfParts++] = Buf + i;

    /* ищем конец слова */
    while (!isspace(Buf[i]) && Buf[i] != 0)
      i++;

    /* уходим по окончании строки */
    if (Buf[i] == 0)
      return NumOfParts;

    /* завершаем слово */
    Buf[i++] = 0;
  }
  return NumOfParts;
} /* End of 'Split' function */

/***
 * Массив переиндексации и хранения номеров вершин,
 * нормалей и текстурных координат
 ***/

/* Массив хранения */
struct
{
  INT
    Nv, Nn, Nt, /* номера индексов */
    Next;       /* указатель на следующий, или -1 */
} *VertexRefs;
INT NumOfVertexRefs, NumOfAllocedVertexRefs;

/* Массив указателей на первую хранимую вершину:
 *   VertexRefs[VertexRefsStart[Nv]] или -1 */
INT *VertexRefsStart;

/* Функция получения реального индекса вершины.
 * АРГУМЕНТЫ:
 *   - номера вершины, нормали и текстурных координат:
 *       INT Nv, Nn, Nt;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) найденный индекс.
 */
static INT GetVertexNo( INT Nv, INT Nt, INT Nn )
{
  INT *n = &VertexRefsStart[Nv];

  while (*n != -1 &&
         !(VertexRefs[*n].Nv == Nv &&
           VertexRefs[*n].Nn == Nn &&
           VertexRefs[*n].Nt == Nt))
    n = &VertexRefs[*n].Next;

  if (*n == -1)
  {
    if (NumOfVertexRefs >= NumOfAllocedVertexRefs)
      return 0; /* нет места для хранения вершины */
    *n = NumOfVertexRefs++;
    VertexRefs[*n].Nv = Nv;
    VertexRefs[*n].Nn = Nn;
    VertexRefs[*n].Nt = Nt;
    VertexRefs[*n].Next = -1;
  }
  return *n;
} /* End of 'GetVertexNo' function */

/* Части имени файла */
static CHAR
  ModelDrive[_MAX_DRIVE],
  ModelDir[_MAX_DIR],
  ModelFileName[_MAX_FNAME],
  ModelFileExt[_MAX_EXT],
  TexDrive[_MAX_DRIVE],
  TexDir[_MAX_DIR],
  TexFileName[_MAX_FNAME],
  TexFileExt[_MAX_EXT];
static CHAR
  VG4_MaterialName[300];


/* Вспомогательная макро-функция разбора индексов граней */
#define SCANF3(Str, Ref) \
  if (sscanf(Str, "%i/%i/%i", &Ref[0], &Ref[1], &Ref[2]) == 3) \
    Ref[0]--, Ref[1]--, Ref[2]--;                              \
  else                                                         \
    if (sscanf(Str, "%i//%i", &Ref[0], &Ref[2]) == 2)          \
      Ref[0]--, Ref[1] = -1, Ref[2]--;                         \
    else                                                       \
      if (sscanf(Str, "%i/%i", &Ref[0], &Ref[1]) == 2)         \
        Ref[0]--, Ref[1]--, Ref[2] = -1;                       \
      else                                                     \
        if (sscanf(Str, "%i", &Ref[0]) == 1)                   \
          Ref[0]--, Ref[1] = -1, Ref[2] = -1;                  \
        else                                                   \
          Ref[0] = -1, Ref[1] = -1, Ref[2] = -1;               \
  if (Ref[0] != -1 && Ref[0] < 0)                              \
    Ref[0] = vn + Ref[0] + 1;                                  \
  if (Ref[1] != -1 && Ref[1] < 0)                              \
    Ref[1] = vtn + Ref[1] + 1;                                 \
  if (Ref[2] != -1 && Ref[2] < 0)                              \
    Ref[2] = vnn + Ref[2] + 1

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       vg4GEOM *G;
 *   - имя файла материалов:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_GeomLoad( vg4GEOM *G, CHAR *FileName )
{
  INT vn = 0, vtn = 0, vnn = 0, fn = 0, pn = 0, size, i, p;
  FILE *F;
  /* читаемые данные */
  VEC *ReadV, *ReadN;
  vg4UV *ReadUV;
  INT (*ReadF)[3];
  /* хранение примитивов */
  struct
  {
    INT
      First, Last;     /* первый и последний номера вершин примитива */
      /* Mtl; */       /* материал примитива */
    CHAR MtlName[300]; /* имя материала */
  } *PrimInfo;

  memset(G, 0, sizeof(vg4GEOM));
  /* разбиваем имя на части и открываем файл */
  _splitpath(FileName, ModelDrive, ModelDir, ModelFileName, ModelFileExt);
  printf("Start processing: %s\n", FileName);
  if ((F = fopen(FileName, "rt")) == NULL)
    return FALSE;

  /* считаем количества */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      vn++;
    else if (Buf[0] == 'v' && Buf[1] == 't')
      vtn++;
    else if (Buf[0] == 'v' && Buf[1] == 'n')
      vnn++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      fn += Split() - 3;
    else if (strncmp(Buf, "usemtl", 6) == 0)
      pn++;

  if (pn == 0)
    pn = 1; /* материалы не использовались */

  /* загружаем:
   *   вершины                        vn
   *   нормали                        vvn
   *   текстурные координаты          vtn
   *   треугольники                   fn
   *   примитивы                      pn
   * дополнительно:
   *   индексы (Vv, Vn, Vt) <- новые номера вершин  ? (vn + vt + vnn) * ???
   *   начальные индексы              vn
   */

  /* выделяем память под вспомогательные данные */
  size = 
    sizeof(VEC) * vn +                        /*   вершины               vn */
    sizeof(VEC) * vnn +                       /*   нормали               vnn */
    sizeof(vg4UV) * vtn +                     /*   текстурные координаты vtn */
    sizeof(INT [3]) * fn +                    /*   треугольники          fn */
    sizeof(PrimInfo[0]) * pn +                /*   примитивы             pn */
    sizeof(VertexRefs[0]) * (vn + vtn + vnn) + /*   индексы (Vv, Vn, Vt)  (vn + vt + vnn) */
    sizeof(INT) * vn;                         /*   начальные индексы     vn */
  if ((ReadV = malloc(size)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  memset(ReadV, 0, size);
  printf("Read: %i vertices, %i normals, %i texture coordinates, %i facets, %i parts\nBuild data\n",
    vn, vnn, vtn, fn, pn);

  /* расставляем указатели */
  ReadN = ReadV + vn;
  ReadUV = (vg4UV *)(ReadN + vnn);
  ReadF = (INT (*)[3])(ReadUV + vtn);
  VertexRefsStart = (INT *)(ReadF + fn);
  PrimInfo = (VOID *)(VertexRefsStart + vn);
  VertexRefs = (VOID *)(PrimInfo + pn);
  NumOfAllocedVertexRefs = vn + vtn + vnn;
  NumOfVertexRefs = 0;
  /* начала списка индексов вершин ==> -1 */
  memset(VertexRefsStart, 0xFF, sizeof(INT) * vn);
  memset(VertexRefs, 0xFF, sizeof(VertexRefs[0]) * NumOfAllocedVertexRefs);

  /* второй проход - читаем геометрию */
  rewind(F);
  vn = 0;
  vtn = 0;
  vnn = 0;
  fn = 0;
  pn = 0;
  PrimInfo[0].First = 0;

  /* читаем данные */
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      /* вершины */
      FLT x = 0, y = 0, z = 0;

      sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
      ReadV[vn++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'v' && Buf[1] == 't')
    {
      /* текстурные координаты вершин */
      FLT u = 0, v = 0;

      sscanf(Buf + 3, "%f%f", &u, &v);
      ReadUV[vtn++] = VG4_UVSet(u, v);
    }
    else if (Buf[0] == 'v' && Buf[1] == 'n')
    {
      /* нормали вершин */
      FLT nx = 0, ny = 0, nz = 0;

      sscanf(Buf + 3, "%f%f%f", &nx, &ny, &nz);
      ReadN[vnn++] = VecNormalize(VecSet(nx, ny, nz));
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      /* грани */
      INT n0[3], n1[3], n[3], r0, r1, r;

      /* первая вершина */
      Split();
      SCANF3(Parts[1], n0);
      r0 = GetVertexNo(n0[0], n0[1], n0[2]);

      /* вторая вершина */
      SCANF3(Parts[2], n1);
      r1 = GetVertexNo(n1[0], n1[1], n1[2]);

      for (i = 3; i < NumOfParts; i++)
      {
        /* очередная вершина */
        SCANF3(Parts[i], n);
        r = GetVertexNo(n[0], n[1], n[2]);

        ReadF[fn][0] = r0;
        ReadF[fn][1] = r1;
        ReadF[fn][2] = r;
        r1 = r;
        fn++;
      }
    }
    else if (strncmp(Buf, "usemtl", 6) == 0)
    {
      /* смена материала - новый примитив */
      Split();

      /* запоминаем номер последней грани */
      if (pn != 0)
        PrimInfo[pn - 1].Last = fn - 1;

      /* ищем материал */
      /*
      for (i = 0; i < G->NumOfMtls; i++)
        if (strcmp(Parts[1], G->Mtls[i].Name) == 0)
          break;
      if (i == G->NumOfMtls)
        PrimInfo[pn].Mtl = 0;
      else
        PrimInfo[pn].Mtl = i;
      */
      strcpy(PrimInfo[pn].MtlName, Parts[1]);
      PrimInfo[pn].First = fn;
      pn++;
    }
    else if (strncmp(Buf, "mtllib ", 7) == 0)
    {
      Split();
      strcpy(VG4_MaterialName, Parts[1]);
      /* LoadMaterials(G, Parts[1]); */
    }

  /* у последнего примитива запоминаем номер последней грани */
  if (pn == 0)
  {
    /* не было материалов */
    PrimInfo[0].Last = fn - 1;
    /* PrimInfo[0].Mtl = 0; */
    PrimInfo[0].MtlName[0] = 0;
    /* LoadMaterials(G, NULL); */
  }
  else
    PrimInfo[pn - 1].Last = fn - 1;
  fclose(F);

  printf("Use material: %s\n", VG4_MaterialName);

  /* Формируем примитивы из прочитанных данных */
  VG4_DefaultColor = ColorSet(1, 1, 1, 1);
  for (p = 0; p < pn; p++)
  {
    INT minv, maxv, j;
    vg4PRIM prim;
    BOOL is_need_normal = FALSE;

    minv = maxv = ReadF[PrimInfo[p].First][0];
    for (i = PrimInfo[p].First; i <= PrimInfo[p].Last; i++)
      for (j = 0; j < 3; j++)
      {
        if (minv > ReadF[i][j])
          minv = ReadF[i][j];
        if (maxv < ReadF[i][j])
          maxv = ReadF[i][j];
      }
    vn = maxv - minv + 1;
    fn = PrimInfo[p].Last - PrimInfo[p].First + 1;
    VG4_PrimCreate(&prim, VG4_PRIM_TRIMESH, vn, fn * 3);

    /* копируем вершины */
    for (i = 0; i < vn; i++)
    {
      INT n;

      prim.V[i].P = ReadV[VertexRefs[minv + i].Nv];
      if ((n = VertexRefs[minv + i].Nn) != -1)
        prim.V[i].N = ReadN[n];
      else
        is_need_normal = TRUE;
      if ((n = VertexRefs[minv + i].Nt) != -1)
        prim.V[i].T = ReadUV[n];
    }
    /* копируем грани */
    for (i = 0; i < fn; i++)
      for (j = 0; j < 3; j++)
        prim.I[i * 3 + j] = ReadF[PrimInfo[p].First + i][j] - minv;
    if (is_need_normal)
      VG4_PrimAutoNormals(&prim);
    /* prim.Mtl = PrimInfo[p].Mtl; */
    strcpy(prim.MtlName, PrimInfo[p].MtlName);
    VG4_GeomAddPrim(G, &prim);
    printf("Part # %i (%i vertices, %i facets, material: \"%s\"\n",
      p, prim.NumOfV, prim.NumOfI, prim.MtlName);
  }
  /* Сортируем примитивы обратно по прозрачности */
  /*
  for (p = 0; p < pn - 1; p++)
  {
    INT min = p, j;
    vg4PRIM tmp;

    for (j = p + 1; j < pn; j++)
      if (G->Prims[j].Mtl > 0 ||
          G->Mtls[G->Prims[j].Mtl].Trans > G->Mtls[G->Prims[p].Mtl].Trans)
        min = j;
    tmp = G->Prims[min];
    G->Prims[min] = G->Prims[p];
    G->Prims[p] = tmp;
  }
  */
  /* освобождаем память из-под прочитанных данных */
  free(ReadV);
  printf("Load done\n");
  return TRUE;
} /* End of 'VG4_GeomLoad' function */

/* Функция сохранения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       vg4GEOM *G;
 *   - имя файла материалов:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_GeomSave( vg4GEOM *G, CHAR *FileName )
{
  INT p;
  FILE *F;

  if ((F = fopen(FileName, "wb")) == NULL)
    return FALSE;

  printf("Save to '%s' ... ", FileName);
  fwrite("G3D", 1, 4, F);
  fwrite(&G->NumOfPrims, 4, 1, F);
  fwrite(VG4_MaterialName, 1, 300, F);
  for (p = 0; p < G->NumOfPrims; p++)
  {
    fwrite(&G->Prims[p].NumOfV, 4, 1, F);
    fwrite(&G->Prims[p].NumOfI, 4, 1, F);
    fwrite(G->Prims[p].MtlName, 1, 300, F);

    fwrite(G->Prims[p].V, sizeof(vg4VERTEX), G->Prims[p].NumOfV, F);
    fwrite(G->Prims[p].I, sizeof(INT), G->Prims[p].NumOfI, F);

  }
  fclose(F);
  printf("done\n");
  return TRUE;
} /* End of 'VG4_GeomLoad' function */


/* END OF 'OBJHAND.C' FILE */
