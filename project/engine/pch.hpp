#pragma once

#pragma warning(push, 0)
#include <codeanalysis/warnings.h>
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS ALL_CPPCORECHECK_WARNINGS)

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define NOMINMAX
#define NOHELP
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// vcpkg
#include <gsl/gsl>
#include <boost/format.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

// direct3d
#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include <wrl/client.h>
#include <DirectXMath/DirectXMath.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// default
#include <windows.h>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>
#include <chrono>

#undef near
#undef far

#pragma warning(pop)

#pragma warning(disable: 26812)	// C26812: enum -> enum class
#pragma warning(disable: 4324) // 