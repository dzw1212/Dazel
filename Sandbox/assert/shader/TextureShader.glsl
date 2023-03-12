#type vertex
#version 430

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in float texIndex;
layout(location = 4) in float tilingFactor;

uniform mat4 u_ViewProjMat;
//uniform mat4 u_ModelMat;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;
out float v_TilingFactor;

void main()
{
    v_TexCoord = texCoord;
    v_Color = color;
    v_TexIndex = texIndex;
    v_TilingFactor = tilingFactor;
    gl_Position = u_ViewProjMat * vec4(vertPos, 1.0);
}

#type fragment
#version 430
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_TextureArray[32];
//uniform vec3 u_Color;
//uniform int u_TileCoef;

void main()
{
    color = texture(u_TextureArray[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;

    //color = v_Color;
}