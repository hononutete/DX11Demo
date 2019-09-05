#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"

class MeshInstance
{
public:
	MeshInstance();
	~MeshInstance();

	//トランスフォーム情報
	Transform* m_pTransform;

	//メッシュ情報　
	//（今はテクスチャはメッシュが持っている、本来はマテリアルが所持するべきである）
	MeshGroup* m_pMeshGroup;

	//マテリアル情報（シェーダー）
	Material* m_pMaterial;

	//このメッシュインスタンスの描画を実行する
	//この関数の中でDirectXを呼び出してレンダリングを実行してしまうとバッチ処理ができない
	//バッチ処理は一緒にレンダリングできるものを一度のレンダリングで実行するもの、つまり他のメッシュインスタンスの情報が必要になる
	//つまりレンダリングは高レベルなところで行う
	HRESULT Render();

};

