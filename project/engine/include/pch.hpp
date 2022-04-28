#pragma once

#pragma warning(push)
#include <codeanalysis/warnings.h>
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)

// vcpkg
#include <gsl/gsl>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#define NOMINMAX
#define NOHELP
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

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