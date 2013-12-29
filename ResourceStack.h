#pragma once

#if 0

#include <stack>
#include <functional>

class ResourceStack
{
public:
	class Handle;
private:
	class Item {
	public:
		virtual void Delete() = 0;
		virtual ~Item() { } ;
		virtual bool isReleased() = 0;
	};
	typedef std::stack<Item *> stack_t;
	stack_t stack;


	template <class TItem>
	class ItemImpl : public Item {
		TItem ptr;
		std::function<void (TItem &)> finalizer;
		bool released;

	public:
		template <typename TFinalizer>
		ItemImpl(TItem& item, TFinalizer fn) : ptr(item){
			this->finalizer = fn;
			this->released = false;
		}

		~ItemImpl() {
			Delete();
		}

		void Delete() {
			if(this->released) return;
			this->finalizer(ptr);
			this->released = true;
		}

		bool isReleased() { return this->released; }
	};

public:

	/*
	TODO:
		リソース削除関数
		チェックポイントの作成、チェックポイントまで解放
	*/

	class Handle {
	private:
		Item* item;
	public:
		Handle(Item* i) : item(i) {}

		void Release() {
			item->Delete();
		}

		bool isReleased() {
			return item->isReleased();
		}
	};

	ResourceStack(void) : stack() {	}

	void DeleteAll() {
		while ( stack.size() > 0){
			auto item = stack.top();
			delete item;
			stack.pop();
		}
	}

	~ResourceStack(void) {
		DeleteAll();
	}

	template <class TData>
	Handle Push(TData* ptr){
		auto item = new ItemImpl<TData*>(ptr, [](TData* &p){ delete p; });
		stack.push(item);
		return Handle(item);
	}

	template <class TData, class TFinalizer>
	Handle Push(TData &res, TFinalizer finalizer){
		auto item = new ItemImpl<TData>(res, finalizer);
		stack.push(item);
		return Handle(item);
	}

};

#elif 0


#include <stack>
#include <functional>
#include <memory>

class ResourceStack
{
public:
	class Handle;
private:
	class Item {
	public:
		virtual void Delete() = 0;
		virtual ~Item() { } ;
		virtual bool isReleased() = 0;
	};
	typedef std::stack<std::shared_ptr<Item> > stack_t;
	stack_t stack;


	template <class TItem>
	class ItemImpl : public Item {
		TItem ptr;
		std::function<void (TItem &)> finalizer;
		bool released;

	public:
		template <typename TFinalizer>
		ItemImpl(TItem& item, TFinalizer fn) : ptr(item){
			this->finalizer = fn;
			this->released = false;
		}

		~ItemImpl() {
			Delete();
		}

		void Delete() {
			if(this->released) return;
			this->finalizer(ptr);
			this->released = true;
		}

		bool isReleased() { return this->released; }
	};

public:

	/*
	TODO:
		リソース削除関数
		チェックポイントの作成、チェックポイントまで解放
	*/

	class Handle {
	private:
		std::weak_ptr<Item> item;
	public:
		Handle(std::shared_ptr<Item> &i) : item(i) {}

		void Release() {
			auto p = item.lock();
			if(p != nullptr) p->Delete();
		}

		bool isReleased() {
			auto p = item.lock();
			if(p != nullptr) return p->isReleased();
			else             return true;
		}

	};

	ResourceStack(void) : stack() {	}

	void DeleteAll() {
		while ( stack.size() > 0){
			stack.pop();
		}
	}

	~ResourceStack(void) {
		DeleteAll();
	}

	template <class TData>
	Handle Push(TData* ptr){
		auto item = new ItemImpl<TData*>(ptr, [](TData* &p){ delete p; });
		stack.push(std::shared_ptr<Item>(item));
		return Handle(stack.top());
	}

	template <class TData, class TFinalizer>
	Handle Push(TData &res, TFinalizer finalizer){
		auto item = new ItemImpl<TData>(res, finalizer);
		stack.push(std::shared_ptr<Item>(item));
		return Handle(stack.top());
	}

};



#endif