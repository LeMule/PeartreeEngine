#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 _pos;
layout (location = 1) in vec4 _color;
layout (location = 2) in vec2 _texCoord;

out vec4 fragmentColor;
out vec2 TexCoord;

uniform mat4 transform;

uniform float x;
uniform float y;
uniform float z;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * (transform * vec4(_pos.x + x, _pos.y + y, _pos.z + z, 1.0));
   fragmentColor = _color;
   TexCoord = _texCoord;
}