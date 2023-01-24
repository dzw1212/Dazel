#pragma once

namespace DAZEL
{
	class Timestep
	{
	public:
		Timestep(float fTime)
			: m_fTime(fTime)
		{}

		inline float GetSeconds() const { return m_fTime; }
		inline float GetMilliseconds() const { return m_fTime * 1000; }
	private:
		float m_fTime;
	};
}