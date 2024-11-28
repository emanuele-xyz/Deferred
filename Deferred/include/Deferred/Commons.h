#pragma once

#include <stdexcept>
#include <stacktrace>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#if defined(_DEBUG)
#define D_Crash(msg) __debugbreak()
#else
#define D_Crash(msg) throw ::std::runtime_error{ std::format("{}({}): {}\n{}", __FILE__, __LINE__, msg, std::stacktrace::current()) }
#endif
#define D_Check(p) do { if (!(p)) { D_Crash("'"#p"' failed"); } } while (false)
#define D_CheckHR(p) do { if (!SUCCEEDED(p)) { D_Crash("'"#p"' failed"); } } while (false)

namespace D
{
}
