in vec2 vPos;
flat in vec2 vFrom;
flat in vec2 vTo;
flat in float vThickness;

out vec4 FragColor;
  
void main()
{ 
    float dist = length(cross(vec3(vFrom - vPos, 0.0f), vec3(vTo-vPos, 0.0f))) / length(vTo - vFrom) / vThickness / 4.0f;
    FragColor = vec4(1.0f - dist * dist);
}

