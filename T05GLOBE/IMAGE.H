/* FILENAME: IMAGE.H
 * PROGRAMMER: VG4
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 05.06.2015
 */


#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

/* Image store data type */
typedef struct
{
  INT W, H;    /* Image size */
  HBITMAP hBm; /* Image handle */
  HDC hDC;     /* Image bitmap device context for drawing */
} IMAGE;

/* Load image function.
 * ARGUMENTS:
 *   - image store data:
 *       IMAGE *Img;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

/* Free memory from image function.
 * ARGUMENTS:
 *   - image store data:
 *       IMAGE *Img;
 * RETURNS: None.
 */
VOID ImageFree( IMAGE *Img );

/* Obtain image pixel color function.
 * ARGUMENTS:
 *   - image store data:
 *       IMAGE *Img;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) TRUE if success, FALSE otherwise.
 */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

#endif /* __IMAGE_H_ */

/* END OF 'IMAGE.H' FILE */
