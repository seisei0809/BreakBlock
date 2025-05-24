#pragma once
#include "AllSetting.h"
#include "Object.h"

// シーン基底
class Scene {
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Scene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Scene();

	/// <summary>
	/// ObjectのStart関数を呼び出す
	/// </summary>
	void Start();

	/// <summary>
	/// ObjectのUpdate関数を呼び出す
	/// </summary>
	void Update();

protected:

	// Objectを追加する(templateはコンパイル時にインスタンス化されるから定義はヘッダに)  
	template <typename T> T* AddObject() {

		// Object基底以外のクラスならコンパイル時にエラーを出す
		static_assert(std::is_base_of<Object, T>::value, "Tクラスは必ずObjectを継承していること");

		// 指定された型のインスタンスを作成
		T* obj = new T();

		// objects に追加
		objects.push_back(obj);

		return obj;
	}

	// Objectポインタを格納
	VECTOR<Object*> objects;
};
