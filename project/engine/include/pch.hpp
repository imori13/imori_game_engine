#pragma once

#pragma warning(push)
#include <codeanalysis/warnings.h>
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS ALL_CPPCORECHECK_WARNINGS)

#define NOMINMAX
#define NOHELP
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// vcpkg
#include <gsl/gsl>
#include <boost/random.hpp>
#include <boost/format.hpp>
#include <boost/random/random_device.hpp>

// default
#include <windows.h>
#include <immintrin.h>	// AVX512

#include <cstdint>
#include <cstdio>
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

#pragma warning(disable : 26440) // noexcept�Ƃ��Đ錾���邱�Ƃ��ł��܂�
#pragma warning(disable : 26432) // ���ׂĒ�`�܂��͍폜���܂�
#pragma warning(disable : 26455) // �K��̃R���X�g���N�^�[��throw�ł��܂���Bnoexcept�Ƃ��Đ錾���܂�