/* FILENAME: GLOBEDRAW.С
 * PROGRAMMER: VG4
 * PURPOSE: Globe drawing declaration module.
 * LAST UPDATE: 05.06.2015
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "image.h"
#include "globe.h"

/* Размер сетки */
#define N (30 * 5)
#define M (30 * 10)

/* Массив хранения координат */
static VEC Grid[N][M];

/* Изображение глобуса */
static IMAGE GlobeImage;

/* Функция поворота точки вокруг оси X */
VEC RotateX( VEC P, DOUBLE AngleDegree )
{
  DOUBLE a = AngleDegree * PI / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = P.X;
  r.Y = P.Y * co - P.Z * si;
  r.Z = P.Y * si + P.Z * co;
  return r;
} /* End of 'RotateX' function */

/* Функция векторного произведения */
VEC VecCrossVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.Y * B.Z - A.Z * B.Y;
  r.Y = A.Z * B.X - A.X * B.Z;
  r.Z = A.X * B.Y - A.Y * B.X;
  return r;
} /* End of 'VecCrossVec' function */

/* Функция вычитания векторов */
VEC VecSubVec( VEC A, VEC B )
{
  VEC r;

  r.X = A.X - B.X;
  r.Y = A.Y - B.Y;
  r.Z = A.Z - B.Z;
  return r;
} /* End of 'VecSubVec' function */

/* Построение сферы */
VOID GlobeBuild( VOID )
{
  INT i, j;
  DOUBLE phi, theta, R = 288, t = clock() / (DOUBLE)CLOCKS_PER_SEC;

  if (GlobeImage.hBm == NULL)
    ImageLoad(&GlobeImage, "GLOBE.BMP");

  for (i = 0; i < N; i++)
  {
    theta = i / (N - 1.0) * PI;
    for (j = 0; j < M; j++)
    {
      phi = j / (M - 1.0) * 2 * PI + t * 0.30;

      Grid[i][j].X = R * sin(theta) * sin(phi);
      Grid[i][j].Y = 0.88 * R * cos(theta);
      Grid[i][j].Z = R * sin(theta) * cos(phi);

      Grid[i][j] = RotateX(Grid[i][j], sin(t * 1) * 88);
    }
  }
} /* End of 'GlobeBuild' function */

/* Рисование четырехугольника */
VOID DrawQuad( HDC hDC, VEC P0, VEC P1, VEC P2, VEC P3, INT W, INT H )
{
  VEC Norm = VecCrossVec(VecSubVec(P3, P0), VecSubVec(P1, P0));
  POINT pnts[4];

  if (P3.X == P0.X && P3.Y == P0.Y && P3.Z == P0.Z)
    Norm = VecCrossVec(VecSubVec(P2, P0), VecSubVec(P1, P0));

  /* back-face culling */
  if (Norm.Z < 0)
    return;

  pnts[0].x = P0.X + W / 2;
  pnts[0].y = -P0.Y + H / 2;

  pnts[1].x = P1.X + W / 2;
  pnts[1].y = -P1.Y + H / 2;

  pnts[2].x = P2.X + W / 2;
  pnts[2].y = -P2.Y + H / 2;

  pnts[3].x = P3.X + W / 2;
  pnts[3].y = -P3.Y + H / 2;


  Polygon(hDC, pnts, 4);
} /* End of 'DrawQuad' function */

/* Рисование сферы */
VOID GlobeDraw( HDC hDC, INT W, INT H )
{
  INT i, j;

  SelectObject(hDC, GetStockObject(NULL_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));

  /*
  SelectObject(hDC, GetStockObject(BLACK_PEN));
  SelectObject(hDC, GetStockObject(NULL_BRUSH));
  */
  /*
  */
  srand(30);
  for (i = 0; i < N - 1; i++)
  {
    for (j = 0; j < M - 1; j++)
    {
      /* Проэцирование */
      INT
        ix = j * (GlobeImage.W - 1) / (M - 1),
        iy = i * (GlobeImage.H - 1) / (N - 1),
        x = W / 2 + Grid[i][j].X,
        y = H / 2 - Grid[i][j].Y;



      SetDCBrushColor(hDC, ImageGetP(&GlobeImage, ix, iy));
      DrawQuad(hDC, Grid[i][j], Grid[i + 1][j],
                    Grid[i + 1][(j + 1) % M], Grid[i][(j + 1) % M], W, H);


      /* Ellipse(hDC, x - 2, y - 2, x + 2, y + 2); */
      /* SetPixel(hDC, x, y, RGB(0, 0, 0)); */
    }
  }
} /* End of 'GlobeDraw' function */

/* END OF 'GLOBEDRAW.C' FILE */

