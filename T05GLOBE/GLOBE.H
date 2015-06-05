/* FILENAME: GLOBE.H
 * PROGRAMMER: VG4
 * PURPOSE: Globe drawing declaration module.
 * LAST UPDATE: 05.06.2015
 */

#ifndef __GLOBE_H_
#define __GLOBE_H_

#include "def.h"

/* Пи */
#define PI 3.14159265358979323846

/* Тип хранения координат точки в пространстве */
typedef struct
{
  DOUBLE X, Y, Z;
} VEC;


/* Построение сферы */
VOID GlobeBuild( VOID );

/* Рисование сферы */
VOID GlobeDraw( HDC hDC, INT W, INT H );

#endif /* __GLOBE_H_ */

/* END OF 'GLOBE/H/ FILE */
