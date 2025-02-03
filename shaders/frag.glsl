R"(

#version 330
in vec3 transformedNormal;
in vec3 originalNormal;

uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

out vec4 outputColor;
void main() {
    vec3 color = originalNormal;
    float lighting = abs(dot(transformedNormal, vec3(0, 0, -1)));
    gl_FragColor = vec4(color * lighting, 1.0f); // constant white
}

)"
