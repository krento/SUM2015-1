/* FILENAME: RENDER.H
 * PROGRAMMER: VG4
 * PURPOSE: 3D render declaration module.
 * LAST UPDATE: 13.06.2014
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

/* ��� ������������� ������ � ������������ */
typedef struct
{
  VEC
    Loc,             /* ����� ������������ */
    Dir, Up, Right,  /* �������������� ������� */
    At;              /* ����� ���������� */
} vg4CAMERA;

/* ��� ������������� ������� - ������ ������ � ������������� � ����� */
typedef struct tagvg4GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} vg4GOBJ;

/* ������� �������� ��������������� ������� */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ��������������� ������� */
VOID VG4_RndGObjFree( vg4GOBJ *GObj );

/* ������� ��������� ��������������� ������� */
VOID VG4_RndGObjDraw( vg4GOBJ *GObj );

/***
 * ������ � ��������������� ���������
 ***/

/* ��� ������������� ���������� ��������� */
typedef struct tagvg4UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} vg4UV;

/* ������� ������������� ���������� ��������� */
__inline vg4UV VG4_UVSet( FLT U, FLT V )
{
  vg4UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'VG4_UVSet' function */

/* ��� ������������� ������� */
typedef struct tagvg4VERTEX
{
  VEC P;    /* ���������� */
  vg4UV T;  /* ���������� ���������� */
  VEC N;    /* ������� */
  COLOR C;  /* ���� */
} vg4VERTEX;


/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagvg4PRIM_TYPE
{
  VG4_PRIM_TRIMESH, VG4_PRIM_GRID
} vg4PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagvg4PRIM
{
  vg4PRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,     /* ���������� ������ */
    NumOfI,     /* ���������� �������� */
    GW, GH,     /* ������ ����� */
    Mtl,        /* ����� ��������� */
    Size;       /* ������ ���������� ������ � ������ */
  vg4VERTEX *V; /* ������ ������ */
  INT *I;       /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],    /* ����� ������ � �������� */
    VertexBuffer;  /* ����� �������� ������ */
  CHAR MtlName[300]; /* ��� ��������� */
} vg4PRIM;

/* ���� ��-��������� */
extern COLOR VG4_DefaultColor;

/* ������� �������� ��������� */
BOOL VG4_PrimCreate( vg4PRIM *P, vg4PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL VG4_PrimCreateSphere( vg4PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* ������� �������� �������� */
BOOL VG4_PrimCreateCylinder( vg4PRIM *P, FLT H, FLT R, INT M, INT N );

/* ������� �������� ����� ����� */
BOOL VG4_PrimCreateHeightField( vg4PRIM *P, CHAR *FileName, FLT Height, FLT Scale );

/* ������� �������� ��������� */
VOID VG4_PrimFree( vg4PRIM *P );

/* ������� ��������� ��������� */
VOID VG4_PrimDraw( vg4PRIM *P );

/* ������� �������������� �������� � ��������� */
VOID VG4_PrimAutoNormals( vg4PRIM *P );

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagvg4MATERIAL
{
  VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} vg4MATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagvg4GEOM
{
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  vg4MATERIAL *Mtls;         /* ������� ���������� */
  vg4PRIM *Prims;            /* ������ ���������� */
} vg4GEOM;

/* ������� ���������� ��������� � ������� */
INT VG4_GeomAddMaterial( vg4GEOM *G,
                         vg4MATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT VG4_GeomAddPrim( vg4GEOM *G, vg4PRIM *Prim );

/* ������� ������������ ��������������� ������� */
VOID VG4_GeomFree( vg4GEOM *G );

/* ������� ��������� */
VOID VG4_GeomDraw( vg4GEOM *G );

/* ������� �������������� ������ */
VOID VG4_GeomTransform( vg4GEOM *G, MATR M );

/* ������� ��������� ��������� */
VOID VG4_GeomGetMinMax( vg4GEOM *G, VEC *Min, VEC *Max );

/* ������� �������� ��������������� ������� */
BOOL VG4_GeomLoad( vg4GEOM *G, CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
