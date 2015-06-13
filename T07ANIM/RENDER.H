/* FILENAME: RENDER.H
 * PROGRAMMER: VG4
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Матрицы */
extern MATR
  VG4_RndMatrWorld,
  VG4_RndMatrView,
  VG4_RndMatrProj,
  VG4_RndMatrWorldViewProj;

/* Матрица изменения примитива при создании */
extern MATR VG4_RndPrimMatrConvert;

/* Параметры проецирования */
extern DBL
  VG4_RndWp, VG4_RndHp,    /* размеры обрасти проецирования */
  VG4_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT VG4_RndProg;

/* Функция загрузки шейдеров для одной программы.
 * АРНУМЕНТЫ:
 *   - префикс имени файла:
 *       CHAR *FileNamePrefix;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (UINT) номер загруженной программы.
 */
UINT VG4_ShaderLoad( CHAR *FileNamePrefix );

/* Функция уничтодения шейдеров для программы.
 * АРНУМЕНТЫ:
 *   - номер программы:
 *       UINT PrgId;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_ShaderFree( UINT PrgId );

/***
 * Работа с примитивами
 ***/

/* Виды примитивов */
typedef enum tagvg4PRIM_TYPE
{
  VG4_PRIM_TRIMESH,  /* примитив - набор треугольников */
  VG4_PRIM_GRID      /* регулярная сетка треугольников (через triangle strip) */
} vg4PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagvg4PRIM
{
  vg4PRIM_TYPE Type; /* Тип примитива (VG4_PRIM_***) */
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfI;      /* количество индексов для вывода примитива */
} vg4PRIM;

/* Тип хранения текстурных координат */
typedef struct tagvg4UV
{
  FLT U, V; /* текстурные координаты */
} vg4UV;

/* Функция задания текстурных координат.
 * АРГУМЕНТЫ:
 *   - новые значени координат:
 *       FLT U, V;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UV) построенные координаты.
 */
__inline vg4UV UVSet( FLT U, FLT V )
{
  vg4UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* Тип хранения одной вершины паримитива */
typedef struct tagvg4VERTEX
{
  VEC P;    /* позиция вершины */
  vg4UV T;  /* текстурные координаты вершины */
  VEC N;    /* нормаль в вершине */
  COLOR C;  /* цвет вершины */
} vg4VERTEX;

/* Функция создания примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       vg4PRIM *Prim;
 *   - тип примитива (VG4_PRIM_***):
 *       vg4PRIM_TYPE Type;
 *   - количество вершин и индексов:
 *       INT NoofV, NoofI;
 *   - массив вершин:
 *       vg4VERTEX *Vertices;
 *   - массив индексов:
 *       INT *Indices;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_PrimCreate( vg4PRIM *Prim, vg4PRIM_TYPE Type,
                     INT NoofV, INT NoofI, vg4VERTEX *Vertices, INT *Indices);

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       vg4PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_PrimFree( vg4PRIM *Prim );

/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       vg4PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_PrimDraw( vg4PRIM *Prim );

/* Функция создания примитива плоскость.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       vg4PRIM *Prim;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL VG4_PrimCreatePlane( vg4PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* Функция создания примитива сфера.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       vg4PRIM *Prim;
 *   - центр сферы:
 *       VEC С;
 *   - радиус сферы:
 *       FLT R;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL VG4_PrimCreateSphere( vg4PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4PRIM *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL VG4_PrimLoad( vg4PRIM *GObj, CHAR *FileName );

/***
 * Работа с геометрическими объектами
 ***/

/* Геометрический объект - набор примитивов */
typedef struct tagvg4GEOM
{
  INT NumOfPrimitives; /* количество примитивов */
  vg4PRIM *Prims;      /* массив примитивов */
} vg4GEOM;

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       vg4GEOM *G;
 *   - указатель на добавляемый примитив:
 *       vg4PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT VG4_GeomAddPrim( vg4GEOM *G, vg4PRIM *Prim );

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       vg4GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_GeomFree( vg4GEOM *G );

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       vg4GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_GeomDraw( vg4GEOM *G );

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       vg4GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL VG4_GeomLoad( vg4GEOM *G, CHAR *FileName );

/* Функция загрузки текстуры.
 * АРГУМЕНТЫ:
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT ) идентификатор OpenGL для текстуры.
 */
INT VG4_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
