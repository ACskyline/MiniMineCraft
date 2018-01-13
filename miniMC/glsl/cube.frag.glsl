#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

in vec4 fs_Col;

out vec4 out_Col;

in vec3 uvCube;

uniform samplerCube u_Cube;

void main()
{
    // Copy the color; there is no shading.
    out_Col = texture(u_Cube, uvCube);
}
