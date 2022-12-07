#version 300 es

layout(location = 0) in vec3 inPosition;

uniform vec4 color;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform float time;

out vec4 fragColor;

void main() {
  vec3 posLocal = inPosition;
  float amp = 0.1f;
  float freq = 20.0f;
  posLocal.z += sin(freq + time) * amp;
  vec4 posEyeSpace = viewMatrix * modelMatrix * vec4(posLocal, 1);

  float i = 1.0 - (-posEyeSpace.z / 3.0);
  fragColor = vec4(i, i, i, 1) * color;

  gl_Position = projMatrix * posEyeSpace;
}