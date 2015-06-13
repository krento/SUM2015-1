/* FILENAME: RENDER.H
 * PROGRAMMER: VG4
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* Тип представления камеры в пространстве */
typedef struct
{
  VEC
    Loc,             /* точка расположения */
    Dir, Up, Right,  /* ориентационные вектора */
    At;              /* точка наблюдения */
} vg4CAMERA;

/* Тип представления объекта - набора вершин и треугольников и ребер */
typedef struct tagvg4GOBJ
{
  VEC *V;      /* массив вершин объекта */
  INT NumOfV;  /* количество вершин */
  INT (*F)[3]; /* массив граней объекта */
  INT NumOfF;  /* количество граней */
} vg4GOBJ;

/* Функция загрузки геометрического объекта */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName );

/* Функция освобождения геометрического объекта */
VOID VG4_RndGObjFree( vg4GOBJ *GObj );

/* Функция отрисовки геометрического объекта */
VOID VG4_RndGObjDraw( vg4GOBJ *GObj );

/***
 * Работа с геометрическими объектами
 ***/

/* Тип представления текстурных координат */
typedef struct tagvg4UV
{
  FLT U, V; /* компоненты текстурных координат */
} vg4UV;

/* Функиця инициализации текстурных координат */
__inline vg4UV VG4_UVSet( FLT U, FLT V )
{
  vg4UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'VG4_UVSet' function */

/* Теп представления вершины */
typedef struct tagvg4VERTEX
{
  VEC P;    /* координаты */
  vg4UV T;  /* текстурные координаты */
  VEC N;    /* нормаль */
  COLOR C;  /* цвет */
} vg4VERTEX;


/* Тип примитива - сетка треугольников или регулярная сетка вершин */
typedef enum tagvg4PRIM_TYPE
{
  VG4_PRIM_TRIMESH, VG4_PRIM_GRID
} vg4PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagvg4PRIM
{
  vg4PRIM_TYPE Type; /* тип примитива */
  INT
    NumOfV,     /* количество вершин */
    NumOfI,     /* количество индексов */
    GW, GH,     /* размер сетки */
    Mtl,        /* номер материала */
    Size;       /* размер выделенной памяти в байтах */
  vg4VERTEX *V; /* массив вершин */
  INT *I;       /* массив индексов */

  /* Данные для вывода в OpenGL */
  INT
    Buffers[2],    /* Буфер вершин и индексов */
    VertexBuffer;  /* Буфер описания вершин */
  CHAR MtlName[300]; /* Имя материала */
} vg4PRIM;

/* Цвет по-умолчанию */
extern COLOR VG4_DefaultColor;

/* Функция создания примитива */
BOOL VG4_PrimCreate( vg4PRIM *P, vg4PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* Функция создания сферы */
BOOL VG4_PrimCreateSphere( vg4PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* Функция создания цилиндра */
BOOL VG4_PrimCreateCylinder( vg4PRIM *P, FLT H, FLT R, INT M, INT N );

/* Функция создания карты высот */
BOOL VG4_PrimCreateHeightField( vg4PRIM *P, CHAR *FileName, FLT Height, FLT Scale );

/* Функция удаления примитива */
VOID VG4_PrimFree( vg4PRIM *P );

/* Функция отрисовки примитива */
VOID VG4_PrimDraw( vg4PRIM *P );

/* Функция перевычисления нормалей у примитива */
VOID VG4_PrimAutoNormals( vg4PRIM *P );

/* Максимальная длина имени */
#define MAX_STR 300

/* Тип представления материала */
typedef struct tagvg4MATERIAL
{
  VEC Ka, Kd, Ks;  /* коэффициенты освещения */
  FLT Phong;       /* коэффициент Фонга */
  FLT Trans;       /* коэффициент прозрачности */
  CHAR
    Name[MAX_STR], /* имя материала */
    MapD[MAX_STR]; /* имя текстуры */
  INT TexNo;       /* идентификатор текстуры в OpenGL */
} vg4MATERIAL;

/* Геометрический объект - набор примитивов */
typedef struct tagvg4GEOM
{
  INT NumOfPrims, NumOfMtls; /* размеры массивов */
  vg4MATERIAL *Mtls;         /* таблица материалов */
  vg4PRIM *Prims;            /* массив примитивов */
} vg4GEOM;

/* Функция добавления материала к объекту */
INT VG4_GeomAddMaterial( vg4GEOM *G,
                         vg4MATERIAL *Mtl );

/* Функция добавления примитива к объекту */
INT VG4_GeomAddPrim( vg4GEOM *G, vg4PRIM *Prim );

/* Функция освобождения геометрического объекта */
VOID VG4_GeomFree( vg4GEOM *G );

/* Функция рисования */
VOID VG4_GeomDraw( vg4GEOM *G );

/* Функция преобразования вершин */
VOID VG4_GeomTransform( vg4GEOM *G, MATR M );

/* Функция получения габаритов */
VOID VG4_GeomGetMinMax( vg4GEOM *G, VEC *Min, VEC *Max );

/* Функция загрузки геометрического объекта */
BOOL VG4_GeomLoad( vg4GEOM *G, CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
