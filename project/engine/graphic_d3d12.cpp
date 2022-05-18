#include "pch.hpp"
#include "graphic_d3d12.hpp"
#include "descriptor_heap.hpp"
#include "gpu_buffer.hpp"

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
		ComPtr<ID3D12Device> device;

		const auto& hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(device.GetAddressOf()));

		Ensures(SUCCEEDED(hr));

		return device;
	}

	ComPtr<ID3D12CommandQueue> create_command_queue(gsl::not_null<ID3D12Device*> pDevice)
	{
		ComPtr<ID3D12CommandQueue> command_queue;

		D3D12_COMMAND_QUEUE_DESC desc{};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		const auto& hr = pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(command_queue.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return command_queue;
	}

	ComPtr<IDXGIFactory4> create_factory()
	{
		ComPtr<IDXGIFactory4> factory = nullptr;
		const auto& hr = CreateDXGIFactory1(IID_PPV_ARGS(factory.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return factory;
	}

	ComPtr<IDXGISwapChain3> create_swapchain(gsl::not_null<ID3D12CommandQueue*> pQueue, const winapp& winapp)
	{
		// create factory
		const auto factory = create_factory();

		// setting swapchain
		DXGI_SWAP_CHAIN_DESC desc{};
		desc.BufferDesc.Width = winapp.get_width();
		desc.BufferDesc.Height = winapp.get_height();
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = FRAME_COUNT;
		desc.OutputWindow = winapp.get_hwnd();
		desc.Windowed = true;	// window_mode?
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		// create swapchain
		ComPtr<IDXGISwapChain> swapchain;
		auto hr = factory->CreateSwapChain(pQueue, &desc, swapchain.GetAddressOf());
		Ensures(SUCCEEDED(hr));

		ComPtr<IDXGISwapChain3> swapchain3;
		hr = swapchain->QueryInterface(IID_PPV_ARGS(swapchain3.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return swapchain3;
	}

	ComPtr<ID3D12CommandAllocator> create_command_allocator(gsl::not_null<ID3D12Device*> pDevice)
	{
		ComPtr<ID3D12CommandAllocator> command_allocator;
		const auto& hr = pDevice->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(command_allocator.GetAddressOf())
		);
		Ensures(SUCCEEDED(hr));

		return command_allocator;
	}

	ComPtr<ID3D12GraphicsCommandList> create_command_list(gsl::not_null<ID3D12Device*> pDevice, gsl::not_null<ID3D12CommandAllocator*> pCommandAllocator)
	{
		ComPtr<ID3D12GraphicsCommandList> command_list;

		const auto& hr = pDevice->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			pCommandAllocator,
			nullptr,
			IID_PPV_ARGS(command_list.GetAddressOf())
		);
		Ensures(SUCCEEDED(hr));

		return command_list;
	}

	ComPtr<ID3D12DescriptorHeap> create_heap(gsl::not_null<ID3D12Device*> pDevice)
	{
		ComPtr<ID3D12DescriptorHeap> heap;

		// setting descriptor heap
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.NumDescriptors = FRAME_COUNT;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 0;

		// create descriptor heap
		const auto& hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(heap.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return heap;
	}

	ComPtr<ID3D12Resource> get_color_buffer(gsl::not_null<IDXGISwapChain3*> pSwapchain, uint32_t frameIndex)
	{
		ComPtr<ID3D12Resource> resource;

		const auto& hr = pSwapchain->GetBuffer(frameIndex, IID_PPV_ARGS(resource.GetAddressOf()));
		Ensures(SUCCEEDED(hr));

		return resource;
	}
}

graphic_d3d12::graphic_d3d12(const winapp& winapp)
{
	HRESULT hr{};

#ifdef _DEBUG
	enable_debuglayer();
#endif

	// create device
	m_device = create_device_11_0();
	m_command_queue = create_command_queue(m_device.Get());

	// create swapchain
	{
		// create swapchain
		m_swapchain = create_swapchain(m_command_queue.Get(), winapp);

		// get current back buffer index
		m_frame_index = m_swapchain->GetCurrentBackBufferIndex();
	}

	// create command allocator
	{
		for(auto& command_allocator : m_command_allocator)
		{
			command_allocator = create_command_allocator(m_device.Get());
		}
	}

	// create commandlist
	{
		m_command_list = create_command_list(m_device.Get(), m_command_allocator.at(m_frame_index).Get());
	}

	// create render target view
	{
		m_heap_rtv = std::make_unique<descriptor_heap>(m_device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FRAME_COUNT);

		for(auto i = 0u; i < m_color_buffer.size(); ++i)
		{
			m_color_buffer.at(i) = get_color_buffer(m_swapchain.Get(), i);

			m_heap_rtv->createRenderTargetView(m_color_buffer.at(i).Get());
		}
	}

	// create fence
	{
		// reset counter
		for(auto& fence_counter : m_fence_counter)
		{
			fence_counter = 0;
		}

		// create fence
		hr = m_device->CreateFence(
			m_fence_counter.at(m_frame_index),
			D3D12_FENCE_FLAG_NONE,
			IID_PPV_ARGS(m_fence.GetAddressOf())
		);
		Ensures(SUCCEEDED(hr));

		++m_fence_counter.at(m_frame_index);

		// create event
		m_fence_event = CreateEvent(nullptr, false, false, nullptr);
		Ensures(m_fence_event != nullptr);
	}

	m_command_list->Close();

	// ----------------------------------------------------------------------------------------------------

	std::vector<vertex> vertices =
	{
		vertex{ vector3{-1.0f,-1.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
		vertex{ vector3{ 1.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
		vertex{ vector3{ 0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
	};

	gpu_buffer<vertex> vertex_buffer(m_device.Get(), vertices.size() * sizeof(vertex));

	vertex_buffer.map(vertices);

	struct alignas(256) transform
	{
		matrix4x4 world;
		matrix4x4 view;
		matrix4x4 proj;
	};

	transform tttt{};

	gpu_buffer<transform> transform_buffer(m_device.Get(), sizeof(transform));

	transform_buffer.map(gsl::make_span<transform>(&tttt, 1));
}

graphic_d3d12::~graphic_d3d12() noexcept(false)
{
	wait_gpu();

	if(m_fence_event != nullptr)
	{
		CloseHandle(m_fence_event);
		m_fence_event = nullptr;
	}
}

void graphic_d3d12::render()
{
	// start commandt
	m_command_allocator.at(m_frame_index)->Reset();
	m_command_list->Reset(m_command_allocator.at(m_frame_index).Get(), nullptr);

	// resource barrier
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_color_buffer.at(m_frame_index).Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	m_command_list->ResourceBarrier(1, &barrier);

	constexpr const std::array<float, 4> clear_color = { 0.25f,0.25f,0.25f,1.0f };

	m_command_list->ClearRenderTargetView(m_heap_rtv->at(m_frame_index), clear_color.data(), 0, nullptr);

	// •`‰æˆ—
	{

	}

	// resource barrier
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = m_color_buffer.at(m_frame_index).Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

	m_command_list->ResourceBarrier(1, &barrier);

	// close command
	m_command_list->Close();

	// exture
	std::array<ID3D12CommandList*, 1> ppCmdLists = { m_command_list.Get() };
	m_command_queue->ExecuteCommandLists(1, ppCmdLists.data());

	// ------------------------------------------------------------------------------------------

	constexpr uint32_t interval = 1;
	m_swapchain->Present(interval, 0);

	const auto currentValue = m_fence_counter.at(m_frame_index);
	m_command_queue->Signal(m_fence.Get(), currentValue);

	m_frame_index = m_swapchain->GetCurrentBackBufferIndex();

	if(m_fence->GetCompletedValue() < m_fence_counter.at(m_frame_index))
	{
		m_fence->SetEventOnCompletion(m_fence_counter.at(m_frame_index), m_fence_event);
		WaitForSingleObjectEx(m_fence_event, INFINITE, false);
	}

	m_fence_counter.at(m_frame_index) = currentValue + 1;
}

void graphic_d3d12::wait_gpu()
{
	m_command_queue->Signal(m_fence.Get(), m_fence_counter.at(m_frame_index));

	m_fence->SetEventOnCompletion(m_fence_counter.at(m_frame_index), m_fence_event);

	WaitForSingleObjectEx(m_fence_event, INFINITE, false);

	++m_fence_counter.at(m_frame_index);
}