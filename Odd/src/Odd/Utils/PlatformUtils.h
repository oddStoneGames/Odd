#pragma once

#include <string>

namespace Odd
{
	class FileDialogs
	{
	public:
		// These Return Empty String if Cancelled.
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}