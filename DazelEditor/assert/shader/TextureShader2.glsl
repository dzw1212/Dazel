#type vertex
#version 450 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in float texIndex;
layout(location = 4) in float tilingFactor;
layout(location = 5) in int entityId;

//uniform mat4 u_ViewProjMat;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjMat;
};

/*
out vec2 v_TexCoord;
out vec4 v_Color;
out flat float v_TexIndex;
out float v_TilingFactor;
out flat int v_EntityId;
*/

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
}
layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityId;

void main()
{
    /*
    v_TexCoord = texCoord;
    v_Color = color;
    v_TexIndex = texIndex;
    v_TilingFactor = tilingFactor;
    */

    Output.Color = color;
    Output.TexCoord = texCoord;
    Output.TexIndex = texIndex;
    Output.TilingFactor = tilingFactor;

    v_EntityId = entityId;

    gl_Position = u_ViewProjMat * vec4(vertPos, 1.0);
}

#type fragment
#version 450 core
layout(location = 0) out vec4 color;
layout(location = 1) out int entityId;

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
}
layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int v_EntityId;


//uniform sampler2D u_TextureArray[32];

layout(binding = 0) uniform sampler2D u_TextureArray[32];

void main()
{
    color = texture(u_TextureArray[int(Input.TexIndex)], Input.TexCoord * Input.TilingFactor) * Input.Color;

    entityId = v_EntityId;
}