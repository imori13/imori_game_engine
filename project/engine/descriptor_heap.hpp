#pragma once

class descriptor_heap
{
public:
	descriptor_heap() = default;
	descriptor_heap(gsl::not_null<ID3D12Device*> pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t descriptorCount);

public:
	inline D3D12_CPU_DESCRIPTOR_HANDLE at(uint32_t index) const noexcept
	{
		Expects(this != nullptr);
		Expects(0 <= index && index < m_descriptor_count);
		return D3D12_CPU_DESCRIPTOR_HANDLE{ m_heap_handle.ptr + m_incriment_size * index };
	}

	inline void createRenderTargetView(ID3D12Resource* resource)
	{
		Expects(this != nullptr);
		D3D12_RENDER_TARGET_VIEW_DESC view_desc{};
		view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		view_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipSlice = 0;
		view_desc.Texture2D.PlaneSlice = 0;

		m_descriptors.at(m_empty_header) = at(m_empty_header);

		m_device->CreateRenderTargetView(resource, &view_desc, at(m_empty_header));
		
		this->next();
	}

private:
	inline void next() noexcept
	{
		Expects(this != nullptr);
		Expects(0 <= m_empty_header && m_empty_header + 1 <= m_descriptor_count);

		++m_empty_header;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_heap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_heap_handle;
	uint64_t m_incriment_size;
	gsl::not_null<ID3D12Device*> m_device;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_descriptors;

private:
	uint32_t m_descriptor_count;
	uint32_t m_empty_header;
};