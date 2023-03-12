#type vertex
#version 430

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in float texIndex;
layout(location = 4) in float tilingFactor;
layout(location = 5) in int entityId;

uniform mat4 u_ViewProjMat;

out vec2 v_TexCoord;
out vec4 v_Color;
out flat float v_TexIndex;
out float v_TilingFactor;
out flat int v_EntityId;

void main()
{
    v_TexCoord = texCoord;
    v_Color = color;
    v_TexIndex = texIndex;
    v_TilingFactor = tilingFactor;
    v_EntityId = entityId;
    gl_Position = u_ViewProjMat * vec4(vertPos, 1.0);
}

#type fragment
#version 430
layout(location = 0) out vec4 color;
layout(location = 1) out int entityId;

in vec2 v_TexCoord;
in vec4 v_Color;
in flat float v_TexIndex;
in float v_TilingFactor;
in flat int v_EntityId;

uniform sampler2D u_TextureArray[32];

void main()
{
    color = texture(u_TextureArray[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;

    /*if (v_EntityId == 3)
        color = vec4(1.0, 0.0, 0.0, 1.0);
    else if (v_EntityId == 4)
        color = vec4(0.0, 1.0, 0.0, 1.0);
    else if (v_EntityId == 0)
        color = vec4(0.0, 0.0, 1.0, 1.0);*/

    entityId = v_EntityId;
}