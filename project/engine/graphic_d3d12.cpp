#include "pch.hpp"
#include "graphic_d3d12.hpp"
#include "descriptor_heap.hpp"
#include "gpu_buffer.hpp"
#include "d3d12_factory.hpp"

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
		m_heap_rtv = std::make_unique<descriptor_heap>(m_device.Get(), FRAME_COUNT, heap_type::rtv);

		for(auto i = 0u; i < m_color_buffer.size(); ++i)
		{
			m_color_buffer.at(i) = get_color_buffer(m_swapchain.Get(), i);

			m_heap_rtv->create_rtv(m_color_buffer.at(i).Get());
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

	descriptor_heap constract_heap(m_device.Get(), FRAME_COUNT, heap_type::cbv_srv_uav, heap_flag::shader_visible);
	constract_heap.create_cbv(transform_buffer.get_resource());
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

	m_command_list->ClearRenderTargetView(m_heap_rtv->at(m_frame_index).cpu_handle, clear_color.data(), 0, nullptr);

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