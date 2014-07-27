#version 330 core

// void main() {
//   gl_FragColor = gl_Color;
// }

out vec4 outputColor;

void main()
{
    float lerpValue = gl_FragCoord.y / 500.0f;
    
    outputColor = mix(vec4(0.0f, 1.0f, 0.0f, 1.0f),
        vec4(1.0f, 0.0f, 0.0f, 1.0f), lerpValue);
}
