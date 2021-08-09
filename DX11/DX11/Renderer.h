#pragma once
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "Component.h"

class Renderer : public Component
{
public:

	//トランスフォーム情報
	//Transform* m_pTransform;

	//メッシュ情報　
	//（今はテクスチャはメッシュが持っている、本来はマテリアルが所持するべきである）
	MeshGroup* m_pMeshGroup;

	//マテリアル情報（シェーダー）
	Material* m_pMaterial;

	Renderer();
	~Renderer();

	HRESULT SetMaterial(Material* pMaterial);

	//バッチングのため、この段階ではレンダリングキューに登録だけを行う
	//ゲームオブジェクトごとにそのUpdateの中からレンダリングを実行するのでは効率が悪い。
	//この関数では、レンダリングシステムにリクエストを送って、使用するメッシュのインスタンスを割り当ててもらう（可視かどうかにかかわらず）
	//レンダリングシステムは、メッシュインスタンスのの集合を内部的に維持・管理していて、ランタイムでの処理を効率化するためにバッチ処理を行う
	//この関数でやることは、レンダリングシステムにリクエストを送るか、メッシュインスタンスのプロパティを変更するだけ
	//レンダリング手法（Fowardレンダリングか、Defferedレンダリングかにもよって、ここはかわってくるだろう）
	HRESULT Render();

	//新しくメッシュインスタンスの割り当てリクエストを出す

	bool IsRenderable();


	
};

