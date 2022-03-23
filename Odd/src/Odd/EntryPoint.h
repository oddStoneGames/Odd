#pragma once

#ifdef ODD_PLATFORM_WINDOWS

extern Odd::Application* Odd::CreateApplication();

int main(int argc, char** argv)
{
	auto application = Odd::CreateApplication();
	application->Run();
	delete application;
}

#endif