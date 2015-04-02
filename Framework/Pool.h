#pragma once

#include <stdexcept>

#include <array>


template <typename T, int Size>
class Pool {

public:
	typedef Pool<T, Size> Self;
	typedef T Value;

private:
	std::array<Value, Size> values;
	std::array<unsigned int, Size> refcounts;

	int nextIndex;

	template <typename... InitArgs>
	void addref(int index, InitArgs&&... f){
		auto& r = refcounts[index];
		if (r == 0){
			new (&values[index]) Value(std::forward<InitArgs>(f)...);
		}
		r++;
	}


	void delref(int index){
		auto& r = refcounts[index];
		if (r > 0){
			r--;
			if (r == 0){
				values[index].Value::~Value();
			}
		}
	};

public:
	class Item {
		friend class Pool;

		Self* pool;
		int index;

		void resetref(){
			if (pool) {
				pool->delref(index);
			}
			pool = nullptr;
			index = -1;
		}

		template <typename... InitArgs>
		void setref(Self* p, int i, InitArgs&&... f){
			if (pool) {
				pool->delref(index);
			}
			pool = p;
			index = i;
			if (pool){
				pool->addref(i, std::forward<InitArgs>(f)...);
			}
		}
		// Poolとインデックスとコンストラクタ引数から構築するコンストラクタ
		template <typename... InitArgs>
		Item(Self* p, int i, InitArgs&&... f)
			: pool{ nullptr }, index{ -1 }
		{
			setref(p, i, std::forward<InitArgs>(f)...);
		}


	public:
		// デフォルトコンストラクタ
		Item() 
			: pool{ nullptr }, index{ -1 }
		{
		}

		// コピーコンストラクタ
		Item(const Item& i) 
			: pool{ nullptr }, index{ -1 }
		{
			*this = h;
		}

		// ムーブコンストラクタ
		Item(Item&& i) 
			: pool{ i.pool }, index{ i.index }
		{
			i.pool = nullptr;
			i.index = -1;
		}

		~Item(){
			resetref();
		}

		Value* operator ->() const {
			return &pool->values[index];
		}
		T& operator *() const {
			return pool->values[index];
		}
		explicit operator bool() const {
			return pool && (index >= 0);
		}
		Item& operator =(const Item& right){
			setref(right.pool, right.index);			
			return *this;
		}
		Item& operator =(Item&& right){
			this->pool = right.pool;
			this->index = right.index;
			right.pool = nullptr;
			right.index = -1;
			return *this;
		}

	};

	Pool() : nextIndex{ 0 }{
		refcounts.fill(0);
	}

	template <typename... InitArgs>
	Item Get(InitArgs&&... f){
		for (int i = 0; i < Size; i++){
			if (refcounts[nextIndex] == 0){
				int index = nextIndex;
				nextIndex++;
				if (nextIndex == Size) nextIndex = 0;

				// RVOによる最適化を期待する...
				return Item{ this, index, std::forward<InitArgs>(f)... };
			}
			nextIndex++;
			if (nextIndex == Size) nextIndex = 0;
		}
		return Item{};
	}


};
