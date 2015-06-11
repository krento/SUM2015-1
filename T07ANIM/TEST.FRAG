/* Filename: TEST.FRAG: */

// версия языка шейдера
#version 330

// время
uniform float Time;

// выходные параметры - цвет рисования
out vec4 OutColor;

// входные параметры от вершинного шейдера
in vec4 DrawColor;
in vec3 DrawPosition;

void main( void )
{
  if (DrawPosition.y - 8 + 5 * sin(3 * Time) > 1.5)
    discard;
  OutColor = DrawColor + vec4(1, 1, 1, 1) * 0.03;
}
