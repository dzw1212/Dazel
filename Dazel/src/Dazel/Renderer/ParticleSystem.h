#pragma once

#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "Dazel/Core/Core.h"
#include "Dazel/Core/Timestep.h"
#include "Dazel/Camera/OrthographicCamera.h"
#include "Shader.h"

namespace DAZEL
{
	constexpr UINT c_ParticlePoolCapacity = 1000;

	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float fSizeBegin, fSizeEnd, fSizeVariation;
		float fLifeTime = 1.f;
	};

	struct Particle
	{
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float fRotationRad = 0.f;
		float fSizeBegin, fSizeEnd;
		float fLifeTime = 1.f;
		float fLifeRemaining = 0.f;
		bool bActive = false;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem(UINT uiMaxParticleCount = c_ParticlePoolCapacity);
		~ParticleSystem() = default;

		void OnUpdate(Timestep timeStep);
		void OnRender(const OrthographicCamera& camera);
		void Emit(const ParticleProps& props);
	public:

	private:
		std::vector<Particle> m_vecParticlePool;
		UINT m_uiPoolIndex = c_ParticlePoolCapacity - 1;

		UINT m_uiQuadVertexArrayId = 0;
		Scope<Shader> m_ParticleShader;
		UINT m_uiParticleShaderViewProj;
		UINT m_uiParticleShaderTransform;
		UINT m_uiParticleShaderColor;
	};
}

