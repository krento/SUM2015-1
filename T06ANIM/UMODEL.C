/* FILENAME: UMODEL.C
 * PROGRAMMER: VG4
 * PURPOSE: Model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

/* ��� ������������� ���� */
typedef struct tagvg4UNIT_MODEL
{
  VG4_UNIT_BASE_FIELDS;

  vg4GOBJ Model; /* ������ ��� ����������� */
} vg4UNIT_MODEL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_MODEL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitInit( vg4UNIT_MODEL *Uni, vg4ANIM *Ani )
{
  VG4_RndGObjLoad(&Uni->Model, "cow.object");
} /* End of 'VG4_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_MODEL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitClose( vg4UNIT_MODEL *Uni, vg4ANIM *Ani )
{
  VG4_RndGObjFree(&Uni->Model);
} /* End of 'VG4_AnimUnitClose' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_MODEL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitRender( vg4UNIT_MODEL *Uni, vg4ANIM *Ani )
{
  INT i;

  VG4_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  for (i = 0; i < 8; i++)
  {
    VG4_RndMatrWorld =
      MatrMulMatr(MatrMulMatr(MatrMulMatr(
        MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
        MatrScale(0.1, 0.1, 0.1)),
        MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
        MatrTranslate((i - 2) * 2, 0, 100 * Ani->JZ));
    VG4_RndGObjDraw(&Uni->Model);
  }
} /* End of 'VG4_AnimUnitRender' function */

/* ������� �������� ������� �������� "������".
 * ���������: ���.
 * ������������ ��������:
 *   (vg4UNIT *) ��������� �� ��������� ������ ��������.
 */
vg4UNIT * VG4_UnitModelCreate( VOID )
{
  vg4UNIT_MODEL *Uni;

  if ((Uni = (VOID *)VG4_AnimUnitCreate(sizeof(vg4UNIT_MODEL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)VG4_AnimUnitInit;
  Uni->Close = (VOID *)VG4_AnimUnitClose;
  Uni->Render = (VOID *)VG4_AnimUnitRender;
  return (vg4UNIT *)Uni;
} /* End of 'VG4_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */
