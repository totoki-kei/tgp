#pragma once

#if 1

#include <memory>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <random>
#include <limits>

//#include <mutex>

#include <CSSkipList.h>


enum TaskResult {
	TR_KEEP, TR_DELETE,
};

template <typename OrderT>
class TaskList {
public:
	struct Key;
	struct Fn;
	struct Rnd;

	typedef TaskList<OrderT> ThisType;
	//typedef std::multiset<Fn> FnSet;
	typedef CS::MultiSkipList<Fn, std::less<Fn>, Rnd> FnSet;
	typedef typename FnSet::iterator FnPtr;
	typedef OrderT Order;
	//typedef std::recursive_mutex Mutex;
	//typedef std::mutex SingleMutex;

	typedef std::function<TaskResult(void)> Func;

	//struct KeyLock {
	//	std::unique_lock<Mutex> lock, gateLock;

	//	KeyLock() : lock{}, gateLock{} {}
	//	KeyLock(const KeyLock&) = delete;
	//	KeyLock(Mutex& mtx, std::unique_lock<Mutex>&& gate) : lock{ mtx }, gateLock{ std::move( gate )} {}

	//	operator bool() {
	//		return (!!gateLock) && (!!lock);
	//	}
	//};

	class Key {
		friend struct Fn;

		uint32_t uid;
		bool destracted;
		//Mutex mtx;
		std::vector<Fn*> fnset;

		//static Mutex mtxGate;
		static uint32_t g_uid;

	public:
		void TaskKeyCallback_Register(Fn* fn) {
			//if (auto lock = TaskKey_Lock()) {
				fnset.push_back(fn);
			//}
		}

		void TaskKeyCallback_Move(Fn* from, Fn* to) {
			//if (auto lock = TaskKey_Lock()) {
				auto iter = std::find(fnset.begin(), fnset.end(), from);
				if (iter != fnset.end()) *iter = to;
			//}
		}

		void TaskKeyCallback_Unregister(Fn* fn) {
			//if (auto lock = TaskKey_Lock()) {
				auto iter = std::find(fnset.begin(), fnset.end(), fn);
				if (iter != fnset.end()) fnset.erase(iter);
			//}
		}

		void TaskKey_Clear() {
			//if (auto lock = TaskKey_Lock()) {
				for (auto f : fnset) {
					f->key = nullptr;
				}
				fnset.clear();
			//}
			uid = g_uid++;
		}

		//KeyLock TaskKey_Lock() {
		//	std::unique_lock<Mutex> gateLock(mtxGate);

		//	if (destracted) {
		//		return KeyLock();
		//	}
		//	else {
		//		return KeyLock(mtx, std::move(gateLock));
		//	}

		//}

		Key() {
			uid = g_uid++;
			fnset.clear();
			destracted = false;
		}

		virtual ~Key() {
			//std::lock_guard<Mutex> gateLock(mtxGate);

			//auto lock = TaskKey_Lock();
			for (auto f : fnset) {
				if (f) f->key = nullptr;
			}
			fnset.clear();
			destracted = true;

		}

	};

	struct Fn {
		OrderT order;
		Key* key;
		uint32_t key_uid;
		Func f;


		Fn() {
			order = (OrderT)0;
			key = nullptr;
			key_uid = 0xFFFFFFFF;
			f = nullptr;
		}

		Fn(OrderT ord, Key* o, Func fn) : order{ ord }, key{ o }, key_uid{ 0xFFFFFFFF }, f{ fn } {
			if (key) {
				this->key_uid = key->uid;
				key->TaskKeyCallback_Register(this);
			}
		}

		Fn(const Fn& fn) : Fn(std::move(const_cast<Fn&>(fn))) {};

		Fn(Fn&& fn) :order{ fn.order }, key{ fn.key }, key_uid{ fn.key_uid }, f{ fn.f } {
			if (key) key->TaskKeyCallback_Move(&fn, this);
			fn.order = (OrderT)0;
			fn.key = nullptr;
			fn.key_uid = 0xFFFFFFFF;
			fn.f = nullptr;
		}

		~Fn() {
			if (key) {
				key->TaskKeyCallback_Unregister(this);
				key_uid = 0xFFFFFFFF;
			}
		}

		operator bool() {
			return key != nullptr && key->uid == key_uid;
		}

		bool operator<(const Fn& right) const {
			return this->order < right.order;
		}

		TaskResult Invoke (void) const {
			
			if (key != nullptr && key->uid == key_uid && f) {
				//if(auto lock = key->TaskKey_Lock()) 
					return f();
			}
			return TR_DELETE;
		}
	};

	struct Rnd {
		std::mt19937 rnd;

		Rnd() : rnd(GetTickCount()) {}

		unsigned int rand() {
			return (unsigned int)rnd();
		}

		double drand() {
			return rnd() / (double)std::numeric_limits<std::mt19937::result_type>::max();
		}
	};


private:
	FnSet tasks;
	FnSet tasksToAdd_1, tasksToAdd_2;
	FnSet *addlist, *newlist;
	//Mutex addlist_mtx, activelist_mtx;

public:
	TaskList() : tasks{} {
		addlist = &tasksToAdd_1;
		newlist = &tasksToAdd_2;
	}


	//const FnPtr Add(Key* key, Func fn, OrderT order) {
	void Add(Key* key, Func fn, OrderT order) {
		//std::lock_guard<Mutex> lock(addlist_mtx);
		auto ret = addlist->insert(Fn{ order, key, fn });
		//return ret;
	}

	template <typename T>
	//const FnPtr Add(T* key, TaskResult(T::*fn)(void), OrderT order) {
	void Add(T* key, TaskResult(T::*fn)(void), OrderT order) {
		//auto ret = tasks.insert(Fn{ order, key, [key, fn]() {
		//	return (key->*fn)();
		//} });
		//std::lock_guard<Mutex> lock(addlist_mtx);
		auto ret = addlist->insert(Fn{ order, key, std::bind(fn, key) });
		//return ret;
	}

	//void Remove(const Key* key) {
	//	{
	//		std::lock_guard<Mutex> lock(activelist_mtx);
	//		auto &t = tasks.begin();
	//		while (t != tasks.end()) {
	//			if (t->key == key) {
	//				t = tasks.erase(t);
	//				continue;
	//			}
	//			t++;
	//		}
	//	}
	//	{
	//		std::lock_guard<Mutex> lock(addlist_mtx);
	//		auto &t = addlist->begin();
	//		while (t != addlist->end()) {
	//			if (t->key == key) {
	//				t = addlist->erase(t);
	//				continue;
	//			}
	//			t++;
	//		}
	//	}
	//}

	//void Remove(const Key* key, const OrderT order) {
	//	auto finder = [&](const Fn& f) {return f.order == order && f.key == key; };

	//	{
	//		std::lock_guard<Mutex> lock(activelist_mtx);
	//		auto bgn = tasks.begin();
	//		auto iter = std::find_if(bgn, tasks.end(), finder);

	//		while (iter != tasks.end()) {
	//			bgn = tasks.erase(iter);
	//			iter = std::find_if(bgn, tasks.end(), finder);
	//		}
	//	}
	//	{
	//		std::lock_guard<Mutex> lock(addlist_mtx);
	//		auto bgn = addlist->begin();
	//		auto iter = std::find_if(bgn, addlist->end(), finder);

	//		while (iter != addlist->end()) {
	//			bgn = addlist->erase(iter);
	//			iter = std::find_if(bgn, addlist->end(), finder);
	//		}
	//	}
	//}

	void Remove(const FnPtr &fn) {
		tasks.erase(fn);
	}


	void Invoke() {

			
		{
			//std::lock_guard<Mutex> lock(addlist_mtx);
			std::swap(addlist, newlist);
		}

		{
			//std::lock_guard<Mutex> lock(activelist_mtx);

			for (auto t : *newlist) {
				tasks.insert(t);
			}
			newlist->clear();

			typename FnSet::iterator t = tasks.begin();
			while (t != tasks.end()) {
				switch (t->Invoke()) {
					case TR_DELETE:
						t = tasks.erase(t);
						continue;
				}
				t++;
			}
		}
	}

	size_t Count() {
		//std::lock_guard<Mutex> lock(activelist_mtx);
		return tasks.size();
	}
};

//template <typename OrderT>
//typename TaskList<OrderT>::Mutex TaskList<OrderT>::Key::mtxGate;
template <typename OrderT>
uint32_t TaskList<OrderT>::Key::g_uid;

#else

#include <functional>
#include <typeinfo>
#include <cassert>

using std::function;

template < typename TRet, typename TParam, typename TRuntimeParam>
class Task {
public:
	class Container;
	friend class Container;


	typedef TRet ret_type;
	typedef TParam param_type;
	typedef TRuntimeParam rtparam_type;
	typedef Task<TRet, TParam, TRuntimeParam> task_type;
	typedef Container container_type;
	typedef function < ret_type(task_type&, param_type, rtparam_type) > act_fn;

private:
	act_fn action;
	param_type param;

	task_type *next;
	task_type *prev;

	container_type *container;


	static int s_id;
	int id;

	bool toRemove;
	bool removeImmidiately;
public:
	
	template<class TAction>
	Task(TAction act, TParam param)
	{
		SetAction(act, param);
		this->next = this->prev = nullptr;

		// 連番を設定
		id = s_id++;
		toRemove = false;
	}

	Task(const task_type& t) : Task(t.action, t.param) { this->id = t.id; }

	Task() : Task([](task_type&, param_type, rtparam_type){ return (ret_type)0; }, (param_type)0){}
	
	~Task() {

	}

	template<class TAction>
	void SetAction(TAction act, TParam param)
	{
		this->action = act;
		this->param = param;
		this->toRemove = false;
	}

	int ID() const { return id; }
	
	void InsertPrev(task_type* task) {
		task->prev = this->prev;
		task->next = this;
		this->prev = task->prev->next = task;
	}

	void InsertNext(task_type* task) {
		task->next = this->next;
		task->prev = this;
		this->next = task->next->prev = task;
	}

	ret_type Invoke(rtparam_type ext) {
		return action(*this, param, ext);
	}

	void MarkToRemove(){ this->toRemove = true; this->removeImmidiately = false; }

	void RemoveImmidiately(){ this->toRemove = true; this->removeImmidiately = true; }

public:
	class Container {
		// 円形リンクリストの起点
		task_type root;

	public:

		Container()
			: root() {
			root.next = &root;
			root.prev = &root;
			root.id = -1;

		}

		task_type* Root() { return &root; }

		void InsertHead(task_type* task) {
			InsertAfter(task, &root);
		}

		void InsertTail(task_type* task) {
			InsertBefore(task, &root);
		}

		void InsertAfter(task_type* task, task_type* p) {
			p->InsertNext(task);
		}

		void InsertBefore(task_type* task, task_type* p) {
			p->InsertPrev(task);
		}

		void Remove(task_type* task) {
			task->next->prev = task->prev;
			task->prev->next = task->next;
			task->prev = task->next = nullptr;
		}

		void Invoke(rtparam_type ext) {
			task_type* last = nullptr;
			for( task_type* t = root.next; t != &root; t = t->next){
				if (last){
					Remove(last);
					last = nullptr;
				}

				task_type& tr = *t;
				tr.Invoke(ext);

				if (tr.toRemove && tr.removeImmidiately){
					last = t;
				}
			}
		}

		template <typename Fn>
		void ForEach(Fn f){
			task_type* last = nullptr;
			for (task_type* t = root.next; t != &root; t = t->next){
				if (last){
					Remove(last);
					last = nullptr;
				}

				task_type& tr = *t;
				f(ext);

				if (tr.toRemove && tr.removeImmidiately){
					last = t;
				}
			}
		}

		void Sweep(){
			for (task_type* t = root.next; t != &root; t = t->next){
				while (t != &root && t->toRemove){
					task_type* tn = t->next;
					Remove(t);
					t = tn;
				}
			}
		}

	};

};

template <class T1, class T2, class T3>
int Task<T1, T2, T3>::s_id;

#endif

