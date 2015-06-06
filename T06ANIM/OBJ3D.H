/* FILENAME: OBJ3D.H
 * PROGRAMMER: VG4
 * PURPOSE: 3D object handle declaration module.
 * LAST UPDATE: 06.06.2015
 */


#ifndef __OBJ3D_H_
#define __OBJ3D_H_

#include "def.h"

/* Draw object functioln.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID ObjDraw( VOID );

/* Draw object functioln.
 * ARGUMENTS:
 *   - object (*.OBJ) file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if loading is success, FALSE otherwise.
 */
BOOL ObjLoad( CHAR *FileName );

#endif /* __OBJ3D_H_ */

/* END OF 'OBJ3D.H' FILE */

