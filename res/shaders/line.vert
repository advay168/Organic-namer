layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aFrom;
layout (location = 2) in vec2 aTo;
layout (location = 3) in float aThickness;

uniform mat4 projection;

out vec2 vPos;
flat out vec2 vFrom;
flat out vec2 vTo;
flat out float vThickness;

void main()
{
    gl_Position = projection * vec4(aPos, 0.0, 1.0); 
    vPos = aPos;
    vFrom = aFrom;
    vTo = aTo;
    vThickness = aThickness;
}  
