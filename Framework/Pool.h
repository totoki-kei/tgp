#pragma once

#include <stdexcept>

template <typename T, int Size>
class Pool {

public:
	typedef Pool<T, Size> Self;
	typedef T Value;

	struct ListEntry {
		Pool* pool;
		int refcount;
		ListEntry* prev;
		ListEntry* next;
		Value e;

		ListEntry() : pool{ nullptr }, e{}, prev{ nullptr }, next{ nullptr }, refcount{ 0 }{}

		void Release(){
			auto ent = this;
			ent->refcount--;
			if (ent->refcount) return;

			if (ent < pool->entries || pool->entries + Size <= ent)
				throw std::out_of_range("ent is out of range.");

			ListEntry * target = nullptr;

			// find from active, and pop
			if (ent == pool->activeHead){
				target = pool->activeHead;
				if ((pool->activeHead = target->next) != nullptr)
					target->next->prev = nullptr;
			}
			else {
				for (ListEntry* p = pool->activeHead; p != nullptr; p = p->next){
					if (ent == p){
						target = p;
						if (target->prev)
							target->prev->next = target->next;
						if (target->next)
							target->next->prev = target->prev;

						break;
					}
				}
			}
			if (target) {
				// push to stock
				if ((target->next = pool->stockHead) != nullptr)
					target->next->prev = target;
				target->prev = nullptr;
				pool->stockHead = target;
			}
		}
	};

	class Item {
		friend class Self;
	private:
		ListEntry *ent;
		Item(ListEntry* e) : ent{ e } {
		}

	public:
		Item() : ent{ nullptr } { }

		Item(const Item& i){
			this->ent = i.ent;
			if(this->ent) this->ent->refcount++;
		}

		~Item() {
			if (ent){
				ent->Release();
			}
		}

		operator bool(){
			return ent != nullptr;
		}

		Value* operator ->() {
			return &ent->e;
		}

		Value& operator *(){
			return ent->e;
		}
	};

	enum {
		PoolSize = Size,
	};


private:
	ListEntry entries[Size];
	ListEntry *activeHead;
	ListEntry *stockHead;

public:

	Pool() {
		entries[0].next = &entries[1];
		entries[0].pool = this;
		for (int i = 1; i < Size - 1; i++){
			entries[i].next = &entries[i + 1];
			entries[i].prev = &entries[i - 1];
			entries[i].pool = this;
		}
		entries[Size - 1].prev = &entries[Size - 2];
		entries[Size - 1].pool = this;

		activeHead = nullptr;
		stockHead = &entries[0];
	}

	template <typename InitFn>
	Item Get(InitFn &activator){

		// find entry
		ListEntry* target = stockHead;
		if (!target) return Item(nullptr);

		// pop from stock
		stockHead = target->next;

		// push to active
		target->prev = nullptr;
		if ((target->next = activeHead) != nullptr)
			target->next->prev = target;
		activeHead = target;

		// activate
		activator(target->e);
		return Item(target);
	}

	Item Get(const T& t){
		return Get([&t](Value& v){v = t; });
	}

	Item Get(){
		return Get([](Value&){});
	}

	template <typename Fn>
	void Enumerate(Fn& f){
		ListEntry *ent = activeHead;
		if (!ent) return;

		do {
			f(ent->e);
		} while (ent = ent->next)
	}

};



