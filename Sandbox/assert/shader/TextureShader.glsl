#type vertex
#version 430
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_ViewProjMat;
uniform mat4 u_ModelMat;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = texCoord;
    gl_Position = u_ViewProjMat * u_ModelMat * vec4(vertPos, 1.0);
}

#type fragment
#version 430
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec3 u_Color;
uniform int u_TileCoef;

void main()
{
    color = texture(u_Texture, v_TexCoord * u_TileCoef) * vec4(u_Color, 1.0);

    //color = vec4(v_TexCoord, 0.0, 1.0);
}