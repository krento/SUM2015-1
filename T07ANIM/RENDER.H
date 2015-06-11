/* FILENAME: RENDER.H
 * PROGRAMMER: VG4
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 10.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* Тип представления объекта - набор вершин и треугольников */
typedef struct tagvg4GOBJ
{
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfV,      /* количество вершин */
    NumOfF;      /* количество граней */
} vg4GOBJ;

/* Матрицы */
extern MATR
  VG4_RndMatrWorld,
  VG4_RndMatrView,
  VG4_RndMatrProj;

/* Параметры проецирования */
extern DBL
  VG4_RndWp, VG4_RndHp,    /* размеры обрасти проецирования */
  VG4_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT VG4_RndProg;

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName );

/* Функция освобождения памяти из-под геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_RndGObjFree( vg4GOBJ *GObj );

/* Функция отрисовки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       vg4GOBJ *GObj;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_RndGObjDraw( vg4GOBJ *GObj );

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

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
