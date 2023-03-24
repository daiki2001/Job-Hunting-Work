#include "./Header/RenderTexture.h"
#include "./Header/DirectXInit.h"
#include <cassert>

float RenderTexture::clearColor[4] = { 1.0f, 0.5f, 0.0f, 0.0f };

RenderTexture::RenderTexture()
{
}

RenderTexture::~RenderTexture()
{
}

RenderTexture* RenderTexture::Get()
{
	static RenderTexture instance = {};
	return &instance;
}

HRESULT RenderTexture::CreateRenderTexture(vector<Textrue>* const texBuff,
										   ComPtr<ID3D12DescriptorHeap>* descHeapSRV, const UINT& texCount)
{
	HRESULT hr = S_FALSE;
	DirectXInit* w = DirectXInit::GetInstance();
	auto dev = DirectXInit::GetDevice();

	texBuff->resize(texCount);

	// �e�N�X�`�����\�[�X�ݒ�
	D3D12_RESOURCE_DESC texResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		static_cast<UINT64>(w->windowWidth),
		static_cast<UINT>(w->windowHeight),
		1,
		0,
		1,
		0,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	for (size_t i = 0; i < texCount; i++)
	{
		// �e�N�X�`���o�b�t�@�̐���
		hr = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
									 D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texResDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, //�e�N�X�`���p�w��
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
			IID_PPV_ARGS(&(*texBuff)[i].texbuff)
		);
		if (FAILED(hr))
		{
			return hr;
		}

		// �e�N�X�`����ԃN���A
		{
			// ��f��
			const UINT pixleCount = w->windowWidth * w->windowHeight;
			// �摜��s���̃f�[�^�T�C�Y
			const UINT rowPitch = sizeof(UINT) * w->windowWidth;
			// �摜�S�̂̃f�[�^�T�C�Y
			const UINT depthPitch = rowPitch * w->windowHeight;
			// �摜�C���[�W
			UINT* img = new UINT[pixleCount];
			for (size_t j = 0; j < pixleCount; j++)
			{
				img[j] = 0xFF0000FF;
			}

			(*texBuff)[i].name = std::to_wstring(i);
			hr = (*texBuff)[i].texbuff->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
			if (FAILED(hr))
			{
				assert(0);
				return hr;
			}

			delete[] img;
		}
	}

	// SRV�p�f�X�N���v�^�q�[�v��ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = texCount;
	// SRV�p�f�X�N���v�^�q�[�v�𐶐�
	hr = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&(*descHeapSRV)));
	if (FAILED(hr))
	{
		return hr;
	}

	// �V�F�[�_�[���\�[�X�r���[�̍쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < static_cast<int>(texCount); i++)
	{
		// �f�X�N���v�^�q�[�v�̐擪�n���h��(CPU)���擾
		(*texBuff)[i].cpuDescHandle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				(*descHeapSRV)->GetCPUDescriptorHandleForHeapStart(),
				i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

		// �f�X�N���v�^�q�[�v�̐擪�n���h��(GPU)���擾
		(*texBuff)[i].gpuDescHandle =
			CD3DX12_GPU_DESCRIPTOR_HANDLE(
				(*descHeapSRV)->GetGPUDescriptorHandleForHeapStart(),
				i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

		// �f�X�N���v�^�q�[�v��SRV�쐬
		dev->CreateShaderResourceView(
			(*texBuff)[i].texbuff.Get(),
			&srvDesc,
			(*texBuff)[i].cpuDescHandle);
	}

	return hr;
}

HRESULT RenderTexture::CreateRTV(ComPtr<ID3D12DescriptorHeap>* descHeapRTV,
								 const vector<Textrue>& texBuff)
{
	HRESULT hr = S_FALSE;
	auto dev = DirectXInit::GetDevice();

	// RTV�p�f�X�N���v�^�q�[�v��ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc = {};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = static_cast<int>(texBuff.size());
	// RTV�p�f�X�N���v�^�q�[�v�𐶐�
	hr = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&(*descHeapRTV)));
	if (FAILED(hr))
	{
		return hr;
	}

	for (int i = 0; i < static_cast<int>(texBuff.size()); i++)
	{
		// �����_�[�^�[�Q�b�g�r���[�̐���
		dev->CreateRenderTargetView(
			texBuff[i].texbuff.Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				(*descHeapRTV)->GetCPUDescriptorHandleForHeapStart(),
				i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
			)
		);
	}

	return hr;
}
