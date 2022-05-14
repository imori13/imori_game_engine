#pragma once
#include "winapp.hpp"

namespace
{
	constexpr uint32_t FRAME_COUNT = 2;
}

class descriptor_heap;

class graphic_d3d12
{
public:
	graphic_d3d12(const winapp& winapp);
	~graphic_d3d12() noexcept(false);

public:
	graphic_d3d12(graphic_d3d12&&) = default;
	graphic_d3d12& operator=(graphic_d3d12&&) = default;

public:
	graphic_d3d12(const graphic_d3d12&) = delete;
	graphic_d3d12& operator=(const graphic_d3d12&) = delete;

public:
	void render();
	void wait_gpu();

private:
	Microsoft::WRL::ComPtr<ID3D12Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_pQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain3> m_pSwapchain;

	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>, FRAME_COUNT> m_pColorBuffer;
	std::array<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, FRAME_COUNT> m_pCommandAllocator;

	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCommandList;
	std::unique_ptr<descriptor_heap> m_pHeapRTV;
	Microsoft::WRL::ComPtr<ID3D12Fence> m_pFence;

	HANDLE m_fenceEvent;
	uint32_t m_frameIndex;

	std::array<uint64_t, FRAME_COUNT> m_fenceCounter;
};