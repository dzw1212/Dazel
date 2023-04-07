#type vertex
#version 450 core

layout(location = 0) in vec3 i_WorldPos;
layout(location = 1) in vec3 i_LocalPos;
layout(location = 2) in vec4 i_Color;
layout(location = 3) in float i_Thickness;
layout(location = 4) in float i_Fade;
layout(location = 5) in int i_EntityId;

layout(std140, binding = 0) uniform Camera {
    mat4 u_ViewProjMat;
};

struct VertexOutput
{
	vec3 LocalPos;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.LocalPos = i_LocalPos;
	Output.Color = i_Color;
	Output.Thickness = i_Thickness;
	Output.Fade = i_Fade;

	v_EntityID = i_EntityId;

	gl_Position = u_ViewProjMat * vec4(i_WorldPos, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;
layout(location = 1) out int o_EntityID;

struct VertexOutput
{
	vec3 LocalPos;
	vec4 Color;
	float Thickness;
	float Fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_EntityID;

void main()
{
	float fLength = length(Input.LocalPos);
    float fRadius = 0.5;

    float fOuterRadius = fRadius;
    float fInnerRadius = fRadius - Input.Thickness;
        
    float firstStep = smoothstep(fInnerRadius, fInnerRadius + Input.Fade, fLength);
    float secondStep = 1.0 - smoothstep(fOuterRadius, fOuterRadius + Input.Fade, fLength);
        
    o_Color = Input.Color * (firstStep * secondStep);

	o_EntityID = v_EntityID;
}
