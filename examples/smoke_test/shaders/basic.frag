#version 410 core
in vec3 vColor;
out vec4 FragColor;

uniform float uTint;   // exercises setFloat; pulses brightness over time

void main() {
    FragColor = vec4(vColor * uTint, 1.0);
}
