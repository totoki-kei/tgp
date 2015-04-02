#pragma once

class Scene {
protected:
	Scene *parent, *child;

	// シーンを更新する
	// 戻り値は、以下のうちいずれかとする
	// this        : 現在のシーンを継続、またはサブシーンへ遷移する
	// parent      : このシーンを終了する
	// nullptr     : このシーンを終了し、即座に親シーンを実行する
	// <new Scene> : このシーンを終了し、返したシーンに差し替える
	//
	// <MEMO>
	//   サブシーンへ遷移する場合は、UpdateScene内で
	//   childへ新しいシーンを設定する。
	virtual Scene* UpdateScene();

	virtual void DrawScene();

public:
	Scene();
	Scene(Scene* parent);
	Scene(const Scene&) = delete;
	Scene(Scene&&) = delete;

	virtual ~Scene();

	Scene* Update();
	void Draw();

	Scene* Ancest();
	Scene* Descendant();
};

