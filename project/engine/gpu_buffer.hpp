#pragma once
#include "vertex.hpp"

template<class T>
class gpu_buffer
{
public:
	gpu_buffer(gsl::not_null<ID3D12Device*> pDevice, size_t buffer_size)
		: m_buffer_size(buffer_size)
	{
		// setting heap
		D3D12_HEAP_PROPERTIES prop{};
		prop.Type = D3D12_HEAP_TYPE_UPLOAD;
		prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		prop.CreationNodeMask = 1;
		prop.VisibleNodeMask = 1;

		// setting resource
		D3D12_RESOURCE_DESC desc{};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment = 0;
		desc.Width = buffer_size;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		// create resource
		const auto& hr = pDevice->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(m_resource.GetAddressOf())
		);
		Ensures(SUCCEEDED(hr));
	}

public:
	void map(gsl::span<T> span)
	{
		// mapping
		void* ptr = nullptr;
		const auto& hr = m_resource->Map(0, nullptr, &ptr);
		Ensures(SUCCEEDED(hr));

		Expects(span.size_bytes() == m_buffer_size);
		memcpy_s(ptr, m_buffer_size, span.data(), m_buffer_size);

		m_resource->Unmap(0, nullptr);
	}

	// vertex_buffer_view
	inline D3D12_VERTEX_BUFFER_VIEW get_vertex_buffer_view()
	{
		D3D12_VERTEX_BUFFER_VIEW vertexbuffer_view{};
		vertexbuffer_view.BufferLocation = m_resource->GetGPUVirtualAddress();
		vertexbuffer_view.SizeInBytes = m_buffer_size;
		vertexbuffer_view.StrideInBytes = gsl::narrow<UINT>(sizeof(T));
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;

	size_t m_buffer_size;
};