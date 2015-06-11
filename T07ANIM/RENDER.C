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

typedef struct tagVERTEX
{
  VEC P;   /* позиция */
  COLOR C; /* Цвет */
} VERTEX;

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName )
{
  FILE *F;
  VERTEX *V;
  INT (*Facets)[3];
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
  if ((V = malloc(sizeof(VERTEX) * nv + sizeof(INT [3]) * nf)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  Facets = (INT (*)[3])(V + nv);

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

  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  /* отправляем в OpenGL */
  glGenVertexArrays(1, &GObj->VA);
  glGenBuffers(1, &GObj->VBuf);
  glGenBuffers(1, &GObj->IBuf);

  /* делаем активным массив вершин */
  glBindVertexArray(GObj->VA);
  /* делаем активным буфер вершин */
  glBindBuffer(GL_ARRAY_BUFFER, GObj->VBuf);
  /* сливаем данные */
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * GObj->NumOfV, V, GL_STATIC_DRAW);
  /* делаем активным буфер индексов */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GObj->IBuf);
  /* сливаем данные */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT [3]) * GObj->NumOfF, Facets, GL_STATIC_DRAW);

  /* задаем порядок данных */
  /*                    layout,
   *                       количество компонент,
   *                          тип,
   *                                    надо ли нормировать,
   *                                           размер в байтах одного элемента буфера,
   *                                                           смещение в байтах до начала данных */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)0); /* позиция */
  glVertexAttribPointer(1, 4, GL_FLOAT, FALSE, sizeof(VERTEX), (VOID *)sizeof(VEC)); /* цвет */

  /* включаем нужные аттрибуты (layout) */
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  /* освобождаем оперативную память */
  free(V);
  return TRUE;
} /* End of 'VG4_RndGObjLoad' function */

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_RndGObjDraw( vg4GOBJ *GObj )
{
  INT loc;

  VG4_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(VG4_RndMatrWorld, VG4_RndMatrView), VG4_RndMatrProj);

  glLoadMatrixf(VG4_RndMatrWorldViewProj.A[0]);

  /* рисуем треугольники */
  glBindVertexArray(GObj->VA);
  glUseProgram(VG4_RndProg);

  loc = glGetUniformLocation(VG4_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, VG4_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(VG4_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, VG4_RndMatrView.A[0]);
  loc = glGetUniformLocation(VG4_RndProg, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, VG4_RndMatrProj.A[0]);
  loc = glGetUniformLocation(VG4_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, VG4_Anim.Time);


  glDrawElements(GL_TRIANGLES, GObj->NumOfF * 3, GL_UNSIGNED_INT, NULL);
  glUseProgram(0);
  glBindVertexArray(0);
} /* End of 'VG4_RndGObjDraw' function */

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_RndGObjFree( vg4GOBJ *GObj )
{
  /* делаем активным массив вершин */
  glBindVertexArray(GObj->VA);
  /* "отцепляем" буфера */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &GObj->VBuf);
  glDeleteBuffers(1, &GObj->IBuf);
  /* делаем неактивным массив вершин */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &GObj->VA);

  memset(GObj, 0, sizeof(vg4GOBJ));
} /* End of 'VG4_RndGObjFree' function */

/* END OF 'RENDER.C' FILE */

