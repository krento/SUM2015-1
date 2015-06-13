/* Filename: TEST.FRAG: */

// верси€ €зыка шейдера
#version 330

// врем€
uniform float Time;
uniform float PartNo;

// выходные параметры - цвет рисовани€
out vec4 OutColor;

// входные параметры от вершинного шейдера
in vec4 DrawColor;
in vec3 DrawPosition;
in vec3 DrawNormal;
in vec2 DrawTexCoord;

/* “екстура */
uniform sampler2D DrawTexture;

void main( void )
{
  /*
  if (DrawPosition.y - 8 + 5 * sin(3 * Time) > 1.5)
    discard;
  */
  /*
  if (PartNo > sin(Time * 0.1) * 400)
    discard;
  */
  vec4 tc = texture2D(DrawTexture, DrawTexCoord);

  if (distance(DrawPosition, vec3(0, 0.5, 0)) < 0.88 - 0.30 * sin(Time))
    ;//discard;
  OutColor = tc * DrawColor * dot(normalize(DrawNormal), normalize(vec3(1, 1, 1)));
  //OutColor = vec4(DrawPosition, 1) * 2;

  //OutColor = vec4(1, 1, 1, 1);
}
