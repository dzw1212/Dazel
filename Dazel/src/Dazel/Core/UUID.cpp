#include "DazelPCH.h"

#include "UUID.h"

#include <random>

namespace DAZEL
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_RandomEngine(s_RandomDevice());
	static std::uniform_int_distribution<UINT64> s_UniformDistribution;

	UUID::UUID()
		: m_u64Id(s_UniformDistribution(s_RandomEngine))
	{
	}
	UUID::UUID(UINT64 Id)
		: m_u64Id(Id)
	{
	}
}
