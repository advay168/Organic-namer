in vec2 vPos;
flat in vec2 vFrom;
flat in vec2 vTo;
flat in float vThickness;
flat in float vDashLength;

uniform int frame;

out vec4 FragColor;

void main()
{
    vec2 line = vTo - vFrom;
    float distFrom = dot(vPos - vFrom, line) / dot(line,line);
    vec2 closest = vFrom + line * distFrom;
    float dist = length(vPos - closest) / vThickness;
    if (vDashLength != 0.0f){
        int multiple = int(distFrom * length(line) / vDashLength);
        if (multiple % 2 == frame % 2){
            discard;
        } 
    }
    FragColor = vec4(1 - dist * dist);
}

