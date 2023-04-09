#type vertex
#version 450 core

layout(location = 0) in vec3 i_WorldPos;
layout(location = 1) in vec4 i_Color;
layout(location = 2) in int i_EntityId;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjMat;
};

struct VertexOutput
{
	vec4 Color;
};

layout (location = 0) out VertexOutput Output;
layout (location = 1) out flat int v_EntityID;

void main()
{
	Output.Color = i_Color;

	v_EntityID = i_EntityId;

	gl_Position = u_ViewProjMat * vec4(i_WorldPos, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec4 Color;
};

layout (location = 0) in VertexOutput Input;
layout (location = 1) in flat int v_EntityID;

void main()
{
	o_Color = Input.Color;

	o_EntityID = v_EntityID;
}
