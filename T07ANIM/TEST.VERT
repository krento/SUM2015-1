/* Filename: TEST.VERT */

// версия языка шейдера
#version 330

// кто куда приходит
layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

// глобальные переменные
uniform mat4 MatrWorld;
uniform mat4 MatrView;
uniform mat4 MatrProj;
uniform mat4 MatrWVP;
uniform mat4 MatrWV;
uniform mat4 MatrWVInverse;
uniform float Time;

// выходные параметры
out vec4 DrawColor;
out vec3 DrawNormal;
out vec3 DrawPosition;
out vec2 DrawTexCoord;

void main( void )
{
  //gl_Position = MatrProj * MatrView * MatrWorld * vec4(InPosition, 1);
  gl_Position = MatrWVP * vec4(InPosition * 0.050, 1);
  //gl_Position = vec4(InPosition, 1);
  DrawNormal = mat3(MatrWVInverse) * InNormal;
  //gl_Position = vec4(0.3, 0, 0.3, 1);
  DrawColor = InColor;
  DrawPosition = mat3(MatrWV) * InPosition;
  DrawTexCoord = InTexCoord;
}
