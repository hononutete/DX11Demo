//#include "stdafx.h"
//#include "UCommon.h"
#include "D3D11User.h"
#include <dxgi.h>
#include <d3dcompiler.h>
//#include <D3DX11async.h>

D3D11User::D3D11User()
{
	m_D3DDevice = NULL;
	m_Adapter = NULL;
	m_D3DDeviceContext = NULL;
	m_SwapChain = NULL;
	m_RenderTargetView = NULL;
	m_DepthStencilView = NULL;
	//m_hWnd = NULL;
	
}


D3D11User::~D3D11User()
{
	Invalidate();
}
HRESULT D3D11User::InitD3D11(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = E_FAIL;

	//�E�B���h�E�̍쐬
	//hr = InitWindow(WinProc, hInstance, AppName);
	//if (FAILED(hr)) goto EXIT;

	//�A�_�v�^�[�쐬
	hr = CreateAdapter();
	if (FAILED(hr)) goto EXIT;

	//�f�o�C�X�̍쐬�i�`��@�\�j
	hr = CreateDevice();
	if (FAILED(hr)) goto EXIT;

	//�X���b�v�`�F�[���쐬�i�o�b�t�@���\�[�X���Ǘ��j
	hr = CreateSwapChain(hWnd);
	if (FAILED(hr)) goto EXIT;

	//�����_�[�^�[�Q�b�g�r���[���쐬
	hr = CreateRenderTargetView();
	if (FAILED(hr)) goto EXIT;

	//�[�x�X�e���V���r���[�̍쐬
	hr = CreateDepthStencilView();
	if (FAILED(hr)) goto EXIT;

	//�f�o�C�X�ƃ����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���o�b�t�@�[�̕R�t��
	m_D3DDeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);

	//�r���[�|�[�g�ݒ�
	hr = CreateViewport();
	if (FAILED(hr)) goto EXIT;

	//�f�t�H���g�̃��X�^���C�U�ݒ�
	hr = SetDefaultRasterize();
	if (FAILED(hr)) goto EXIT;

	//�f�t�H���g�̐[�x�X�e���V���X�e�[�g�̐ݒ�
	hr = SetDefaultDepthStencilState();
	if (FAILED(hr)) goto EXIT;

	OutputDebugString(_T("D3D Initialization Completed!\n"));

	hr = S_OK;

EXIT:
	return hr;
}


HRESULT D3D11User::InitD3D11(WNDPROC WinProc, HINSTANCE hInstance, TCHAR* AppName)
{
	HRESULT hr = E_FAIL;

	//�E�B���h�E�̍쐬
	hr = InitWindow(WinProc, hInstance, AppName);
	if (FAILED(hr)) goto EXIT;

	//�A�_�v�^�[�쐬
	hr = CreateAdapter();
	if (FAILED(hr)) goto EXIT;

	//�f�o�C�X�̍쐬�i�`��@�\�j
	hr = CreateDevice();
	if (FAILED(hr)) goto EXIT;

	//�X���b�v�`�F�[���쐬�i�o�b�t�@���\�[�X���Ǘ��j
	hr = CreateSwapChain();
	if (FAILED(hr)) goto EXIT;

	//�����_�[�^�[�Q�b�g�r���[���쐬
	hr = CreateRenderTargetView();
	if (FAILED(hr)) goto EXIT;

	//�[�x�X�e���V���r���[�̍쐬
	hr = CreateDepthStencilView();
	if (FAILED(hr)) goto EXIT;

	//�f�o�C�X�ƃ����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���o�b�t�@�[�̕R�t��
	m_D3DDeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);

	//�r���[�|�[�g�ݒ�
	hr = CreateViewport();
	if (FAILED(hr)) goto EXIT;

	//�f�t�H���g�̃��X�^���C�U�ݒ�
	hr = SetDefaultRasterize();
	if (FAILED(hr)) goto EXIT;

	//�f�t�H���g�̐[�x�X�e���V���X�e�[�g�̐ݒ�
	hr = SetDefaultDepthStencilState();
	if (FAILED(hr)) goto EXIT;

	OutputDebugString(_T("D3D Initialization Completed!\n"));

	hr = S_OK;

EXIT:
	return hr;
}

void D3D11User::Invalidate()
{
	ULONG cnt = 0;

	//�����_�[�^�[�Q�b�g�r���[�̃��������
	if (m_RenderTargetView)
	{
		cnt = m_RenderTargetView->Release();
		if (cnt != 0) OutputDebugString(_T("RenderTargetView reference counter invalid\n"));
		m_RenderTargetView = NULL;
	}

	if (m_SwapChain)
	{
		m_SwapChain->Release();
		if (cnt != 0) OutputDebugString(_T("SwapChain reference counter invalid\n"));
		m_SwapChain = NULL;
	}

	if (m_D3DDeviceContext)
	{
		m_D3DDeviceContext->Release();
		if (cnt != 0) OutputDebugString(_T("DeviceContext reference counter invalid\n"));
		m_D3DDeviceContext = NULL;
	}

	if (m_D3DDevice)
	{
		m_D3DDevice->Release();
		if (cnt != 0) OutputDebugString(_T("Device reference counter invalid\n"));
		m_D3DDevice = NULL;
	}

	if (m_SwapChain)
	{
		m_SwapChain->Release();
		if (cnt != 0) OutputDebugString(_T("RenderTargetView reference counter invalid\n"));
		m_SwapChain = NULL;
	}

	if (m_DepthStencilView)
	{
		m_DepthStencilView->Release();
		if (cnt != 0) OutputDebugString(_T("DepthStencilView reference counter invalid\n"));
		m_DepthStencilView = NULL;
	}

	if (m_Adapter)
	{
		m_Adapter->Release();
		if (cnt != 0) OutputDebugString(_T("Adapter reference counter invalid\n"));
		m_Adapter = NULL;
	}
}

HRESULT D3D11User::InitWindow(WNDPROC WinProc, HINSTANCE hInstance, TCHAR* AppName)
{
	HRESULT hr = E_FAIL;

//	//�E�B���h�E�N���X�̍쐬
//	WNDCLASSEX wc;
//	wc.cbSize = sizeof(WNDCLASSEX);
//	wc.style = CS_VREDRAW | CS_HREDRAW;
//	wc.lpfnWndProc = WinProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = sizeof(DWORD);
//	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
//	wc.hIcon = NULL;
//	wc.hIconSm = NULL;
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = AppName;
//	wc.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
//	wc.hInstance = hInstance;
//
//	//�E�B���h�E�N���X�̓o�^
//	RegisterClassEx(&wc);
//
//	//�E�B���h�E�̍쐬
//	m_hWnd = ::CreateWindow(
//		wc.lpszClassName,
//		wc.lpszClassName,
//		WS_OVERLAPPEDWINDOW,
//		100,
//		100,
//		300,
//		300,
//		NULL,
//		NULL,
//		hInstance,
//		NULL
//		);
//
//	if (m_hWnd == NULL)
//	{
//		OutputDebugString(_T("Error: Window Creation Failed\n"));
//		hr = E_FAIL;
//		goto EXIT;
//	}
//
//	OutputDebugString(_T("Window Creation Succeeded!\n"));
//	hr = S_OK;
//EXIT:
//	
	return hr;
}

HRESULT D3D11User::CreateAdapter()
{
	HRESULT hr = E_FAIL;
	IDXGIFactory* pFactory = NULL;

	if (m_Adapter == NULL)
	{
		//�t�@�N�g���̍쐬�i�f�t�H���g�A�_�v�^�[���擾����̂ɕK�v�j
		hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
		if (FAILED(hr)) goto EXIT;

		//�f�t�H���g�A�_�v�^�[���擾
		hr = pFactory->EnumAdapters(0, &m_Adapter);
		if (FAILED(hr)) goto EXIT;
	}

	OutputDebugString(_T("Adapter Creation Succeeded!\n"));
	hr = S_OK;

	//�I������
EXIT:
	SAFE_RELEASE( pFactory );
	
	return hr;
}

HRESULT D3D11User::CreateDevice()
{
	//���ꂼ��̃o�[�W�����ŃT�|�[�g����Ă���@�\���^�[�Q�b�g�Ƃ���
	D3D_FEATURE_LEVEL FeatureLevel[] = {
		D3D_FEATURE_LEVEL_11_0,  // Direct3D 11.0 ShaderModel 5
		D3D_FEATURE_LEVEL_10_1,  // Direct3D 10.1 SM 4
		D3D_FEATURE_LEVEL_10_0,  // Direct3D 10.0 SM 4
		D3D_FEATURE_LEVEL_9_3,   // Direct3D 9.3  SM 3
		D3D_FEATURE_LEVEL_9_2,   // Direct3D 9.2  SM 2
		D3D_FEATURE_LEVEL_9_1,   // Direct3D 9.1  SM 2
	};

	//D3D_FEATURE_LEVEL�z��̗v�f��
	int featureCnt = sizeof(FeatureLevel) / sizeof(D3D_FEATURE_LEVEL);

	//�ڎw���ׂ�����
	HRESULT hr = D3D11CreateDevice(
		m_Adapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		FeatureLevel,
		featureCnt,
		D3D11_SDK_VERSION,
		&m_D3DDevice,
		&m_FeatureLevel,
		&m_D3DDeviceContext
		);

	if (FAILED (hr))
	{
		OutputDebugString(_T("Error : Device Creation Failed\n"));
		return E_FAIL;
	}

	OutputDebugString(_T("Device Creation Succeeded!\n"));

	return S_OK;
	
}

HRESULT D3D11User::CreateSwapChain()
{
//	HRESULT hr = E_FAIL;
//
//	//DXGI_SWAP_CHAIN_DESC�\���́i�X���b�v�`�F�C���̔\�͂��w��j
//	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
//	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
//	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //�o�b�N�o�b�t�@�t�H�[�}�b�g
//	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�o�b�N�o�b�t�@�̃T�[�t�F�X�g�p�@����� CPU �A�N�Z�X �I�v�V������\�� DXGI_USAGE �񋓌^�̃����o�[�B�o�b�N�o�b�t�@�Ƀ����_�����O�\�ɂ���
//	SwapChainDesc.BufferCount = 1; //�X���b�v�`�F�[���̃o�b�t�@�̐�
//	SwapChainDesc.OutputWindow = m_hWnd; //�o�̓E�B���h�E�ւ�HWND�n���h��
//	SwapChainDesc.Windowed = TRUE;
//	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //�T�[�t�F�X�̃X���b�v������Ƀo�b�N�o�b�t�@�̓��e��ێ����邩�H�o�b�N�o�b�t�@�̓��e��ێ����Ȃ�
//	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //�X���b�v�`�F�[���̕t���\�͂��w��B�t���X�N���[���ƃE�B���h�E�̉𑜓x�ύX�������ōs���Ă����
//
//	//�}���`�T���v�����O�̐ݒ�i�A���`�G�C���A�X�j
//	SwapChainDesc.SampleDesc.Count = 1; //�P�s�N�Z���̐F�����߂邽�߂̃T���v�����O��
//	SwapChainDesc.SampleDesc.Quality = 0; //���x���x��
//
//	IDXGIFactory* pFactory;
//	//IDXGIFactory�́A�t���X�N���[���؂�ւ����������� DXGI �I�u�W�F�N�g�������\�b�h������
//	//IDXGIFactory���g���ăX���b�v�`�F�[�����쐬����
//	//CreateDXGIFactory��DXGIFactory�𐶐������ꍇ�C�ȍ~��IDXGIFactory1�C2�C3�C4�̃C���^�[�t�F�[�X���T�|�[�g�����I�u�W�F�N�g�ɂ͂Ȃ�Ȃ�
//	//hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
//
//	//IDXGIAdapter����쐬
//	hr = m_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
//
//	if (FAILED(hr))
//	{
//		OutputDebugString(_T("Error: IDXGIFactory Creation Failed\n"));
//		goto EXIT;
//	}
//
//	//�X���b�v�`�F�[����IDXGIFactory���g���Đ�������
//	hr = pFactory->CreateSwapChain(m_D3DDevice, &SwapChainDesc, &m_SwapChain);
//
//	if (FAILED(hr))
//	{
//		OutputDebugString(_T("Error: SwapChain Creation Failed\n"));
//		goto EXIT;
//	}
//
//	OutputDebugString(_T("SwapChain Creation Succeeded!\n"));
//
//	hr = S_OK;
//
//	//�I������
//EXIT:
//	SAFE_RELEASE(pFactory);
//
//	return hr;
	return E_FAIL;
}

HRESULT D3D11User::CreateSwapChain(HWND hWnd) {
	HRESULT hr = E_FAIL;

	//DXGI_SWAP_CHAIN_DESC�\���́i�X���b�v�`�F�C���̔\�͂��w��j
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //�o�b�N�o�b�t�@�t�H�[�}�b�g
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�o�b�N�o�b�t�@�̃T�[�t�F�X�g�p�@����� CPU �A�N�Z�X �I�v�V������\�� DXGI_USAGE �񋓌^�̃����o�[�B�o�b�N�o�b�t�@�Ƀ����_�����O�\�ɂ���
	SwapChainDesc.BufferCount = 1; //�X���b�v�`�F�[���̃o�b�t�@�̐�
	SwapChainDesc.OutputWindow = hWnd; //�o�̓E�B���h�E�ւ�HWND�n���h��
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //�T�[�t�F�X�̃X���b�v������Ƀo�b�N�o�b�t�@�̓��e��ێ����邩�H�o�b�N�o�b�t�@�̓��e��ێ����Ȃ�
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //�X���b�v�`�F�[���̕t���\�͂��w��B�t���X�N���[���ƃE�B���h�E�̉𑜓x�ύX�������ōs���Ă����

																  //�}���`�T���v�����O�̐ݒ�i�A���`�G�C���A�X�j
	SwapChainDesc.SampleDesc.Count = 1; //�P�s�N�Z���̐F�����߂邽�߂̃T���v�����O��
	SwapChainDesc.SampleDesc.Quality = 0; //���x���x��

	IDXGIFactory* pFactory;
	//IDXGIFactory�́A�t���X�N���[���؂�ւ����������� DXGI �I�u�W�F�N�g�������\�b�h������
	//IDXGIFactory���g���ăX���b�v�`�F�[�����쐬����
	//CreateDXGIFactory��DXGIFactory�𐶐������ꍇ�C�ȍ~��IDXGIFactory1�C2�C3�C4�̃C���^�[�t�F�[�X���T�|�[�g�����I�u�W�F�N�g�ɂ͂Ȃ�Ȃ�
	//hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));

	//IDXGIAdapter����쐬
	hr = m_Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	if (FAILED(hr))
	{
		OutputDebugString(_T("Error: IDXGIFactory Creation Failed\n"));
		goto EXIT;
	}

	//�X���b�v�`�F�[����IDXGIFactory���g���Đ�������
	hr = pFactory->CreateSwapChain(m_D3DDevice, &SwapChainDesc, &m_SwapChain);

	if (FAILED(hr))
	{
		OutputDebugString(_T("Error: SwapChain Creation Failed\n"));
		goto EXIT;
	}

	OutputDebugString(_T("SwapChain Creation Succeeded!\n"));

	hr = S_OK;

	//�I������
EXIT:
	SAFE_RELEASE(pFactory);

	return hr;

}
//�[�x�X�e���V���r���[�̍쐬
//�[�x�o�b�t�@�ƃX�e���V���o�b�t�@�𓯎��ɍ쐬
HRESULT D3D11User::CreateDepthStencilView()
{
	HRESULT hr = E_FAIL;
	ID3D11Texture2D *pDepthBuffer = NULL; //�[�x�o�b�t�@�p�e�N�X�`��

	//�X���b�v�`�F�[���̏����擾�i�[�x�X�e���V���o�b�t�@�̃T�C�Y�Ȃǂ̓o�b�N�o�b�t�@�Ɠ����łȂ���΂����Ȃ��j
	DXGI_SWAP_CHAIN_DESC chainDesc;
	hr = m_SwapChain->GetDesc(&chainDesc);

	//�[�x�o�b�t�@�p�e�N�X�`���ݒ�̍\����
	D3D11_TEXTURE2D_DESC descDS; 
	::ZeroMemory(&descDS, sizeof(D3D11_TEXTURE2D_DESC));
	descDS.Width              = chainDesc.BufferDesc.Width;    //�o�b�N�o�b�t�@�Ɠ���
	descDS.Height             = chainDesc.BufferDesc.Height;   //�o�b�N�o�b�t�@�Ɠ���
	descDS.MipLevels          = 1;                             //�~�b�v�}�b�v�͍쐬���Ȃ�
	descDS.ArraySize          = 1;                             //�e�N�X�`���̔z��
	descDS.Format             = DXGI_FORMAT_R16_TYPELESS;      //�t�H�[�}�b�g 
	descDS.SampleDesc.Count   = chainDesc.SampleDesc.Count;    //�o�b�N�o�b�t�@�Ɠ���
	descDS.SampleDesc.Quality = chainDesc.SampleDesc.Quality;  //�o�b�N�o�b�t�@�Ɠ���
	descDS.Usage              = D3D11_USAGE_DEFAULT;           //GPU�ɂ��ǂݎ�肨��я������݃A�N�Z�X��K�v�Ƃ��郊�\�[�X
	descDS.BindFlags          = D3D11_BIND_DEPTH_STENCIL |     //�[�x�X�e���V���o�b�t�@�Ƃ��č쐬
		                        D3D11_BIND_SHADER_RESOURCE;    //�V�F�[�_�[���\�[�X�r���[�Ƃ��č쐬
	descDS.CPUAccessFlags     = 0;                             //CPU�A�N�Z�X�͕s�v
	descDS.MiscFlags          = 0;                             //���̑��̃t���O���ݒ肵�Ȃ�
	
	//�[�x�o�b�t�@�p�̃e�N�X�`���[�쐬
	hr = m_D3DDevice->CreateTexture2D(&descDS, NULL, &pDepthBuffer);
	if (FAILED(hr)){
		OutputDebugString(_T("DepthStencil surface texture creation failed...\n"));
		goto EXIT;
	}
	


	//�[�x�X�e���V���r���[�̐ݒ�̍\���́B�[�x�X�e���V���r���[����鎞��
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	::ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	descDSV.Format             = DXGI_FORMAT_D16_UNORM; //�e�N�X�`���̃t�H�[�}�b�g�ƌ݊���������t�H�[�}�b�g���w��
	descDSV.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D; //�}���`�T���v�����g�p���Ȃ��ꍇ�i�g�p����ꍇ��D3D11_DSV_DIMENSION_TEXTURE2DMS�j
	descDSV.Texture2D.MipSlice = 0;

	//�[�x�X�e���V���r���[�̍쐬
	hr = m_D3DDevice->CreateDepthStencilView(pDepthBuffer, &descDSV, &m_DepthStencilView);
	if (FAILED(hr)){
		OutputDebugString(_T("DepthStencilView creation failed...\n"));
		goto EXIT;
	}

	hr = S_OK;
	OutputDebugString(_T("DepthStencilView creation succeeded!\n"));

	//�I������
EXIT:
	SAFE_RELEASE(pDepthBuffer);

	return hr;

}

//�f�t�H���g�̐[�x�X�e���V���X�e�[�g��ݒ�
HRESULT D3D11User::SetDefaultDepthStencilState()
{
	HRESULT hr = E_FAIL;

	//�[�x�X�e���V���X�e�[�g�C���^�[�t�F�C�X
	ID3D11DepthStencilState *pDepthStencilState = NULL;

	//�[�x�X�e���V���X�e�[�g�\����
	//���̍\���̂ł́A�[�x�o�b�t�@�ƃX�e���V���o�b�t�@�̎g�p���@�𐧌䂷��
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	::ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable    = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc      = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable  = FALSE;

	//�[�x�X�e���V���X�e�[�g���쐬
	hr = m_D3DDevice->CreateDepthStencilState(&dsDesc, &pDepthStencilState);
	if (FAILED(hr)){
		OutputDebugString(_T("DepthStencilState creation failed...\n"));
		goto EXIT;
	}

	//�[�x�X�e���V���X�e�[�g�C���^�[�t�F�C�X���R���e�L�X�g�ɐݒ�
	//�������͊�Ƃ��Ďg�p�����Q�ƒl
	m_D3DDeviceContext->OMSetDepthStencilState( pDepthStencilState, 0);

	hr = S_OK;
	OutputDebugString(_T("DepthStencilState creation succeeded!\n"));

	//�I������
EXIT:
	SAFE_RELEASE(pDepthStencilState);

	return hr;
}

HRESULT D3D11User::SetDefaultRasterize()
{
	HRESULT hr = E_FAIL;

	//���X�^���C�U�[�X�e�[�W�̃��X�^���C�U�[�X�e�[�g�ɃA�N�Z�X���邽�߂̃C���^�[�t�F�C�X
	ID3D11RasterizerState* g_pRasterState = NULL;

	//���X�^���C�U�[�ݒ���s���\����
	D3D11_RASTERIZER_DESC rsState;
	rsState.FillMode = D3D11_FILL_SOLID; //�|���S���ʕ`��
	rsState.CullMode = D3D11_CULL_BACK;  //���ʂ�`�悵�Ȃ�
	//rsState.FrontCounterClockwise = FALSE; //���v����\�ʂƂ���
	rsState.FrontCounterClockwise = TRUE; //�����v����\�ʂƂ���
	rsState.DepthBias = 0;
	rsState.DepthBiasClamp = 0;
	rsState.SlopeScaledDepthBias = 0;
	rsState.DepthClipEnable = TRUE;
	rsState.ScissorEnable = FALSE; //�V�U�[��`����

	//���X�^���C�U�[�̃}���`�T���v�����O�̐ݒ�́A�X���b�v�`�F�[���̃}���`�T���v�����O�ݒ�ɍ��킹��
	DXGI_SWAP_CHAIN_DESC swapDesc;
	m_SwapChain->GetDesc(&swapDesc);
	if (swapDesc.SampleDesc.Count != 1)
		rsState.MultisampleEnable = TRUE;
	else
		rsState.MultisampleEnable = FALSE;

	rsState.AntialiasedLineEnable = FALSE; //�A���`�G�C���A�X�̐ݒ�


	//���X�^���C�U�ݒ�\���̂���A���X�^���C�U�[�X�e�[�g�C���^�[�t�F�C�X���쐬����B
	hr = m_D3DDevice->CreateRasterizerState(&rsState, &g_pRasterState);
	if (FAILED(hr)){
		OutputDebugString(_T("error: Default Rasterize State Creation failed...\n"));
		goto EXIT;
	}

	//�f�o�C�X�R���e�L�X�g�Ƀ��X�^���C�U�[�X�e�[�g��ݒ肷��B���ꂪ�������X�^���C�U�ݒ�ɂȂ�
	m_D3DDeviceContext->RSSetState(g_pRasterState);

	OutputDebugString(_T("Default Rasterize State set succeeded!\n"));

EXIT:
	SAFE_RELEASE(g_pRasterState);
	return hr;

}

HRESULT D3D11User::CreateRenderTargetView()
{
	HRESULT hr = E_FAIL;
	ID3D11Texture2D *pBackBuffer = NULL;

	//�o�b�N�o�b�t�@�̓X���b�v�`�F�[���̒��ɂ���
	//��P�����F�A�N�Z�X����o�b�t�@�C���f�b�N�X�B����Ȃ�ȊO0����
	//��Q�����F�g�p���郊�\�[�X�C���^�[�t�F�C�X�̎�ށiGUID�j
	//��R�����F�o�b�N�o�b�t�@�ւ̃|�C���^���A���Ă���
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	if (FAILED(hr))
	{
		OutputDebugString(_T("Error : BackBuffer extraction from SwapChain failed\n"));
		goto EXIT;
	}
	
	//�����_�[�^�[�Q�b�g�r���[�쐬
	//��P�����FID3D11Resource��ID3D11Texture2D�ȂǁA�o�b�t�@�ƃe�N�X�`���̊��C���^�[�t�F�C�X�ł���
	//��Q�����F�����_�[�^�[�Q�b�g�r���[�̔\�͂��\���̂Ŏw��B2D�o�b�t�@�ł�NULL�ł����B
	//��R�����F�����_�[�^�[�Q�b�g�r���[�ւ̃|�C���^���A���Ă���
	hr = m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	
	if (FAILED(hr))
	{
		OutputDebugString(_T("Error : RederTargetView creation failed\n"));
		goto EXIT;
	}

	OutputDebugString(_T("RederTargetView creation succeeded!\n"));

	hr = S_OK;

	//�I������
	EXIT:
	SAFE_RELEASE(pBackBuffer);

	return hr;
}

HRESULT D3D11User::CreateViewport()
{
	HRESULT hr = E_FAIL;

	//�r���[�|�[�g�̐ݒ�Ƀo�b�N�o�b�t�@�̕��ƍ������K�v
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	hr = m_SwapChain->GetDesc(&SwapChainDesc);
	if (FAILED(hr)) goto EXIT;

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT Viewport[1];
	Viewport[0].TopLeftX = 0;
	Viewport[0].TopLeftY = 0;
	Viewport[0].Width    = (FLOAT)SwapChainDesc.BufferDesc.Width;
	Viewport[0].Height   = (FLOAT)SwapChainDesc.BufferDesc.Height;
	Viewport[0].MinDepth = 0.0f;
	Viewport[0].MaxDepth = 1.0f;

	m_D3DDeviceContext->RSSetViewports(1, Viewport);
	OutputDebugString(_T("Set Viewport Succeeded!\n"));

	hr = S_OK;

EXIT:

	return hr;
}

HRESULT D3D11User::CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, D3D11_BIND_FLAG BindFlag, UINT CPUAccessFlag)
{
	HRESULT hr = E_FAIL;

	//���_�o�b�t�@���ǂ̂悤�Ȃ��̂Ȃ̂���D3D11_BUFFER_DESC�\���̂ɓo�^
	D3D11_BUFFER_DESC BufferDesc;
	::ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth      = size;                    //���_�̐�
	if (CPUAccessFlag == 0)
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;          //���_�o�b�t�@��CPU��GPU����ǂ��A�N�Z�X����邩�̃t���O
	else
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;          //���_�o�b�t�@��CPU��GPU����ǂ��A�N�Z�X����邩�̃t���O�A��������CPUAccessFlag��D3D11_CPU_ACCESS_WRITE�̎��A
	BufferDesc.BindFlags      = BindFlag;                //�o�b�t�@�̎�ށi���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�Ȃǁj���w��
	BufferDesc.CPUAccessFlags = CPUAccessFlag;           //�o�b�t�@�ɑ΂���CPU�̃A�N�Z�X�������w��
	BufferDesc.MiscFlags      = 0;                       //���ɗ��R���Ȃ���΂O

	//���ۂ̒��_�����\���̂ɓo�^����K�v������
	//D3D11_SUBRESOURCE_DATA SubresourceData;
	//SubresourceData.pSysMem          = pData; //���_�z��̐퓬�|�C���^
	//SubresourceData.SysMemPitch      = 0;     //�e�N�X�`���Ɋւ���l
	//SubresourceData.SysMemSlicePitch = 0;     //�e�N�X�`���Ɋւ���l

	D3D11_SUBRESOURCE_DATA* resource = NULL;
	if (pData)
	{
		resource = new D3D11_SUBRESOURCE_DATA();
		resource->pSysMem = pData;
		resource->SysMemPitch = 0;
		resource->SysMemSlicePitch = 0;
	}

	//�o�b�t�@�쐬
	hr = m_D3DDevice->CreateBuffer(&BufferDesc, resource, pBuffer);

	if (FAILED(hr)){
		OutputDebugString(_T("Error: Buffer Creation failed\n"));
		goto EXIT;
	}
	//OutputDebugString(_T("Buffer Creation Succeeded!\n"));

	hr = S_OK;
EXIT:

	return hr;
}

HRESULT D3D11User::CreateVertexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, D3D11_BIND_VERTEX_BUFFER, CPUAccessFlag);
}

HRESULT D3D11User::CreateIndexBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, D3D11_BIND_INDEX_BUFFER, CPUAccessFlag);
}

HRESULT D3D11User::CreateConstantBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag)
{
	return CreateBuffer(pBuffer, pData, size, D3D11_BIND_CONSTANT_BUFFER, CPUAccessFlag);
}

//���_�V�F�[�_�[���t�@�C������쐬
HRESULT D3D11User::CreateVertexShaderFromFile(ID3D11VertexShader** pVertexShader, TCHAR pSrcFile[], const CHAR pFunctionName[], const CHAR pProfile[], ID3D11InputLayout** g_pLayout, D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements)
//HRESULT D3D11User::CreateVertexShaderFromFile(OUT ID3D11VertexShader** pVertexShader, IN TCHAR pSrcFile[], IN CHAR pFunctionName[], IN CHAR pProfile[], OUT ID3D11InputLayout** g_pLayout, IN D3D11_INPUT_ELEMENT_DESC pLayoutDesc[], UINT NumElements)
{
	HRESULT hr = E_FAIL;

	//�Ō�̃t���O�̓f�o�b�O�p
	//UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_OPTIMIZATION_LEVEL0;
	//ID3D10Blob* pBlob = NULL;
	//ID3D10Blob* pErrorBlob = NULL;

	//Direct3D 9 ���[�h�Ŏ��s���̏ꍇ�̓o�[�W�����̕ύX�̕K�v������
	//�@������͊�������
	//�@���e�Ƃ��Ă͎擾����m_FeatureLevel��D3D_FEATURE_LEVEL_?_?�𔻒肵Profile�����ւ���
	//CHAR Profile[256];
	//�s��͗�D��Őݒ�A�Â��`���̋L�q�͋����Ȃ�
	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
	ID3DBlob* pBlobN = NULL;

	//�t�@�C�������ƂɃG�t�F�N�g���R���p�C��
	hr = D3DCompileFromFile(
		pSrcFile,       //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C����
		nullptr,        //�V�F�[�_�[���Ń}�N�����g���ꍇ�A���̃}�N����`�̔z��ւ̃|�C���^
		nullptr,        //�V�F�[�_�[����#include���g���ꍇ�A�C���^�[�t�F�C�X�ւ̃|�C���^
		pFunctionName,  //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
		pProfile,        //�V�F�[�_�[���f�����w��
		Flag1,          //�V�F�[�_�[�ɑ΂���R���p�C���I�v�V����
		0,              //�G�t�F�N�g�ɑ΂���R���p�C���I�v�V����
		&pBlobN,        //�R���p�C���ς݃V�F�[�_�[�ւ̃|�C���^
		nullptr         //�R���p�C�����̃G���[�ւ̃|�C���^
	);
	//hr = D3DX11CompileFromFile(
	//	pSrcFile,        //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C����
	//	NULL,            //�V�F�[�_�[���Ń}�N�����g���ꍇ�A���̃}�N����`�̔z��ւ̃|�C���^
	//	NULL,            //�V�F�[�_�[����#include���g���ꍇ�A�C���^�[�t�F�C�X�ւ̃|�C���^
	//	pFunctionName,   //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
	//	pProfile,         //�V�F�[�_�[���f�����w��
	//	Flag1,           //�V�F�[�_�[�R���p�C���t���O
	//	0,               //�G�t�F�N�g�R���p�C���t���O
	//	NULL,            //�X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[
	//	&pBlob,          //�R���p�C���ς݃V�F�[�_�[�ւ̃|�C���^
	//	&pErrorBlob,     //�R���p�C�����̃G���[�ւ̃|�C���^
	//	NULL);           //�߂�l�ւ̃|�C���^(NULL�ł���)
	if (hr == E_FAIL)
	{
		OutputDebugString(_T("Error: Vertex Shader Compile failed\n"));
		//if (pErrorBlob)
		//{
		//	OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		//}
		return E_FAIL;
		//goto EXIT;
	}
	

	//�R���p�C���ς݃V�F�[�_�[���璸�_�V�F�[�_�[�I�u�W�F�N�g���쐬
	hr = m_D3DDevice->CreateVertexShader(
		pBlobN->GetBufferPointer(), //�R���p�C���ς݃V�F�[�_�[�ւ̃|�C���^
		pBlobN->GetBufferSize(),    //�R���p�C���ς݃V�F�[�_�[�ւ̃T�C�Y
		NULL,                      //�Ƃ肠����NULL�ł���
		pVertexShader              //ID3D11VertexShader�C���^�[�t�F�C�X�ւ̃|�C���^
		);
	if (hr == E_FAIL)
	{
		OutputDebugString(_T("Error: Vertex Shader Object creation failed\n"));
		return E_FAIL;
		//goto EXIT;
	}


	//���_���C�A�E�g���쐬
	//�@�@�@�V�F�[�_�[�Ŏg�p�������̓o�b�t�@���L�q���邽�߂̒��_�i���́j���C�A�E�g�I�u�W�F�N�g���쐬
	//�@�@�@�v���O���}����`�������_�\���́iD3D11_INPUT_ELEMENT_DESC�j�ƃV�F�[�_�[���K�v�Ƃ�������C�荇�킹�Ȃ���΂����Ȃ�
	BYTE* vsbuffer = (LPBYTE)pBlobN->GetBufferPointer();
	size_t vsbuffersize = pBlobN->GetBufferSize();

	hr = m_D3DDevice->CreateInputLayout(
		pLayoutDesc,  //��`�������_�\����
		NumElements,  //pLayoutDesc�̓��̓f�[�^�^�̐�
		vsbuffer,     //�R���p�C���ς݃V�F�[�_�[�ւ̃|�C���^
		vsbuffersize, //�R���p�C���ς݃V�F�[�_�[�̃T�C�Y
		g_pLayout     //���_���C�A�E�g�ւ̃|�C���^
		);
	if (hr == E_FAIL){
		OutputDebugString(_T("Error: Input Layout creation failed\n"));
		goto EXIT;
	}

	hr = S_OK;
	OutputDebugString(_T("Vertex Shader Creation succeeded!\n"));

	//�I������
EXIT:
	SAFE_RELEASE(pBlobN);
	//SAFE_RELEASE( pErrorBlob );
	
	return hr;
}

//�s�N�Z���V�F�[�_�[���t�@�C������쐬
HRESULT D3D11User::CreatePixelShaderFromFile(ID3D11PixelShader** pPixelShader, TCHAR pSrcFile[], const CHAR pFunctionName[], const CHAR pProfile[])
//HRESULT D3D11User::CreatePixelShaderFromFile(ID3D11PixelShader** pPixelShader, TCHAR pSrcFile[], CHAR pFunctionName[], CHAR pProfile[])
{
	HRESULT hr = E_FAIL;

	//�Ō�̃t���O�̓f�o�b�O�p
	//UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_OPTIMIZATION_LEVEL0;
	//ID3D10Blob* pBlob = NULL;
	//ID3D10Blob* pErrorBlob = NULL;

	//Direct3D 9 ���[�h�Ŏ��s���̏ꍇ�̓o�[�W�����̕ύX�̕K�v������
	//�@������͊�������
	//�@���e�Ƃ��Ă͎擾����m_FeatureLevel��D3D_FEATURE_LEVEL_?_?�𔻒肵Profile�����ւ���
	//CHAR Profile[256];
	ID3DBlob* pBlobN = nullptr;
	
	//CHAR* Profile = "vs_5_0";
	//�s��͗�D��Őݒ�A�Â��`���̋L�q�͋����Ȃ�
	UINT Flag1 = D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_ENABLE_STRICTNESS;
	//�œK�����x���̐ݒ�͍��͍s��Ȃ�
	// Flat1 |= D3D10_SHADER_OPTIMIZATION_LEVEL0;�Ȃ�


	//�t�@�C�������ƂɃG�t�F�N�g���R���p�C��
	hr = D3DCompileFromFile(
		pSrcFile,       //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C����
		nullptr,        //�V�F�[�_�[���Ń}�N�����g���ꍇ�A���̃}�N����`�̔z��ւ̃|�C���^
		nullptr,        //�V�F�[�_�[����#include���g���ꍇ�A�C���^�[�t�F�C�X�ւ̃|�C���^
		pFunctionName,  //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
		pProfile,        //�V�F�[�_�[���f�����w��
		Flag1,          //�V�F�[�_�[�ɑ΂���R���p�C���I�v�V����
		0,              //�G�t�F�N�g�ɑ΂���R���p�C���I�v�V����
		&pBlobN,        //�R���p�C���ς݃V�F�[�_�[�ւ̃|�C���^
		nullptr         //�R���p�C�����̃G���[�ւ̃|�C���^
	);
	//hr = D3DX11CompileFromFile(
	//	pSrcFile,        //�V�F�[�_�[�R�[�h���i�[����Ă���t�@�C����
	//	NULL,            //�V�F�[�_�[���Ń}�N�����g���ꍇ�A���̃}�N����`�̔z��ւ̃|�C���^
	//	NULL,            //�V�F�[�_�[����#include���g���ꍇ�A�C���^�[�t�F�C�X�ւ̃|�C���^
	//	pFunctionName,   //�V�F�[�_�[�̎��s���J�n�����G���g���|�C���g�֐���
	//	pProfile,         //�V�F�[�_�[���f�����w��
	//	Flag1,           //�V�F�[�_�[�R���p�C���t���O
	//	0,               //�G�t�F�N�g�R���p�C���t���O
	//	NULL,            //�X���b�h�|���v�C���^�[�t�F�C�X�ւ̃|�C���^�[
	//	&pBlob,          //�R���p�C���ς݃V�F�[�_�[�ւ̃|�C���^
	//	&pErrorBlob,     //�R���p�C�����̃G���[�ւ̃|�C���^
	//	NULL);           //�߂�l�ւ̃|�C���^(NULL�ł���)
	if (hr == E_FAIL)
	{
		OutputDebugString(_T("Error: Pixel Shader Compile failed\n"));
		//if (pErrorBlob)
		//{
		//	OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		//}
		goto EXIT;
	}


	//�R���p�C���ς݃V�F�[�_�[����s�N�Z���V�F�[�_�[�I�u�W�F�N�g���쐬
	hr = m_D3DDevice->CreatePixelShader(
		pBlobN->GetBufferPointer(), //�R���p�C���ς݃V�F�[�_�[�ւ̃|�C���^
		pBlobN->GetBufferSize(),    //�R���p�C���ς݃V�F�[�_�[�ւ̃T�C�Y
		NULL,                      //�Ƃ肠����NULL�ł���
		pPixelShader              //ID3D11VertexShader�C���^�[�t�F�C�X�ւ̃|�C���^
		);
	if (hr == E_FAIL)
	{
		OutputDebugString(_T("Error: Pixel Shader Object creation failed\n"));
		goto EXIT;
	}

	hr = S_OK;
	OutputDebugString(_T("Pixel Shader Creation succeeded!\n"));


	//�I������
EXIT:
	SAFE_RELEASE(pBlobN);
	//SAFE_RELEASE(pBlob);
	//SAFE_RELEASE(pErrorBlob);

	return hr;
}