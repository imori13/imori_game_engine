#include "pch.hpp"
#include "graphic_d3d12.hpp"
#include "descriptor_heap.hpp"

using namespace Microsoft::WRL;

namespace
{
	void enable_debuglayer()
	{
		ComPtr<ID3D12Debug> pDebug;
		const auto& hr = D3D12GetDebugInterface(IID_PPV_ARGS(pDebug.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		// enable debug layer
		pDebug->EnableDebugLayer();
	}

	ComPtr<ID3D12Device> create_device_11_0()
	{
		ComPtr<ID3D12Device> pDevice;

		const auto& hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(pDevice.GetAddressOf()));

		Ensures(SUCCEEDED(hr));

		return pDevice;
	}

	ComPtr<ID3D12CommandQueue> create_command_queue(gsl::not_null<ID3D12Device*> pDevice)
	{
		ComPtr<ID3D12CommandQueue> pCommandQueue;

		D3D12_COMMAND_QUEUE_DESC desc{};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		const auto& hr = pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(pCommandQueue.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return pCommandQueue;
	}

	ComPtr<IDXGIFactory4> create_factory()
	{
		ComPtr<IDXGIFactory4> pFactory = nullptr;
		const auto& hr = CreateDXGIFactory1(IID_PPV_ARGS(pFactory.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return pFactory;
	}

	ComPtr<IDXGISwapChain3> create_swapchain(gsl::not_null<ID3D12CommandQueue*> pQueue, const winapp& winapp)
	{
		// create factory
		const auto pFactory = create_factory();

		// setting swapchain
		DXGI_SWAP_CHAIN_DESC desc{};
		desc.BufferDesc.Width = winapp.getWidth();
		desc.BufferDesc.Height = winapp.getHeight();
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = FRAME_COUNT;
		desc.OutputWindow = winapp.getHWnd();
		desc.Windowed = true;	// window_mode?
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// create swapchain
		ComPtr<IDXGISwapChain> pSwapchain;
		auto hr = pFactory->CreateSwapChain(pQueue, &desc, pSwapchain.GetAddressOf());
		Ensures(SUCCEEDED(hr));

		ComPtr<IDXGISwapChain3> pSwapChain3;
		hr = pSwapchain->QueryInterface(IID_PPV_ARGS(pSwapChain3.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return pSwapChain3;
	}

	ComPtr<ID3D12CommandAllocator> create_command_allocator(gsl::not_null<ID3D12Device*> pDevice)
	{
		ComPtr<ID3D12CommandAllocator> pCommandAllocator;
		const auto& hr = pDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(pCommandAllocator.GetAddressOf())
		);
		Ensures(SUCCEEDED(hr));

		return pCommandAllocator;
	}

	ComPtr<ID3D12GraphicsCommandList> create_command_list(gsl::not_null<ID3D12Device*> pDevice, gsl::not_null<ID3D12CommandAllocator*> pCommandAllocator)
	{
		ComPtr<ID3D12GraphicsCommandList> pCommandList;

		const auto& hr = pDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			pCommandAllocator,
			nullptr,
			IID_PPV_ARGS(pCommandList.GetAddressOf())
		);
		Ensures(SUCCEEDED(hr));

		return pCommandList;
	}

	ComPtr<ID3D12DescriptorHeap> create_heap(gsl::not_null<ID3D12Device*> pDevice)
	{
		ComPtr<ID3D12DescriptorHeap> pHeap;

		// setting descriptor heap
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.NumDescriptors = FRAME_COUNT;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 0;

		// create descriptor heap
		const auto& hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(pHeap.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return pHeap;
	}

	ComPtr<ID3D12Resource> get_color_buffer(gsl::not_null<IDXGISwapChain3*> pSwapchain, uint32_t frameIndex)
	{
		ComPtr<ID3D12Resource> pResource;

		const auto& hr = pSwapchain->GetBuffer(frameIndex, IID_PPV_ARGS(pResource.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return pResource;
	}
}

graphic_d3d12::graphic_d3d12(const winapp& winapp)
{
	HRESULT hr{};

#ifdef _DEBUG
	enable_debuglayer();
#endif

	// create device
	m_pDevice = create_device_11_0();
	m_pQueue = create_command_queue(m_pDevice.Get());

	// create swapchain
	{
		// create swapchain
		m_pSwapchain = create_swapchain(m_pQueue.Get(), winapp);

		// get current back buffer index
		m_frameIndex = m_pSwapchain->GetCurrentBackBufferIndex();
	}

	// create command allocator
	{
		for(auto& command_allocator : m_pCommandAllocator)
		{
			command_allocator = create_command_allocator(m_pDevice.Get());
		}
	}

	// create commandlist
	{
		m_pCommandList = create_command_list(m_pDevice.Get(), m_pCommandAllocator.at(m_frameIndex).Get());
	}

	// create render target view
	{
		m_pHeapRTV = std::make_unique<descriptor_heap>(m_pDevice.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FRAME_COUNT);

		for(auto i = 0u; i < m_pColorBuffer.size(); ++i)
		{
			m_pColorBuffer.at(i) = get_color_buffer(m_pSwapchain.Get(), i);

			m_pHeapRTV->createRenderTargetView(m_pColorBuffer.at(i).Get());
		}
	}

	// create fence
	{
		// reset counter
		for(auto& fence_counter : m_fenceCounter)
		{
			fence_counter = 0;
		}

		// create fence
		hr = m_pDevice->CreateFence(
			m_fenceCounter.at(m_frameIndex),
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(m_pFence.GetAddressOf())
		);
		Ensures(SUCCEEDED(hr));

		++m_fenceCounter.at(m_frameIndex);

		// create event
		m_fenceEvent = CreateEvent(nullptr, false, false, nullptr);
		Ensures(m_fenceEvent != nullptr);
	}

	m_pCommandList->Close();
}

graphic_d3d12::~graphic_d3d12() noexcept(false)
{
	wait_gpu();

	if(m_fenceEvent != nullptr)
	{
		CloseHandle(m_fenceEvent);
		m_fenceEvent = nullptr;
	}
}

void graphic_d3d12::render()
{
	// start commandt
	m_pCommandAllocator.at(m_frameIndex)->Reset();
	m_pCommandList->Reset(m_pCommandAllocator.at(m_frameIndex).Get(), nullptr);

	// resource barrier
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_pColorBuffer.at(m_frameIndex).Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	m_pCommandList->ResourceBarrier(1, &barrier);

	constexpr const std::array<float, 4> clear_color = { 0.25f,0.25f,0.25f,1.0f };

	m_pCommandList->ClearRenderTargetView(m_pHeapRTV->at(m_frameIndex), clear_color.data(), 0, nullptr);

	// •`‰æˆ—
	{

	}

	// resource barrier
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_pColorBuffer.at(m_frameIndex).Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	m_pCommandList->ResourceBarrier(1, &barrier);

	// close command
	m_pCommandList->Close();

	// exture
	std::array<ID3D12CommandList*, 1> ppCmdLists = { m_pCommandList.Get() };
	m_pQueue->ExecuteCommandLists(1, ppCmdLists.data());

	// ------------------------------------------------------------------------------------------

	constexpr uint32_t interval = 1;
	m_pSwapchain->Present(interval, 0);

	const auto currentValue = m_fenceCounter.at(m_frameIndex);
	m_pQueue->Signal(m_pFence.Get(), currentValue);

	m_frameIndex = m_pSwapchain->GetCurrentBackBufferIndex();

	if(m_pFence->GetCompletedValue() < m_fenceCounter.at(m_frameIndex))
	{
		m_pFence->SetEventOnCompletion(m_fenceCounter.at(m_frameIndex), m_fenceEvent);
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, false);
	}

	m_fenceCounter.at(m_frameIndex) = currentValue + 1;
}

void graphic_d3d12::wait_gpu()
{
	m_pQueue->Signal(m_pFence.Get(), m_fenceCounter.at(m_frameIndex));

	m_pFence->SetEventOnCompletion(m_fenceCounter.at(m_frameIndex), m_fenceEvent);

	WaitForSingleObjectEx(m_fenceEvent, INFINITE, false);

	++m_fenceCounter.at(m_frameIndex);
}