#version 450

//
// (-1,-1) .__________. (1, -1)
//         |          |
//         |          |
//         |          |
// (-1, 1) .__________. (1, 1)
//

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

void main() {
    int index = gl_VertexIndex;

    // output
    gl_Position = vec4(positions[index], 0.0, 1.0);
}
