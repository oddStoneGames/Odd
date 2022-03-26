#pragma once

#ifdef ODD_PLATFORM_WINDOWS

#include "Log.h"

extern Odd::Application* Odd::CreateApplication();

int main(int argc, char** argv)
{
	auto application = Odd::CreateApplication();
	application->Run();
	delete application;
}

#endif