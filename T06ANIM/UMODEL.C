/* FILENAME: UMODEL.C
 * PROGRAMMER: VG4
 * PURPOSE: Model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

/* Тип представления мяча */
typedef struct tagvg4UNIT_MODEL
{
  VG4_UNIT_BASE_FIELDS;

  vg4GOBJ Model; /* Модель для отображения */
} vg4UNIT_MODEL;

/* Функция инициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID VG4_AnimUnitInit( vg4UNIT_MODEL *Uni, vg4ANIM *Ani )
{
  VG4_RndGObjLoad(&Uni->Model, "cow.object");
} /* End of 'VG4_AnimUnitInit' function */

/* Функция деинициализации объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID VG4_AnimUnitClose( vg4UNIT_MODEL *Uni, vg4ANIM *Ani )
{
  VG4_RndGObjFree(&Uni->Model);
} /* End of 'VG4_AnimUnitClose' function */

/* Функция построения объекта анимации.
 * АРГУМЕНТЫ:
 *   - указатель на "себя" - сам объект анимации:
 *       vg4UNIT_MODEL *Uni;
 *   - указатель на контекст анимации:
 *       vg4ANIM *Ani;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID VG4_AnimUnitRender( vg4UNIT_MODEL *Uni, vg4ANIM *Ani )
{
  VG4_RndMatrView = MatrView(VecSet(30, 30, 30),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  VG4_RndMatrWorld =
    MatrMulMatr(MatrMulMatr(MatrMulMatr(
      MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
      MatrScale(0.1, 0.1, 0.1)),
      MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
      MatrTranslate(0, 0, 100 * Ani->JZ));
  VG4_RndGObjDraw(&Uni->Model);
} /* End of 'VG4_AnimUnitRender' function */

/* Функция создания объекта анимации "модель".
 * АРГУМЕНТЫ: Нет.
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (vg4UNIT *) указатель на созданный объект анимации.
 */
vg4UNIT * VG4_UnitModelCreate( VOID )
{
  vg4UNIT_MODEL *Uni;

  if ((Uni = (VOID *)VG4_AnimUnitCreate(sizeof(vg4UNIT_MODEL))) == NULL)
    return NULL;
  /* заполняем поля */
  Uni->Init = (VOID *)VG4_AnimUnitInit;
  Uni->Close = (VOID *)VG4_AnimUnitClose;
  Uni->Render = (VOID *)VG4_AnimUnitRender;
  return (vg4UNIT *)Uni;
} /* End of 'VG4_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */
