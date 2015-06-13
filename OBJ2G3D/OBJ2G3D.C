/* FILENAME: OBJ2G3D.C
 * PROGRAMMER: VG4
 * PURPOSE: OBJ to G3D converter.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "render.h"

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       vg4GEOM *G;
 *   - имя файла материалов:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_GeomLoad( vg4GEOM *G, CHAR *FileName );

/* Функция сохранения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       vg4GEOM *G;
 *   - имя файла материалов:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_GeomSave( vg4GEOM *G, CHAR *FileName );

/* The main  program function */
VOID main( INT ArgC , CHAR *ArgV[] )
{
  vg4GEOM G;

  printf("CGSG OBJ to G3D data converter\n");
  if (ArgC < 3)
  {
    printf("Usage:\n%s <inOBJ> <OutG3D>\n", ArgV[0]);
    return;
  }
  VG4_GeomLoad(&G, ArgV[1]);
  VG4_GeomSave(&G, ArgV[2]);
  VG4_GeomFree(&G);
} /* End of' main' function */


/* END OF 'OBJ2G3D.C' FILE */
