#include "pch.hpp"
#include "descriptor_heap.hpp"

using namespace Microsoft::WRL;

descriptor_heap::descriptor_heap(gsl::not_null<ID3D12Device*> pDevice, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t descriptorCount)
	: m_descriptorCount(descriptorCount)
	, m_descriptors(descriptorCount)
	, m_pDevice(pDevice)
	, m_emptyViewHandleIndex(0)
{
	// setting descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = descriptorCount;
	desc.Type = heapType;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	// create descriptor heap
	const auto& hr = pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_pHeap.GetAddressOf()));
	Ensures(SUCCEEDED(hr));

	m_hHeap = m_pHeap->GetCPUDescriptorHandleForHeapStart();
	m_incrimentSize = pDevice->GetDescriptorHandleIncrementSize(heapType);
}