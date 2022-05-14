#pragma once
#include <cstdint>
#include <memory>
#include <windows.h>

class winapp
{
public:
	winapp() = default;
	winapp(uint32_t width, uint32_t height);
	~winapp();

public:
	winapp(winapp&&) = default;
	winapp& operator=(winapp&&) = default;

public:
	winapp(const winapp&) = delete;
	winapp& operator=(const winapp&) = delete;

public:
	void update() noexcept;

public:
	bool isloop() const noexcept;
	uint32_t getWidth() const noexcept;
	uint32_t getHeight() const noexcept;
	HINSTANCE getHInstance() const noexcept;
	HWND getHWnd() const noexcept;

private:
	class winapp_impl; std::unique_ptr<winapp_impl> m_impl;
};