#pragma once

#include "Core.h"

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