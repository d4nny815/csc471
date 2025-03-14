#version  330 core
layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

uniform vec3 lightPos;

out vec3 fragNor;
out vec3 lightDir;
out vec2 vTexCoord;
out vec3 EPos;

void main() {

  vec4 vPosition;

  /* First model transforms */
  gl_Position = P * V * M * vec4(vertPos.xyz, 1.0);

  fragNor = (V * M * vec4(vertNor, 0.0)).xyz;
  lightDir = lightPos - (V * M * vertPos).xyz;
  EPos = (V * M * vertPos).xyz;

  /* pass through the texture coordinates to be interpolated */
  vTexCoord = vertTex;
}
