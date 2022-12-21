#include <Dazel.h>

class SandBox : public DAZEL::Application
{
public:
	SandBox()
	{


	}

	~SandBox()
	{

	}


};

DAZEL::Application* DAZEL::CreateApplication()
{
	return new SandBox();
}