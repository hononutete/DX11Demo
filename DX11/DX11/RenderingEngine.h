#pragma once
#include <vector>
#include "MeshInstance.h"

class RenderingEngine
{
public:
	RenderingEngine();
	~RenderingEngine();

	//メッシュインスタンスの連結リスト
	std::vector<MeshInstance*> m_pMeshInstanceList;

	//メッシュインスタンスを作成
	void CreateMeshInstance();

	//ゲームワールドを対象に、実際のレンダリングを行う
	void RenderFrameAndSwapBuffer();

	//バッチ処理を行う

	//カリングシステム

	//視錐台カリング
	void FrustumCulling();

	//オクルージョンカリング（遮蔽カリング）
	void OcclusionCulling();

	//ポータルカリング
	void PortalCulling();
};

