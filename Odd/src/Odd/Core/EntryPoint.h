#pragma once

#include "Log.h"

extern Odd::Application* Odd::CreateApplication();

int main(int argc, char** argv)
{
	ODD_PROFILE_BEGIN_SESSION("Startup", "OddProfile-Startup.json");
	auto application = Odd::CreateApplication();
	ODD_PROFILE_END_SESSION();

	ODD_PROFILE_BEGIN_SESSION("Runtime", "OddProfile-Runtime.json");
	application->Run();
	ODD_PROFILE_END_SESSION();

	ODD_PROFILE_BEGIN_SESSION("Shutdown", "OddProfile-Shutdown.json");
	delete application;
	ODD_PROFILE_END_SESSION();
}