#pragma once
#include "Core.h"

#include <xhash>

namespace DAZEL
{
	class UUID
	{
	public:
		UUID();
		UUID(const UINT64& Id);
		UUID(const UUID&) = default;

		operator UINT64() const { return m_u64Id; }

	private:
		UINT64 m_u64Id;
	};
}

namespace std
{
	template<>
	struct std::hash<DAZEL::UUID>
	{
		std::size_t operator()(const DAZEL::UUID& uuid)
		{
			return std::hash<UINT64>()(uuid);
		}
	};
}
