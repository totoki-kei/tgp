#pragma once

#include <functional>
#include <typeinfo>
#include <cassert>

using std::function;

template < typename TRet = int, typename TParam = void*>
class Task {

public:
	class Container;

private:

	typedef TRet ret_t;
	typedef TParam param_t;
	typedef Task<TRet, TParam> task_t;
	typedef Container container_t;
	typedef function < ret_t (param_t) > act_f;

	act_f action;
	param_t param;

	task_t *next;
	task_t *prev;

	container_t *container;

	Task(const task_t&) {}

	static int s_id;
	int id;
	static int GetTypeHead() {
		const int h = (int)typeid(task_t).hash_code();
		const int s = (int)sizeof(int) * 8 / 2;
		const int m = (1 << s) - 1;
		return (h & (m << s)) ^ ((h & m) << s);
	};



public:
	
	template<class TAction>
	Task(TAction act, TParam param)
	{
		SetAction(act, param);
		this->next = this->prev = nullptr;

		id = GetTypeHead() ^ s_id++;
		s_id &= ( 1 << (sizeof(int) * 8 / 2) ) - 1;

	}
	
	template<class TAction>
	void SetAction(TAction act, TParam param)
	{
		this->action = act;
		this->param = param;
	}

	int ID() const { return id; }
	
	void InsertPrev(task_t* task) {
		task->prev = this->prev;
		task->next = this;
		this->prev = task->prev->next = task;
	}

	void InsertNext(task_t* task) {
		task->next = this->next;
		task->prev = this;
		this->next = task->next->prev = task;
	}

	ret_t Invoke() {
		return action(param);
	}


public:
	class Container {
		// 円形リンクリストの起点
		task_t root;

	public:

		Container() : root([](void*){ return (ret_t)0; }, (param_t)0){
			root.next = &root;
			root.prev = &root;
			root.id = GetTypeHead();
		}

		task_t& Root() { return root; }

		void InsertHead(task_t* task) {
			assert(!task->next && !task->prev);
			InsertAfter(task, &root);
		}

		void InsertTail(task_t* task) {
			assert(!task->next && !task->prev);
			InsertBefore(task, &root);
		}

		void InsertAfter(task_t* task, task_t* p) {
			assert(!task->next && !task->prev);
			p->InsertNext(task);
		}

		void InsertBefore(task_t* task, task_t* p) {
			assert(!task->next && !task->prev);
			p->InsertPrev(task);
		}

		void Remove(task_t* task) {
			task->next->prev = task->prev;
			task->prev->next = task->next;
			task->prev = task->next = nullptr;
		}

		void Invoke() {
			for( task_t* t = root.next; t != &root; t = t->next){
				task_t& tr = *t;
				tr.Invoke();
			}
		}
	};

};

template <class T1, class T2>
int Task<T1, T2>::s_id;
