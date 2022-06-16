layout (location = 0) in vec2 aVertex;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aVertex, 0.0, 1.0); 
}  
