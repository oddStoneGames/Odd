#pragma once

#include "Core.h"
#include "Odd/Events/Event.h"

namespace Odd
{
	class ODD_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();
	};

	Application* CreateApplication();	//Defined By Client
}