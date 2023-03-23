#type vertex
#version 450 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 color;
layout(location = 3) in float texIndex;
layout(location = 4) in float tilingFactor;
layout(location = 5) in int entityId;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjMat;
};

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

layout(location = 0) out VertexOutput Output;
layout(location = 4) out flat int v_EntityId;

void main()
{
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
};

layout(location = 0) in VertexOutput Input;
layout(location = 4) in flat int v_EntityId;

layout(binding = 0) uniform sampler2D u_TextureArray[32];

void main()
{
    vec4 texColor = Input.Color;

	switch(int(Input.TexIndex))
	{
		case  0: texColor *= texture(u_TextureArray[ 0], Input.TexCoord * Input.TilingFactor); break;
		case  1: texColor *= texture(u_TextureArray[ 1], Input.TexCoord * Input.TilingFactor); break;
		case  2: texColor *= texture(u_TextureArray[ 2], Input.TexCoord * Input.TilingFactor); break;
		case  3: texColor *= texture(u_TextureArray[ 3], Input.TexCoord * Input.TilingFactor); break;
		case  4: texColor *= texture(u_TextureArray[ 4], Input.TexCoord * Input.TilingFactor); break;
		case  5: texColor *= texture(u_TextureArray[ 5], Input.TexCoord * Input.TilingFactor); break;
		case  6: texColor *= texture(u_TextureArray[ 6], Input.TexCoord * Input.TilingFactor); break;
		case  7: texColor *= texture(u_TextureArray[ 7], Input.TexCoord * Input.TilingFactor); break;
		case  8: texColor *= texture(u_TextureArray[ 8], Input.TexCoord * Input.TilingFactor); break;
		case  9: texColor *= texture(u_TextureArray[ 9], Input.TexCoord * Input.TilingFactor); break;
		case 10: texColor *= texture(u_TextureArray[10], Input.TexCoord * Input.TilingFactor); break;
		case 11: texColor *= texture(u_TextureArray[11], Input.TexCoord * Input.TilingFactor); break;
		case 12: texColor *= texture(u_TextureArray[12], Input.TexCoord * Input.TilingFactor); break;
		case 13: texColor *= texture(u_TextureArray[13], Input.TexCoord * Input.TilingFactor); break;
		case 14: texColor *= texture(u_TextureArray[14], Input.TexCoord * Input.TilingFactor); break;
		case 15: texColor *= texture(u_TextureArray[15], Input.TexCoord * Input.TilingFactor); break;
		case 16: texColor *= texture(u_TextureArray[16], Input.TexCoord * Input.TilingFactor); break;
		case 17: texColor *= texture(u_TextureArray[17], Input.TexCoord * Input.TilingFactor); break;
		case 18: texColor *= texture(u_TextureArray[18], Input.TexCoord * Input.TilingFactor); break;
		case 19: texColor *= texture(u_TextureArray[19], Input.TexCoord * Input.TilingFactor); break;
		case 20: texColor *= texture(u_TextureArray[20], Input.TexCoord * Input.TilingFactor); break;
		case 21: texColor *= texture(u_TextureArray[21], Input.TexCoord * Input.TilingFactor); break;
		case 22: texColor *= texture(u_TextureArray[22], Input.TexCoord * Input.TilingFactor); break;
		case 23: texColor *= texture(u_TextureArray[23], Input.TexCoord * Input.TilingFactor); break;
		case 24: texColor *= texture(u_TextureArray[24], Input.TexCoord * Input.TilingFactor); break;
		case 25: texColor *= texture(u_TextureArray[25], Input.TexCoord * Input.TilingFactor); break;
		case 26: texColor *= texture(u_TextureArray[26], Input.TexCoord * Input.TilingFactor); break;
		case 27: texColor *= texture(u_TextureArray[27], Input.TexCoord * Input.TilingFactor); break;
		case 28: texColor *= texture(u_TextureArray[28], Input.TexCoord * Input.TilingFactor); break;
		case 29: texColor *= texture(u_TextureArray[29], Input.TexCoord * Input.TilingFactor); break;
		case 30: texColor *= texture(u_TextureArray[30], Input.TexCoord * Input.TilingFactor); break;
		case 31: texColor *= texture(u_TextureArray[31], Input.TexCoord * Input.TilingFactor); break;
	}

	color = texColor;
    entityId = v_EntityId;
}
