#pragma once

#include "Dazel/Core/Input.h"
#include "Dazel/Core/Core.h"


namespace DAZEL
{
	class WindowsInput : public Input
	{
		virtual bool IsKeyPressedImpl(int nKeyCode) override;
		virtual bool IsMouseButtonPressedImpl(int nButton) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
	};
}

