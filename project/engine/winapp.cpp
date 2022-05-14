#include "pch.hpp"
#include "winapp.hpp"

namespace
{
	LRESULT CALLBACK WndProc(HWND hWnd, uint32_t msg, WPARAM wp, LPARAM lp) noexcept
	{
		switch(msg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				break;

			default:
				break;
		}

		return DefWindowProc(hWnd, msg, wp, lp);
	}

	constexpr auto s_class_name = TEXT("imori_engine");
	constexpr auto s_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
}

class winapp::winapp_impl
{
public:
	winapp_impl(uint32_t width, uint32_t height) noexcept
		: m_width(width)
		, m_height(height)
		, m_hInstance(nullptr)
		, m_hWnd(nullptr)
		, m_msg{}
	{
	}

public:
	uint32_t m_width;
	uint32_t m_height;

	HINSTANCE m_hInstance;
	HWND m_hWnd;
	MSG m_msg;

};

winapp::winapp(uint32_t width, uint32_t height) 
	: m_impl(std::make_unique<winapp_impl>(width, height))
{
	auto& hInstance = m_impl->m_hInstance;
	auto& hWnd = m_impl->m_hWnd;

	hInstance = GetModuleHandle(nullptr);

	WNDCLASSEX window_class{};
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;
	window_class.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(hInstance, IDC_ARROW);
	window_class.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = s_class_name;
	window_class.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

	RegisterClassEx(&window_class);

	RECT rc = RECT{ 0, 0, gsl::narrow<LONG>(width), gsl::narrow<LONG>(height) };

	AdjustWindowRect(&rc, s_style, false);

	hWnd = CreateWindowEx(
		0,
		s_class_name,
		s_class_name,
		s_style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	SetFocus(hWnd);
}
winapp::~winapp()
{
	if(m_impl->m_hInstance != nullptr)
	{
		UnregisterClass(s_class_name, m_impl->m_hInstance);
	}
}

void winapp::update() noexcept
{
	auto& msg = m_impl->m_msg;

	if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

bool winapp::isloop() const noexcept
{
	return WM_QUIT != m_impl->m_msg.message;
}

uint32_t winapp::getWidth() const noexcept
{
	return m_impl->m_width;
}

uint32_t winapp::getHeight() const noexcept
{
	return m_impl->m_height;
}

HINSTANCE winapp::getHInstance() const noexcept
{
	return m_impl->m_hInstance;
}

HWND winapp::getHWnd() const noexcept
{
	return m_impl->m_hWnd;
}
