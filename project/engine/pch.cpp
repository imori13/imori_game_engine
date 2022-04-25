#include "pch.h"

#pragma warning(push)
#include <codeanalysis/warnings.h>
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)

#define NOMINMAX
#define NOHELP
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// vcpkg
#include <gsl/gsl>

// WindowsSDK
#define DIRECTINPUT_VERSION 0x0800	// DirectXInput version
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <dinput.h>
#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include <cstdint>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#undef near
#undef far

#pragma warning(pop)