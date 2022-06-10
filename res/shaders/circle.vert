#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aCenter;
layout (location = 2) in float aRadius;
layout (location = 3) in vec3 aColor;

out vec2 vPos;
flat out vec2 vCenter;
flat out float vRadius;
out vec3 vColor;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    vPos = aPos.xy;
    vCenter = aCenter;
    vRadius = aRadius;
    vColor = aColor;
}
