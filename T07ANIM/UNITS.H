/* FILENAME: UNITS.H
 * PROGRAMMER: VG4
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 10.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"

/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (vg4UNIT *) ��������� �� ��������� ������ ��������.
 */
vg4UNIT * VG4_UnitBallCreate( VOID );

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (vg4UNIT *) ��������� �� ��������� ������ ��������.
 */
vg4UNIT * VG4_UnitControlCreate( VOID );

/* ������� �������� ������� �������� "������".
 * ���������: ���.
 * ������������ ��������:
 *   (vg4UNIT *) ��������� �� ��������� ������ ��������.
 */
vg4UNIT * VG4_UnitModelCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
