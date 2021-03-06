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

/* ������� */
extern MATR
  VG4_RndMatrWorld,
  VG4_RndMatrView,
  VG4_RndMatrProj,
  VG4_RndMatrWorldViewProj;

/* ������� ��������� ��������� ��� �������� */
extern MATR VG4_RndPrimMatrConvert;

/* ��������� ������������� */
extern DBL
  VG4_RndWp, VG4_RndHp,    /* ������� ������� ������������� */
  VG4_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������ �� ��������� */
extern UINT VG4_RndProg;

/* ������� �������� �������� ��� ����� ���������.
 * ���������:
 *   - ������� ����� �����:
 *       CHAR *FileNamePrefix;
 * ������������ ��������:
 *   (UINT) ����� ����������� ���������.
 */
UINT VG4_ShaderLoad( CHAR *FileNamePrefix );

/* ������� ����������� �������� ��� ���������.
 * ���������:
 *   - ����� ���������:
 *       UINT PrgId;
 * ������������ ��������: ���.
 */
VOID VG4_ShaderFree( UINT PrgId );

/***
 * ������ � �����������
 ***/

/* ���� ���������� */
typedef enum tagvg4PRIM_TYPE
{
  VG4_PRIM_TRIMESH,  /* �������� - ����� ������������� */
  VG4_PRIM_GRID      /* ���������� ����� ������������� (����� triangle strip) */
} vg4PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagvg4PRIM
{
  vg4PRIM_TYPE Type; /* ��� ��������� (VG4_PRIM_***) */
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfI;      /* ���������� �������� ��� ������ ��������� */
} vg4PRIM;

/* ��� �������� ���������� ��������� */
typedef struct tagvg4UV
{
  FLT U, V; /* ���������� ���������� */
} vg4UV;

/* ������� ������� ���������� ���������.
 * ���������:
 *   - ����� ������� ���������:
 *       FLT U, V;
 * ������������ ��������:
 *   (vg4UV) ����������� ����������.
 */
__inline vg4UV UVSet( FLT U, FLT V )
{
  vg4UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* ��� �������� ����� ������� ���������� */
typedef struct tagvg4VERTEX
{
  VEC P;    /* ������� ������� */
  vg4UV T;  /* ���������� ���������� ������� */
  VEC N;    /* ������� � ������� */
  COLOR C;  /* ���� ������� */
} vg4VERTEX;

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       vg4PRIM *Prim;
 *   - ��� ��������� (VG4_PRIM_***):
 *       vg4PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NoofV, NoofI;
 *   - ������ ������:
 *       vg4VERTEX *Vertices;
 *   - ������ ��������:
 *       INT *Indices;
 * ������������ ��������: ���.
 */
VOID VG4_PrimCreate( vg4PRIM *Prim, vg4PRIM_TYPE Type,
                     INT NoofV, INT NoofI, vg4VERTEX *Vertices, INT *Indices);

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       vg4PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID VG4_PrimFree( vg4PRIM *Prim );

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       vg4PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID VG4_PrimDraw( vg4PRIM *Prim );

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       vg4PRIM *Prim;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL VG4_PrimCreatePlane( vg4PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* ������� �������� ��������� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       vg4PRIM *Prim;
 *   - ����� �����:
 *       VEC �;
 *   - ������ �����:
 *       FLT R;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL VG4_PrimCreateSphere( vg4PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       vg4PRIM *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL VG4_PrimLoad( vg4PRIM *GObj, CHAR *FileName );

/***
 * ������ � ��������������� ���������
 ***/

/* �������������� ������ - ����� ���������� */
typedef struct tagvg4GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  vg4PRIM *Prims;      /* ������ ���������� */
} vg4GEOM;

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       vg4GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       vg4PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT VG4_GeomAddPrim( vg4GEOM *G, vg4PRIM *Prim );

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       vg4GEOM *G;
 * ������������ ��������: ���.
 */
VOID VG4_GeomFree( vg4GEOM *G );

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       vg4GEOM *G;
 * ������������ ��������: ���.
 */
VOID VG4_GeomDraw( vg4GEOM *G );

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       vg4GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL VG4_GeomLoad( vg4GEOM *G, CHAR *FileName );

/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
INT VG4_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */
