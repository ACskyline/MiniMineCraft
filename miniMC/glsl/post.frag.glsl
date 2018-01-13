#version 150
// ^ Change this to version 130 if you have compatibility issues

// Refer to the lambert shader files for useful comments

in vec4 fs_Col;

out vec4 out_Col;

in vec2 fs_UV;

uniform sampler2D u_Texture;
uniform int u_Time;

void main()
{
    vec2 myUV = fs_UV;// + vec2(1,0) * sin(u_Time*fs_UV.y) * 0.1;

    vec4 diffuseColor = texture(u_Texture,myUV);

    out_Col = diffuseColor;//fs_Col;
}
