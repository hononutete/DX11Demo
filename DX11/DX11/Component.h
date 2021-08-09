#pragma once
//#include "GameObject.h"

//前方宣言
//GameObjectクラスを使えるようにするため
class GameObject;

class Component
{
public:
	//このコンポーネントを持っているGameObjectへのポインタ
	GameObject* m_pGameObject;
	
	//デフォルトコンストラクタ
	Component();

	//コピーコンストラクタ
	Component(Component& rother);

	//デストラクタ
	virtual ~Component();

	//コンポーネントのフレーム処理
	void Update();
};

