/* FILENAME: VEC.H
 * PROGRAMMER: VG4
 * PURPOSE: 3D math declaration module.
 * LAST UPDATE: 06.06.2015
 */

#ifndef __VEC_H_
#define __VEC_H_

#include <math.h>

#include "def.h"

/*  онстанты и преобразовани€ дл€ углов */
#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define R2D(A) ((A) * (180.0 / PI))

/* ћножитель дл€ преобразовани€ градусов в радианы */
#define DEGREE2RADIANL 0.01745329251994329576L
extern LDBL MultiplierDegree2Radian;

/* тип дл€ вектора в простанстве */
typedef struct tagVEC
{
  DBL X, Y, Z; /* компоненты вектора */
} VEC;

/* тип дл€ матрицы - массив в структуре */
typedef struct tagMATR
{
  DBL A[4][4]; /* элементы матрицы */
} MATR;


#endif /* __VEC_H_ */

/* END OF 'VEC.H' FILE */
