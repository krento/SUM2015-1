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

  vg4GEOM Model; /* Модель для отображения */
  vg4GEOM Geom; /* Модель для отображения */
  vg4PRIM Pr;
  INT TextId;  /* Id текстуры */
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
  vg4VERTEX V[]= 
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {5, 0}, {0, 0, 1}, {1, 0, 1, 1}},
    {{0, 1, 0}, {0, 5}, {0, 0, 1}, {1, 1, 0, 1}},
    {{1, 1, 0}, {5, 5}, {0, 0, 1}, {1, 1, 0, 1}},
  };
  INT I[] = {0, 1, 2, 2, 1, 3};
  BYTE txt[2][2][3] =
  {
    {{255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {255, 255, 255}}
  };

  /* загружаем текстуру */
  Uni->TextId = VG4_TextureLoad("M.BMP");

  VG4_PrimCreate(&Uni->Pr, VG4_PRIM_TRIMESH, 4, 6, V, I);

  VG4_GeomLoad(&Uni->Model, "Elephant.g3d");
  VG4_RndPrimMatrConvert = MatrRotateX(-90);
  //VG4_GeomLoad(&Uni->Geom, "X6.G3D");
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
  VG4_GeomFree(&Uni->Model);
  VG4_GeomFree(&Uni->Geom);
  VG4_PrimFree(&Uni->Pr);
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
  INT i, j;

  VG4_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  for (i = 0; i < 1; i++)
    for (j = 0; j < 1; j++)
    {
      VG4_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
          MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
          MatrScale(0.1, 0.1, 0.1)),
          MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
          MatrTranslate(j * 1.30, 0, i * 1.30 + 100 * Ani->JZ));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      VG4_GeomDraw(&Uni->Model);
    }
  VG4_RndMatrWorld = MatrRotateY(30 * Ani->Time);
  //MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time));
  VG4_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  VG4_PrimDraw(&Uni->Pr);
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
