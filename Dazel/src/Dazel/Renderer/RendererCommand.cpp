#include "DazelPCH.h"
#include "RendererCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace DAZEL
{
	RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
}