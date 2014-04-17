#pragma once

#include <functional>
#include <typeinfo>
#include <cassert>

#include "BlockArray.h"

using std::function;

template < typename TRet = int, typename TParam = void*, typename TExtraParam = void*>
class Task {

public:
	class Container;

	typedef TRet ret_type;
	typedef TParam param_type;
	typedef TExtraParam exparam_type;
	typedef Task<TRet, TParam> task_type;
	typedef Container container_type;
	typedef function < ret_type (param_type, exparam_type) > act_fn;

private:
	act_fn action;
	param_type param;

	task_type *next;
	task_type *prev;

	container_type *container;

	Task(const task_type&) {}

	static int s_id;
	int id;


public:
	
	template<class TAction>
	Task(TAction act, TParam param)
	{
		SetAction(act, param);
		this->next = this->prev = nullptr;

		// 連番を設定
		id = s_id++;

	}

	Task() : Task([](param_type, exparam_type){ return (ret_type)0; }, (param_type)0){}
	
	template<class TAction>
	void SetAction(TAction act, TParam param)
	{
		this->action = act;
		this->param = param;
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

	ret_type Invoke(exparam_type ext) {
		return action(param, ext);
	}


public:
	class Container {
		// 円形リンクリストの起点
		task_type root;

	public:

		Container()
			: root([](param_type, exparam_type){ return (ret_type)0; }, (param_type)0) {
			root.next = &root;
			root.prev = &root;
			root.id = -1;

		}

		task_type* Root() { return $root; }

		void InsertHead(task_type* task) {
			assert(!task->next && !task->prev);
			InsertAfter(task, &root);
		}

		void InsertTail(task_type* task) {
			assert(!task->next && !task->prev);
			InsertBefore(task, &root);
		}

		void InsertAfter(task_type* task, task_type* p) {
			assert(!task->next && !task->prev);
			p->InsertNext(task);
		}

		void InsertBefore(task_type* task, task_type* p) {
			assert(!task->next && !task->prev);
			p->InsertPrev(task);
		}

		void Remove(task_type* task) {
			task->next->prev = task->prev;
			task->prev->next = task->next;
			task->prev = task->next = nullptr;
		}

		void Invoke(exparam_type ext) {
			for( task_type* t = root.next; t != &root; t = t->next){
				task_type& tr = *t;
				tr.Invoke(ext);
			}
		}
	};

};

template <class T1, class T2, class T3>
int Task<T1, T2, T3>::s_id;
