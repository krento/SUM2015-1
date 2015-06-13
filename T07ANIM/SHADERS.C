/* FILENAME: SHADERS.C
 * PROGRAMMER: VG4
 * PURPOSE: Shader support module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <string.h>

#include "anim.h"
#include "render.h"

/* Функция записи ошибок в файл.
 * АРНУМЕНТЫ:
 *   - текст ошибки:
 *       CHAR *Text;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
static VOID VG4_SaveLog( CHAR *Text )
{
  FILE *F;

  if ((F = fopen("{_}SHD{30}.LOG", "a")) != NULL)
  {
    fprintf(F, "%s\n", Text);
    fclose(F);
  }
} /* End of 'VG4_SaveLog' function */

/* Функция загрузки текстового файла в память.
 * АРНУМЕНТЫ:
 *   - имея файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (CHAR *) загруженный текст.
 */
static CHAR * VG4_TextLoad( CHAR *FileName )
{
  FILE *F;
  CHAR *mem = NULL;

  /* Открываем текстовый файл */
  if ((F = fopen(FileName, "r")) != NULL)
  {
    LONG len;

    /* измеряем длину файла */
    fseek(F, 0, SEEK_END);
    len = ftell(F);

    /* выделяем память под текст */
    if ((mem = malloc(len + 1)) != NULL)
    {
      fseek(F, 0, SEEK_SET);
      /* загружаем файл в память */
      len = fread(mem, 1, len, F);
      mem[len] = 0;
    }
    fclose(F);
  }
  return mem;
} /* End of 'VG4_ShaderLoad' function */

/* Функция загрузки шейдеров для одной программы.
 * АРНУМЕНТЫ:
 *   - префикс имени файла:
 *       CHAR *FileNamePrefix;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (UINT) номер загруженной программы.
 */
UINT VG4_ShaderLoad( CHAR *FileNamePrefix )
{
  INT res, i;
  CHAR *txt;
  INT NumOfShaders = 2;
  UINT
    Shaders[2] = {0},
    ShTypes[2] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER},
     Prg = 0;
  CHAR *Suff[2] = {"VERT", "FRAG"};
  BOOL isok = TRUE;
  static CHAR Buf[1000];

  /* загружаем шейдера */

  for (i = 0; i < NumOfShaders; i++)
  {
    sprintf(Buf, "%s.%s", FileNamePrefix, Suff[i]);
    /* создаем в памяти шейдер */
    if ((Shaders[i] = glCreateShader(ShTypes[i])) == 0)
    {
      isok = FALSE;
      VG4_SaveLog("Error create shader");
      break;
    }
    /* загружаем файл */
    if ((txt = VG4_TextLoad(Buf)) == NULL)
    {
      isok = FALSE;
      VG4_SaveLog("Error load file");
      VG4_SaveLog(Buf);
      break;
    }
    /* закрепляем текст */
    glShaderSource(Shaders[i], 1, &txt, NULL);
    free(txt);
    /* компилируем шейдер */
    glCompileShader(Shaders[i]);
    glGetShaderiv(Shaders[i], GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      /* ошибка компиляции */
      glGetShaderInfoLog(Shaders[i], sizeof(Buf), &res, Buf);
      VG4_SaveLog(Buf);
      isok = FALSE;
      break;
    }
  }

  if (isok)
    /* Инициализируем программу - набор шейдеров */
    if ((Prg = glCreateProgram()) == 0)
      isok = FALSE;
    else
    {
      /* присоединяем к программе шейдера */
      for (i = 0; i < NumOfShaders; i++)
        if (Shaders[i] != 0)
          glAttachShader(Prg, Shaders[i]);
      /* компонуем программу */
      glLinkProgram(Prg);
      glGetProgramiv(Prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        /* ошибка компиляции */
        glGetProgramInfoLog(Prg, sizeof(Buf), &res, Buf);
        VG4_SaveLog(Buf);
        isok = FALSE;
      }
    }
  /* обработка ошибок */
  if (!isok)
  {
    for (i = 0; i < 2; i++)
      if (Shaders[i] != 0)
      {
        if (Prg != 0)
          glDetachShader(Prg, Shaders[i]);
        glDeleteShader(Shaders[i]);
      }
    if (Prg != 0)
      glDeleteProgram(Prg);
    return 0;
  }
  return Prg;
} /* End of 'VG4_ShaderLoad' function */

/* Функция уничтодения шейдеров для программы.
 * АРНУМЕНТЫ:
 *   - номер программы:
 *       UINT PrgId;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID VG4_ShaderFree( UINT PrgId )
{
  UINT i, n, shdrs[5];

  if (PrgId == 0)
    return;

  /* получаем присоединенные шейдера */
  glGetAttachedShaders(PrgId, 5, &n, shdrs);

  /* удаляем */
  for (i = 0; i < n; i++)
  {
    glDetachShader(PrgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(PrgId);
} /* End of 'VG4_ShaderFree' function */

/* END OF 'RENDER.C' FILE */
