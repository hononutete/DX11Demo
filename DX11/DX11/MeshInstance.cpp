#include "stdafx.h"
#include "MeshInstance.h"
//#include "Scene.h"
#include "Camera.h"
#include "ShaderCompiler.h"
#include "GameWorld.h"

MeshInstance::MeshInstance()
{
}


MeshInstance::~MeshInstance()
{
}

//このメッシュインスタンスの描画を実行する
HRESULT MeshInstance::Render() {
	HRESULT hr = E_FAIL;
	extern D3D11User* g_pD3D11User;
	//extern Scene* g_pScene;
	UINT DrawMeshIndex = 0;
	extern GameWorld* g_pGameWorld;
	////メッシュがなかった場合レンダリングは実行されない
	//if (!IsRenderable()) {
	//	return S_OK;
	//}

	//*** モデルのアニメーションを更新 ***
	if (m_pMeshGroup->Meshes[0].HasAnimation())
		m_pMeshGroup->Meshes[0].UpdateAnimationTime();


	//********* 頂点関連のデータをデバイスコンテキストにセット　**************


	//頂点バッファを設定  --- 頂点バッファはそれぞれの頂点データ（座標など）の配列。描画するためには頂点バッファをデバイスに教える
	UINT stride = sizeof(Mesh::GVERTEX);
	UINT offset = 0;
	g_pD3D11User->m_D3DDeviceContext->IASetVertexBuffers(0, 1, &m_pMeshGroup->Meshes[0].VertexBuffer, &stride, &offset);

	//頂点インデックスバッファを設定 --- 頂点インデックスバッファはポリゴンを作るための頂点の結び順に頂点番号が格納されたバッファ	
	g_pD3D11User->m_D3DDeviceContext->IASetIndexBuffer(m_pMeshGroup->Meshes[0].IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//頂点レイアウト設定
	g_pD3D11User->m_D3DDeviceContext->IASetInputLayout(m_pMaterial->m_pLayout);

	//プリミティブタイプおよびデータの順序に関する情報を設定 ---- 描画するプリミティブの形状を指定（ジオメトリシェーダーの場合シェーダー側で再定義されるので意味ないらしい）
	//　*** フラットシェーディングの場合は、三角リストを指定
	g_pD3D11User->m_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//　*** グーローシェーディングを使う場合は、三角ストリップを指定
	//g_pD3D11User->m_D3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//********** 行列設定 ****************************


	//フレーム移動

	//ワールド、ビュー、プロジェクション変換行列, WVP最終変換行列
	XMMATRIX matWorld, matView, matProj, matWVP, matNormal;
	matWorld = XMMatrixIdentity();
	matView = XMMatrixIdentity();
	matProj = XMMatrixIdentity();
	matWVP = XMMatrixIdentity();
	matNormal = XMMatrixIdentity();

	//射影変換行列 , fovY, aspect, Zn, Zf
	{
		matProj = g_pGameWorld->m_pCameraComponent->LoadProjectionMatrix();
	}

	//ビュー変換行列
	{
		matView = g_pGameWorld->m_pCameraComponent->LoadViewMatrix();
	}

	//ワールド変換行列
	{
		XMMATRIX matIdentity, matScale, matTrans, matRot, matRotX, matRotY, matRotZ;
		matScale = XMMatrixIdentity();
		matTrans = XMMatrixIdentity();
		matRot = XMMatrixIdentity();
		matRotX = XMMatrixIdentity();
		matRotY = XMMatrixIdentity();
		matRotZ = XMMatrixIdentity();

		//単位行列
		matIdentity = XMMatrixIdentity();

		//スケール変換
		matScale = XMMatrixScaling(m_pTransform->scale.x, m_pTransform->scale.y, m_pTransform->scale.z);

		//平行移動
		matTrans = XMMatrixTranslation(m_pTransform->localPosition.x, m_pTransform->localPosition.y, m_pTransform->localPosition.z);

		//回転
		matRotX = XMMatrixRotationX(XMConvertToRadians(m_pTransform->eulerAngles.x));
		matRotY = XMMatrixRotationY(XMConvertToRadians(m_pTransform->eulerAngles.y));
		matRotZ = XMMatrixRotationZ(XMConvertToRadians(m_pTransform->eulerAngles.z));
		//matRotX = XMMatrixRotationX(XMConvertToRadians(m_pTransform->rotX));
		//matRotY = XMMatrixRotationY(XMConvertToRadians(m_pTransform->rotY));
		//matRotZ = XMMatrixRotationZ(XMConvertToRadians(m_pTransform->rotZ));
		matRot = matRotX * matRotY * matRotZ;

		matWorld = matIdentity * matScale * matRot * matTrans;

		//計算を行う場合、平行移動に関しては無視、回転行列に関してはそのまま使用できる、スケール行列に関してはxが3倍なら1/3倍のように逆数を使う
		//これらを考慮すると以下のように求められる
		matNormal = matIdentity * matRot;
	}
	//ワールド・ビュー・射影変換行列を作成
	matWVP = matWorld * matView * matProj;

	//D3DXMATRIXは行優先の行列、行優先というのは行単位でひとつの定数レジスタに格納するということ
	//しかしシェーダー内では列優先のレイアウトで配置しているから転置して列と行を入れ替える必要がある
	//シェーダーでも扱う行列をすべて入れ替える
	matNormal = XMMatrixTranspose(matNormal);//法線変換行列
	matWVP = XMMatrixTranspose(matWVP);//WVP行列

									   //**********　定数バッファ ****************************

									   //行列を定数バッファに設定、方法が２つある
									   //�@直接コピーする方法(CopyMemory関数で直接使う、D3DDeviceContext->MapとUnMapを使用してメモリアクセスを制限）

									   //�AUpdateSubresourceを使う方法
									   //常にこれを使おうとするとこのエラーがでる→Can only invoke UpdateSubresource when the destination Resource was created with D3D11_USAGE_DEFAULT and is not a multisampled Resource.
									   //定数バッファ構造体に値を設定
	//extern Scene* g_pScene;

	Shader::CB_LAMBERT cb;
	cb.matWVP = matWVP;
	cb.matNormal = matNormal;
	//TODO: ライトベクトルは正規化してから設定するがこれは、すべてのゲームオブジェクトですることではない
	XMVECTOR mLight = XMVectorZero();
	mLight = XMLoadFloat4(&(g_pGameWorld->m_vecLight));
	mLight = XMVector4Normalize(mLight);
	XMFLOAT4 light;
	XMStoreFloat4(&light, mLight);
	cb.vecLight = light;
	cb.ambient = g_pGameWorld->m_Ambient;

	//定数バッファ構造体の情報をVRAM上に転送
	g_pD3D11User->m_D3DDeviceContext->UpdateSubresource(m_pMaterial->m_pConstantBuffersLambert, 0, NULL, &cb, 0, 0);


	// ************ シェーダー関連 ************************

	//頂点シェーダー
	{
		//頂点シェーダーをデバイスに設定 (頂点シェーダーへのポインタ, シェーダーでインターフェイスを使用する場合必要, シェーダーのインターフェイスのクラスインスタンス数)
		g_pD3D11User->m_D3DDeviceContext->VSSetShader(m_pMaterial->m_pVertexShader, NULL, 0);

		//頂点シェーダーに定数バッファを設定 (デバイスの配列の中で定数バッファの設定を開始する位置, 設定するバッファの数, デバイスに設定する定数バッファ)
		{

			//ランバート反射の定数バッファ構造体の定数バッファを頂点シェーダーにセット
			g_pD3D11User->m_D3DDeviceContext->VSSetConstantBuffers(0, 1, &m_pMaterial->m_pConstantBuffersLambert);

		}
	}

	//ピクセルシェーダー
	{
		//ピクセルシェーダーをデバイスに設定
		g_pD3D11User->m_D3DDeviceContext->PSSetShader(m_pMaterial->m_pPixelShader, NULL, 0);

		//ピクセルシェーダーに定数バッファを設定 (ピクセル シェーダーのパイプライン ステージで使用される定数バッファーを設定)

		//フォンシェーディングでは使用する
		//g_pD3D11User->m_D3DDeviceContext->PSSetConstantBuffers(1, 1, &g_pConstantBuffers[1]);

		//シェーダーにサンプラーステートを設定
		g_pD3D11User->m_D3DDeviceContext->PSSetSamplers(0, 1, &m_pMaterial->m_pSamplerState);

		//シェーダーにシェーダーリソースビューを設定（シェーダー用にロードしたテクスチャ）

		//これは描画されるメッシュで使用されるテクスチャを設定する
		ID3D11ShaderResourceView* pSRV = NULL;

		//メッシュで使用されているテクスチャを取得
		//TODO: もし複数のテクスチャを使っている場合全部
		m_pMeshGroup->Meshes[DrawMeshIndex].GetTexture(_T("DiffuseColor"), &pSRV);

		//ピクセルシェーダーにテクスチャリソースビューを設定
		g_pD3D11User->m_D3DDeviceContext->PSSetShaderResources(0, 1, &pSRV);

		//デバッグ用
		//g_pD3D11User->m_D3DDeviceContext->PSSetShaderResources(0, 1, &g_pSRView);
	}

	//ハルシェーダーを無効にする
	g_pD3D11User->m_D3DDeviceContext->HSSetShader(NULL, NULL, 0);

	//ドメインシェーダーを無効にする
	g_pD3D11User->m_D3DDeviceContext->DSSetShader(NULL, NULL, 0);

	//ジオメトリシェーダーを無効にする
	g_pD3D11User->m_D3DDeviceContext->GSSetShader(NULL, NULL, 0);

	//コンピュートシェーダーを無効にする
	g_pD3D11User->m_D3DDeviceContext->CSSetShader(NULL, NULL, 0);


	//両面描画に変更する処理


	//ラスタライザの設定を取得
	ID3D11RasterizerState* pRasterState = NULL;
	g_pD3D11User->m_D3DDeviceContext->RSGetState(&pRasterState);
	//※注意　RSGetStateはデフォルトでは未設定でNULLが設定されている。RSGetStateでNULLが返ってきてしまうので、よって初期化段階で初期値を設定しておくこと

	D3D11_RASTERIZER_DESC RasterDesc;
	//::ZeroMemory(&RasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	pRasterState->GetDesc(&RasterDesc);

	//取得したラスタライザの設定を両面描画に変更して再設定
	//RasterDesc.CullMode = D3D11_CULL_NONE;

	g_pD3D11User->m_D3DDevice->CreateRasterizerState(&RasterDesc, &pRasterState);
	g_pD3D11User->m_D3DDeviceContext->RSSetState(pRasterState);

	SAFE_RELEASE(pRasterState);


	// ************ 描画　************************

	//インデックスバッファを使用した描画
	g_pD3D11User->m_D3DDeviceContext->DrawIndexed(m_pMeshGroup->Meshes[DrawMeshIndex].IndexCount, 0, 0);


	//終了処理
EXIT:
	//SAFE_RELEASE(constantBuffers);

	return hr;


}