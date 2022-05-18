#include "pch.hpp"
#include "descriptor_heap.hpp"

using namespace Microsoft::WRL;

descriptor_heap::descriptor_heap(gsl::not_null<ID3D12Device*> device, D3D12_DESCRIPTOR_HEAP_TYPE heap_type, uint32_t descriptor_count)
	: m_descriptor_count(descriptor_count)
	, m_descriptors(descriptor_count)
	, m_device(device)
	, m_empty_header(0)
{
	// setting descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.NumDescriptors = descriptor_count;
	desc.Type = heap_type;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	desc.NodeMask = 0;

	// create descriptor heap
	const auto& hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(m_heap.GetAddressOf()));
	Ensures(SUCCEEDED(hr));

	m_heap_handle = m_heap->GetCPUDescriptorHandleForHeapStart();
	m_incriment_size = device->GetDescriptorHandleIncrementSize(heap_type);
}