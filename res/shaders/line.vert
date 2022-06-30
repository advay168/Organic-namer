layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aFromTo;
layout (location = 2) in float aThickness;
layout (location = 3) in float aDashLength;

uniform mat4 view;
uniform mat4 projection;

out vec2 vPos;
flat out vec2 vFrom;
flat out vec2 vTo;
flat out float vThickness;
flat out float vDashLength;

void main()
{
    gl_Position = projection *  view * vec4(aPos, 0.0, 1.0); 
    vPos = aPos;
    vFrom = aFromTo.xy;
    vTo = aFromTo.zw;
    vThickness = aThickness;
    vDashLength = aDashLength;
}  
