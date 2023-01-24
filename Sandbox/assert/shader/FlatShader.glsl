#type vertex
#version 430
layout(location = 0) in vec3 vertPos;

uniform mat4 u_ViewProjMat;
uniform mat4 u_ModelMat;
uniform vec3 u_Color;

out vec4 userColor;

void main()
{
    gl_Position = u_ViewProjMat * u_ModelMat * vec4(vertPos, 1.0);
    userColor = vec4(u_Color, 1.0);
}

#type fragment
#version 430
layout(location = 0) out vec4 color;

in vec4 userColor;

void main()
{
    color = userColor;
}