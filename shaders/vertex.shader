#version 330 core

layout(location = 0) in vec4 position;
out vec2 FragPos;

void main() {
  FragPos = position.xy;
  gl_Position = position;
};