/* Filename: TEST.VERT */

// версия языка шейдера
#version 330

// кто куда приходит
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec4 InColor;

// глобальные переменные
uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform float Time;

// выходные параметры
out vec4 DrawColor;
out vec3 DrawPosition;

void main( void )
{
  gl_Position = MatrProj * MatrView * MatrWorld * vec4(InPosition * 3, 1);
  // gl_Position = vec4(0.3, 0, 0.3, 1);
  DrawColor = InColor;
  DrawPosition = InPosition;
}
