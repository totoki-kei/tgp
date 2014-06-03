#pragma once

#include <functional>
#include <typeinfo>
#include <cassert>

using std::function;

template < typename TRet = int, typename TParam = void*, typename TRuntimeParam = void*>
class Task {
public:
	class Container;
	friend class Container;


	typedef TRet ret_type;
	typedef TParam param_type;
	typedef TRuntimeParam rtparam_type;
	typedef Task<TRet, TParam> task_type;
	typedef Container container_type;
	typedef function < ret_type(task_type&, param_type, rtparam_type) > act_fn;

private:
	act_fn action;
	param_type param;

	task_type *next;
	task_type *prev;

	container_type *container;

	Task(const task_type&) {}

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

	Task() : Task([](task_type&, param_type, rtparam_type){ return (ret_type)0; }, (param_type)0){}
	
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

		const task_type* Root() { return &root; }

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
