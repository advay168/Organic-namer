#version 330 core

in vec2 vPos;
flat in vec2 vCenter;
flat in float vRadius;

out vec4 FragColor;

void main(){
	vec2 delta = vPos - vCenter;
	if (length(delta) < vRadius){
		FragColor = vec4(0.8, 0.3,0.2,1.0);
	}
	else{
		discard;
	}
}