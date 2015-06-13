/* FILENAME: RENDER.C
 * PROGRAMMER: VG4
 * PURPOSE: Rendering system module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

#pragma comment(lib, "glew32s")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")

/* Матрицы */
MATR
  VG4_RndMatrWorld = VG4_UNIT_MATR,
  VG4_RndMatrView = VG4_UNIT_MATR,
  VG4_RndMatrProj = VG4_UNIT_MATR,
  VG4_RndMatrWorldViewProj = VG4_UNIT_MATR;

/* Параметры проецирования */
DBL
  VG4_RndWp = 3, VG4_RndHp = 3,     /* размеры обрасти проецирования */
  VG4_RndProjDist = 5;              /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
UINT VG4_RndProg;

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4PRIM *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL VG4_PrimLoad( vg4PRIM *GObj, CHAR *FileName )
{
  FILE *F;
  vg4VERTEX *V;
  INT (*Facets)[3];
  INT nv = 0, nf = 0;
  static CHAR Buf[10000];

  memset(GObj, 0, sizeof(vg4PRIM));
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
  if ((V = malloc(sizeof(vg4VERTEX) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Facets = (INT (*)[3])(V + nv);
  memset(V, 0, sizeof(vg4VERTEX) * nv + sizeof(INT [3]) * nf);

  /* Read vertices */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%f%f%f",
        &V[nv].P.X, &V[nv].P.Y, &V[nv].P.Z);
      V[nv].C = ColorSet(Rnd0(), Rnd0(), Rnd0(), 1);
      nv++;
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT a, b, c;

      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 ||
              sscanf(Buf + 2, "%i %i %i", &a, &b, &c);

      Facets[nf][0] = a - 1;
      Facets[nf][1] = b - 1;
      Facets[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  VG4_PrimCreate(GObj, VG4_PRIM_TRIMESH, nv, nf * 3, V, (INT *)Facets);

  /* освобождаем оперативную память */
  free(V);
  return TRUE;
} /* End of 'VG4_PrimLoad' function */

/* Функция загрузки текстуры.
 * АРГУМЕНТЫ:
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT ) идентификатор OpenGL для текстуры.
 */
INT VG4_TextureLoad( CHAR *FileName )
{
  INT TexId = 0;
  HDC hMemDC;
  BITMAP bm;
  HBITMAP hBm;
  DWORD *Bits;

  /* загружаем изображение из файла */
  hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (hBm == NULL)
    return 0;

  /* Create compatible context and select image into one */
  hMemDC = CreateCompatibleDC(VG4_Anim.hDC);
  SelectObject(hMemDC, hBm);

  /* Obtain image size */
  GetObject(hBm, sizeof(BITMAP), &bm);
  if ((Bits = malloc(sizeof(DWORD) * bm.bmWidth * bm.bmHeight)) != NULL)
  {
    INT x, y, r, g, b;
    COLORREF c;

    for (y = 0; y < bm.bmHeight; y++)
      for (x = 0; x < bm.bmWidth; x++)
      {
        c = GetPixel(hMemDC, x, y);
        r = GetRValue(c);
        g = GetGValue(c);
        b = GetBValue(c);
        Bits[(bm.bmHeight - 1 - y) * bm.bmWidth + x] = 0xFF000000 | (r << 16) | (g << 8) | b;
      }
      glGenTextures(1, &TexId);
      glBindTexture(GL_TEXTURE_2D, TexId);
      gluBuild2DMipmaps(GL_TEXTURE_2D, 4, bm.bmWidth, bm.bmHeight,
        GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
      glBindTexture(GL_TEXTURE_2D, 0);
    free(Bits);
  }
  DeleteDC(hMemDC);
  DeleteObject(hBm);
  return TexId;
} /* End of 'VG4_TextureLoadfunction */


/* END OF 'RENDER.C' FILE */

