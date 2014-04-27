#pragma once

#include <functional>
#include <stdexcept>

template <typename T, int Size>
class Pool {

public:
	typedef Pool<T, Size> Self;
	typedef T Value;
	typedef function<void(Value&)> Activator;

	struct ListEntry {
		ListEntry* prev;
		ListEntry* next;
		Value e;

		ListEntry() : e(), prev(nullptr), next(nullptr){ }
	};

	class Item {
		friend class Self;
	private:
		ListEntry *ent;
		Item(ListEntry* e) : ent(e) { }

	public:
		Item() : ent(nullptr) { }

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
		for (int i = 1; i < Size - 1; i++){
			entries[i].next = &entries[i + 1];
			entries[i].prev = &entries[i - 1];
		}
		entries[Size - 1].prev = &entries[Size - 2];

		activeHead = nullptr;
		stockHead = &entries[0];
	}

	template <typename InitFn>
	Item Activate(InitFn &activator){

		// find entry
		ListEntry* target = stockHead;
		if (!target) return Item(nullptr);

		// pop from stock
		stockHead = target->next;

		// push to active
		target->prev = nullptr;
		if((target->next = activeHead) != nullptr)
			target->next->prev = target;
		activeHead = target;

		// activate
		activator(target->e);
		return Item(target);
	}

	Item Activate(){
		return Activate([](Value&){});
	}


	void Deactivate(Item& i){
		auto ent = i.ent;

		if (ent < entries || entries + Size <= ent)
			throw std::out_of_range("ent is out of range.");

		ListEntry * target = nullptr;

		// find from active, and pop
		if (ent == activeHead){
			target = activeHead;
			if ((activeHead = target->next) != nullptr)
				target->next->prev = nullptr;
		}
		else {
			for (ListEntry* p = activeHead; p != nullptr; p = p->next){
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
			if((target->next = stockHead) != nullptr)
				target->next->prev = target;
			target->prev = nullptr;
			stockHead = target;

			i.ent = nullptr;
		}
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

#ifdef UNITTEST
void PoolTest(){

	static struct test_set {
		typedef Pool<long, 8192> TestPool;
		typedef TestPool::Item item_type;

		TestPool testPool;
		test_set() {
			testPool.Activator = [](long& d){ d++; };
		}
		void test1 () {
			item_type p = testPool.Activate();
			if (!p) return;
			test1();
			testPool.Deactivate(p);
		}

		void test2() {
			item_type p = testPool.Activate();
			item_type q = testPool.Activate();
			if (p) testPool.Deactivate(p);
			if (q) {
				test2();
				(*q) = -(*q);
				testPool.Deactivate(q);
			}
		}
	} test;
	

	test.test1();
	test.test2();
}
#endif

#ifdef BACKUP
template <typename T, int Size>
class Pool {

public:
	typedef T Value;

	struct ListEntry {
		Value e;
		ListEntry* next;

		ListEntry() : e(), next(nullptr){	}
	};

	enum {
		PoolSize = Size,
	};

	std::function<void(Value&)> Activator;

private:
	ListEntry entries[Size];
	ListEntry *activeHead;
	ListEntry *stockHead;

public:
	Pool() {
		for (int i = 0; i < Size - 1; i++){
			entries[i].next = &entries[i + 1];
		}
		activeHead = nullptr;
		stockHead = &entries[0];
	}


	// InitialiserT
	//  void operator () (Value&) を持つファンクタ
	Value* Activate(){

		// find entry
		ListEntry* target = stockHead;
		if (!target) return nullptr;

		// pop from stock
		stockHead = target->next;

		// push to active
		target->next = activeHead;
		activeHead = target;

		// activate
		Activator(target->e);
		return &target->e;
	}

	void Deactivate(Value* ent){
		if (ent < &entries->e || &(entries + Size)->e <= ent)
			throw std::out_of_range("ent is out of range.");

		ListEntry * target = nullptr;

		// find from active, and pop
		if (ent == &activeHead->e){
			target = activeHead;
			activeHead = target->next;
		}
		else {
			for (ListEntry* p = activeHead; p->next != nullptr; p = p->next){
				if (ent == &(p->next->e)){
					target = p->next;
					p->next = p->next->next;

					break;
				}
			}
		}
		if (target) {
			// push to stock
			target->next = stockHead;
			stockHead = target;
		}
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

#endif

#ifdef BACKUP
#pragma once

#include <functional>
#include <stdexcept>

template <typename T, int Size>
class Pool {

public:
	typedef T Value;

	struct ListEntry {
		Value e;
		ListEntry* next;

		ListEntry() : e(), next(nullptr){	}
	};

	enum {
		PoolSize = Size,
	};

	std::function<void(Value&)> Activator;

private:
	ListEntry entries[Size];
	ListEntry *activeHead;
	ListEntry *activeTail;
	ListEntry *stockHead;
	ListEntry *stockTail;

public:
	Pool() {
		for (int i = 0; i < Size - 1; i++){
			entries[i].next = &entries[i + 1];
		}
		activeHead = nullptr;
		activeTail = nullptr;
		stockHead = &entries[0];
		stockTail = &entries[Size - 1];
	}


	// InitialiserT
	//  void operator () (Value&) を持つファンクタ
	Value* Activate(){

		// find entry
		ListEntry* target = stockHead;
		if (!target) return nullptr;

		// dequeue from stock
		stockHead = target->next;
		if (!stockHead){
			stockTail = nullptr;
		}

		if (!activeHead) {
			activeHead = target;
			activeTail = target;
		}

		// enqueue to active
		activeTail->next = target;
		activeTail = target;

		// activate
		Activator(target->e);
		return &target->e;
	}

	void Deactivate(Value* ent){
		if (ent < &entries->e || &(entries + Size)->e <= ent)
			throw std::out_of_range("ent is out of range.");

		ListEntry * target = nullptr;

		// find from active, and dequeue
		if (ent == &activeHead->e){
			target = activeHead;
			activeHead = activeHead->next;
			if (!activeHead)
				activeTail = nullptr;
		}
		else {
			for (ListEntry* p = activeHead; p != nullptr; p = p->next){
				if (ent == &(p->next->e)){
					target = p->next;
					if (target == activeTail){
						activeTail = p;
					}
					p->next = p->next->next;

					break;
				}
			}
		}

		// enqueue to stock
		if (!stockHead){
			stockHead = stockTail = target;
		}
		else {
			stockTail->next = target;
			stockTail = target;
		}

		target->next = nullptr;
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

#endif