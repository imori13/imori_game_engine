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
		Expects(0 <= index && index < m_descriptorCount);
		return D3D12_CPU_DESCRIPTOR_HANDLE{ m_hHeap.ptr + m_incrimentSize * index };
	}

	inline void createRenderTargetView(ID3D12Resource* pResource)
	{
		Expects(this != nullptr);
		D3D12_RENDER_TARGET_VIEW_DESC view_desc{};
		view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		view_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipSlice = 0;
		view_desc.Texture2D.PlaneSlice = 0;

		m_descriptors.at(m_emptyViewHandleIndex) = at(m_emptyViewHandleIndex);

		m_pDevice->CreateRenderTargetView(pResource, &view_desc, at(m_emptyViewHandleIndex));
		next();
	}

private:
	inline void next()
	{
		Expects(this != nullptr);
		Expects(0 <= m_emptyViewHandleIndex && m_emptyViewHandleIndex + 1 <= m_descriptorCount);

		++m_emptyViewHandleIndex;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_hHeap;
	uint64_t m_incrimentSize;
	gsl::not_null<ID3D12Device*> m_pDevice;

	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_descriptors;

private:
	uint32_t m_descriptorCount;
	uint32_t m_emptyViewHandleIndex;
};