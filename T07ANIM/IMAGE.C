/* FILENAME: IMAGE.H
 * PROGRAMMER: VG4
 * PURPOSE: Image handle declaration module.
 * LAST UPDATE: 05.06.2015
 */

#include <string.h>

#include "image.h"

/* Load image function.
 * ARGUMENTS:
 *   - image store data:
 *       IMAGE *Img;
 *   - image file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName )
{
  HDC hDC;
  BITMAP bm;

  memset(Img, 0, sizeof(IMAGE));

  /* Load image from file */
  Img->hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (Img->hBm == NULL)
    return FALSE;

  /* Create compatible context and select image into one */
  hDC = GetDC(NULL);
  Img->hDC = CreateCompatibleDC(hDC);
  ReleaseDC(NULL, hDC);

  SelectObject(Img->hDC, Img->hBm);

  /* Obtain image size */
  GetObject(Img->hBm, sizeof(BITMAP), &bm);
  Img->W = bm.bmWidth;
  Img->H = bm.bmHeight;
  return TRUE;
} /* End of 'ImageLoad' function */

/* Free memory from image function.
 * ARGUMENTS:
 *   - image store data:
 *       IMAGE *Img;
 * RETURNS: None.
 */
VOID ImageFree( IMAGE *Img )
{
  if (Img->hBm != NULL)
    DeleteObject(Img->hBm);
  if (Img->hDC != NULL)
    DeleteDC(Img->hDC);
  memset(Img, 0, sizeof(IMAGE));
} /* End of 'ImageFree' function */

/* Obtain image pixel color function.
 * ARGUMENTS:
 *   - image store data:
 *       IMAGE *Img;
 *   - pixel coordinates:
 *       INT X, Y;
 * RETURNS:
 *   (DWORD) TRUE if success, FALSE otherwise.
 */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y )
{
  if (Img->hBm == NULL)
    return 0;

  return GetPixel(Img->hDC, X, Y);
} /* End of 'ImageGetP' function */

/* END OF 'IMAGE.H' FILE */
