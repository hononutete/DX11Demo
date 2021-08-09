#pragma once
#include "Mesh.h"
//#include "Transform.h"
#include "Material.h"
//#include "Component.h"
#include <vector>

//前方宣言
//Componentクラスを使えるようにするため
class Component;
class Renderer;
class Transform;

class GameObject
{
public:
	//ゲームオブジェクトの固有ID
	int uid = 0;

	//可視状態にあるかどうかのフラグ
	bool isActive = true;

	//持っているコンポーネント属性へのポインタリスト
	std::vector<Component*> m_componentsList;

	//トランスフォーム情報
	Transform* m_pTransform;

	//メッシュ情報　
	//（今はテクスチャはメッシュが持っている、本来はマテリアルが所持するべきである）
	//MeshGroup* m_pMeshGroup;

	//マテリアル情報（シェーダー）
	//Material* m_pMaterial;

	//レンダラー
	Renderer* m_pRenderer;

	//初期化
	HRESULT Initialize();

	//メッシュをセットする
	//HRESULT SetMesh(MeshGroup* pMeshGroup);

	//ゲームオブジェクトをレンダリングする
	HRESULT Render();

	//フレームごとの処理をする
	void Update();

	//マテリアルを、使用するシェーダーの名前で作成する
	//HRESULT CreateMaterial(TCHAR shaderName);

	//ゲームオブジェクトにコンポーネント属性をセット
	HRESULT SetComponent(Component* pComponent);

	//ゲームオブジェクトからコンポーネント属性を削除
	HRESULT RemoveComponent(Component* pComponent);

	//コンストラクタ
	GameObject();

	//デストラクタ
	~GameObject();

private:
	void Invalidiate();

	bool IsRenderable();

	void CreateTransformComponent();

};

