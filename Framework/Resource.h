#pragma once

#define RSC_DBG

#include <memory>
#include <vector>
#include <set>
#include <functional>

#include "Debug.h"
#include "WinAPI.h"

#ifdef RSC_DBG
#define RSC_DBG_OUT LOG_DBG
#else
#define RSC_DBG_OUT __noop
#endif


class Resource{
	/*
	Resourceのサブクラスは、内部で独自に作成したリソース(ハンドルやポインタ)を
	AddResourceして自身に登録しておくことで、削除時に自動的に削除される。
	ただし、実際に削除が行われるのはResourceのデストラクタが呼ばれた時なので
	サブクラス自身のデストラクタ完了までに削除済みである必要があるリソースは
	以下のどちらかで対応すること。
	 (1) デストラクタで 「if(!isDisposed()) Dispose();」を行う
	 (2) AddResourceせずに、独自に管理してデストラクタで「通常通り」開放する

	なお、削除時の動作順序は以下のようになる：
	1. サブクラスのデストラクタが呼ばれる(必要に応じてリソースの解放(Dispose呼び出し含む)を行う)
	2. Resourceのデストラクタが呼ばれる
	2.1. isDisposedを呼び、Disposeが呼ばれたか確認する(オーバーライドしていればそれが呼ばれる)
	2.2. もしisDisposedがfalseであればAddResourceされているリソースを全てDisposeする
	3. デストラクタが完了し削除される

	【注意】
	このクラスは、親リソースが解放されたときに子リソースの解放漏れを防ぐ目的の物であり
	使われなくなったリソースを自動解放するための物ではない。
	むしろAddResourceされたインスタンスは、親リソースにハンドルされている限り
	実際のdeleteとメモリ解放は行われない。
	手動で開放したい場合は、以下の手順で親リソースから取り除くこと
	(1) 親リソース.FindResource(子リソースのID) でweak_ptr取得
	(2) weak_ptrをロックしてshared_ptr取得
	(3) 取得したshared_ptrを使用して親リソースのRemoveResourceを呼ぶ。
	*/

protected:
	typedef std::set< std::shared_ptr<Resource> > children_type;
	// 登録されているリソース
	children_type* children;

	// 自身のリソースID
	unsigned int ResourceId;

public:
	// 自動削除されるリソースを登録する
	// 登録されたリソースのポインタをそのまま返す
	std::shared_ptr<Resource> AddResource(std::shared_ptr<Resource>);

	// 登録されているリソースから削除する
	bool RemoveResource(std::shared_ptr<Resource>, bool all = false, bool recursive = false);
	bool RemoveResource(Resource*, bool all = false, bool recursive = false);

	// このリソースのリソースIDを得る
	unsigned int GetResourceID() const;

	// 登録されているリソースから指定のリソースIDを持ったリソースを検索する
	std::weak_ptr<Resource> FindResource(unsigned int id, bool recursive = false);
	
	// コンストラクタ、リソースIDを自動的に割り当てる
	Resource(void);
	// コンストラクタ、指定のリソースIDを使用する
	Resource(unsigned int);

	// リソース解放済みの場合はtrueを返す。
	// 既定の実相では、登録されているリソースが存在しない、またはすべて解放済みである場合trueとなる
	// [オーバーライド時の規約] Dispose() が呼ばれた後は、かならずtrueを返すようにすること
	virtual bool isDisposed();

	// リソースを開放する
	// 既定の実装では、登録されているリソースをすべて解放(Dispose)し、登録解除する
	// [オーバーライド時の規約] 必ずResource::Dispose()を1回以上呼ぶこと
	virtual void Dispose();

	// 登録されているリソースをすべて解放し、登録解除する
	void DisposeResources();

	// デストラクタ
	// 既定の実装では、もしisDisposed()がfalseの時にはDisposeを呼ぶ
	virtual ~Resource();

private:

	static unsigned int nextid;

};

