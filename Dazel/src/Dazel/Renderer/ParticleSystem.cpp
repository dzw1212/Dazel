#include "DazelPCH.h"
#include "ParticleSystem.h"

#include "glm/gtc/constants.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/compatibility.hpp"

#include "Dazel/Core/Random.h"
#include "Dazel/Renderer/VertexArray.h"
#include "Renderer2D.h"

namespace DAZEL
{
	ParticleSystem::ParticleSystem(UINT uiMaxParticleCount)
		: m_uiPoolIndex(uiMaxParticleCount - 1)
	{
		m_vecParticlePool.resize(c_ParticlePoolCapacity);
	}
	void ParticleSystem::OnUpdate(Timestep timeStep)
	{
		for (auto& particle : m_vecParticlePool)
		{
			if (!particle.bActive)
				continue;

			if (particle.fLifeRemaining <= 0.f)
			{
				particle.bActive = false;
				continue;
			}

			particle.fLifeRemaining -= timeStep.GetSeconds();
			particle.Position += particle.Velocity * timeStep.GetSeconds();
			particle.fRotationRad += 0.01 * timeStep.GetSeconds();
		}
	}
	void ParticleSystem::OnRender(const OrthographicCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		for (auto& particle : m_vecParticlePool)
		{
			if (!particle.bActive)
				continue;

			//Fade away
			float fLifeRatio = particle.fLifeRemaining / particle.fLifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, fLifeRatio);
			color.a *= fLifeRatio;
			
			//Size
			float fSize = glm::lerp(particle.fSizeEnd, particle.fSizeBegin, fLifeRatio);

			//Render
			Renderer2D::DrawRotateQuad(particle.Position, glm::vec2(fSize, fSize), particle.fRotationRad, color);
		}
		Renderer2D::EndScene();
	}
	void ParticleSystem::Emit(const ParticleProps& props)
	{
		auto& particle = m_vecParticlePool[m_uiPoolIndex];
		particle.bActive = true;
		particle.Position = props.Position;
		particle.fRotationRad = Random::Float() * 2.f * glm::pi<float>();

		//Velocity
		particle.Velocity = props.Velocity;
		particle.Velocity.x += props.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += props.VelocityVariation.y * (Random::Float() - 0.5f);

		//Color
		particle.ColorBegin = props.ColorBegin;
		particle.ColorEnd = props.ColorEnd;

		//Size
		particle.fSizeBegin = props.fSizeBegin + props.fSizeVariation * (Random::Float() - 0.5f);
		particle.fSizeEnd = props.fSizeEnd;

		//LifeTime
		particle.fLifeTime = props.fLifeTime;
		particle.fLifeRemaining = particle.fLifeTime;

		m_uiPoolIndex = --m_uiPoolIndex % m_vecParticlePool.size(); //must be positive num
	}
}