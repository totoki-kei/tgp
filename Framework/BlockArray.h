#pragma once

#include <cassert>
#include <stdexcept>

#ifndef BLOCK_EXTENT_SIZE
#define BLOCK_EXTENT_SIZE 32
#endif

#ifndef BLOCK_DEFAULT_SIZE
#define BLOCK_DEFAULT_SIZE 32
#endif

template<
	typename T,
	int ExtentSize = BLOCK_EXTENT_SIZE,
	int StartupSize = BLOCK_DEFAULT_SIZE,
	int AutoExtend = 0>
class Block{

public:
	enum {
		BlockSize = StartupSize
	};
	typedef Block<T, ExtentSize, StartupSize, AutoExtend> this_type;
	typedef Block<T, ExtentSize, ExtentSize, AutoExtend> block_type;
	typedef T element_type;

private:
	T arr[StartupSize];
	block_type *next;

public:

	Block(){ next = nullptr; }
	Block(int size, T dfl){
		this->next = nullptr;
		block_type* p = this;

		(*this)[size - 1] = dfl;

		int i = 0;
		while (size-- > 0){
			(*p)[i++] = dfl;
			if (i == StartupSize){
				p = p->next;
				i = 0;
			}
		}
	}

	template<typename IndexT>
	T& operator[](IndexT index){
		if (index >= StartupSize){
			if (!next){
				if (!AutoExtend){
					throw std::out_of_range("buffer size over");
				}
				else {
					ExtendOne(this);
				}
			}

			return (*next)[index - StartupSize];

		}
		else
			return arr[index];
	}

	~Block(){
		delete next;
	}

	int GetCapacity(){
		if (next)
			return StartupSize + next->capacity();
		else
			return StartupSize;
	}

private:
	int ExtendOne(Block* b){
		assert(b->next == nullptr);
		b->next = new block_type();
		return block_type::BlockSize;
	}

public:
	int Extend(){
		if (this->next)
			return next->Extend();
		else
			return ExtendOne(this);
	}

	template <typename Fn>
	void Enumerate(Fn& f){
		for (int i = 0; i < StartupSize; i++){
			if (!(f(arr[i]))) return;
		}
		if (next)
			next->Enumerate(f);
	}

};

