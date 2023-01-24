#pragma once

#include "Dazel/Core/Core.h"

namespace DAZEL
{
	class DAZEL_API Input
	{
	public:
		inline static bool IsKeyPressed(int nKeyCode) { return s_Inst->IsKeyPressedImpl(nKeyCode); }
		inline static bool IsMouseButtonPressed(int nButton) { return s_Inst->IsMouseButtonPressedImpl(nButton); }
		inline static std::pair<float, float> GetMousePos() { return s_Inst->GetMousePosImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int nKeyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(int nButton) = 0;
		virtual std::pair<float,float> GetMousePosImpl() = 0;
	private:
		static Input* s_Inst;
	};
}