#pragma once

#include "Dazel/Core/Timestep.h"

#include "Entity.h"

namespace DAZEL
{

	class ScriptableEntity
	{
	public:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(Timestep timeStep) {};

		//template<typename T>
		//T& GetComponent()
		//{
		//	//return m_Entity.GetComponent<T>();
		//}

		//Entity m_Entity;
	};
}


