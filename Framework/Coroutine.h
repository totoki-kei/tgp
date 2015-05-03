#pragma once

#include <functional>
#include <utility>
#include <boost/coroutine/asymmetric_coroutine.hpp>

#ifdef _DEBUG
#ifdef _DLL
#pragma comment (lib, "libboost_coroutine-vc120-mt-gd-1_57.lib")
#else
#pragma comment (lib, "libboost_coroutine-vc120-mt-sgd-1_57.lib")
#endif
#else
#ifdef _DLL
#pragma comment (lib, "libboost_coroutine-vc120-mt-1_57.lib")
#else
#pragma comment (lib, "libboost_coroutine-vc120-mt-s-1_57.lib")
#endif
#endif


#if 0
template <typename T, typename StateT>
class Coroutine {
	typedef typename boost::coroutines::asymmetric_coroutine<T>::push_type pusher_t;
	typedef typename boost::coroutines::asymmetric_coroutine<T>::pull_type puller_t;

public:
	class Yielder {
		friend class Coroutine;
		pusher_t &pusher;

	public:

		StateT& state;

		Yielder() = delete;
		Yielder(const Yielder&) = delete;

		void operator ()(T ret) {
			pusher(ret);
		}

	private:
		Yielder(pusher_t& p, StateT& s) : pusher{ p }, state{ s } {}
	};

	typedef std::pair<bool, T> ReturnType;

private:
	puller_t coro;
	std::function<void(Yielder&)> fn;

	bool started;
	bool lives;

public:
	StateT state;

public:
	template<typename Fn>
	Coroutine(Fn f)
		: fn{ f }
		, state()
		, started{ false }
		, lives{ false } {}

	template<typename Fn>
	Coroutine(Fn f, const StateT& s)
		: fn{ f }
		, state(s)
		, started{ false }
		, lives{ false } {}

	template<typename Fn>
	Coroutine(Fn f, StateT&& s)
		: fn{ f }
		, state(std::move(s))
		, started{ false }
		, lives{ false } {}

	operator bool() {
		return lives || !started;
	}

	ReturnType operator ()() {
		if (!started) {
			coro.swap(puller_t{ [&](pusher_t& p) {
				Yielder y{ p, state };
				fn(y);
			} });
			started = true;
			lives = !!coro;
		}
		else if (lives) {
			coro();
			lives = !!coro;
		}

		if (lives) {
			return std::make_pair<bool, T>(true, coro.get());
		}
		else {
			return std::make_pair<bool, T>(false, T());
		}

	}

	void Reset() {
		coro.swap(puller_t{});
		started = lives = false;
	}

	template<typename Fn>
	void Reset(Fn f) {
		coro.swap(puller_t{});
		fn = f;
		started = lives = false;
	}

};
#else

#if 0
template <typename T, typename StateT = void>
class Coroutine {
	typedef typename boost::coroutines::asymmetric_coroutine<T>::push_type pusher_t;
	typedef typename boost::coroutines::asymmetric_coroutine<T>::pull_type puller_t;
	typedef Coroutine<T, StateT> self_t;

public:
	class Yielder {
		friend class Coroutine;
		puller_t &puller;

	public:

		StateT& state;

		Yielder() = delete;
		Yielder(const Yielder&) = delete;

		T operator ()() {
			auto ret = puller.get();
			puller();
			return ret;
		}

	private:
		Yielder(puller_t& p, StateT& s) : puller{ p }, state{ s } {}
	};

private:
	pusher_t coro;

	template <typename Fn>
	void coro_wrap(puller_t& p, Fn f) {
		Yielder y{ p, state };
		f(y);
	}

public:
	StateT state;

public:
	template<typename Fn>
	Coroutine(Fn f)
		: coro{ std::bind(&self_t::coro_wrap<Fn>, this, std::placeholders::_1, f) }
		, state() {}

	template<typename Fn>
	Coroutine(Fn f, const StateT& s)
		: coro{ std::bind(&self_t::coro_wrap<Fn>, this, std::placeholders::_1, f) }
		, state(s){}

	template<typename Fn>
	Coroutine(Fn f, StateT&& s)
		: coro{ std::bind(&self_t::coro_wrap<Fn>, this, std::placeholders::_1, f) }
		, state(std::move(s)){}

	Coroutine() {}

	Coroutine(const self_t& c) : coro{ c.coro }, state{ c.state } {}

	Coroutine(self_t&& c) : coro(std::move(c.coro)), state(std::move(c.state)) {}

	operator bool() {
		return !!coro;
	}

	bool operator ()(T&& arg) {
		if (coro) {
			coro(arg);
		}

		return !!coro;

	}

	template<typename Fn>
	void Reset(Fn f) {
		coro.swap(pusher_t{ std::bind(&self_t::coro_wrap<Fn>, this, std::placeholders::_1, f) });
	}

};


template <typename T>
class Coroutine<T, void> {
#else
template <typename T>
class Coroutine {
#endif
	typedef typename boost::coroutines::asymmetric_coroutine<T>::push_type pusher_t;
	typedef typename boost::coroutines::asymmetric_coroutine<T>::pull_type puller_t;
	typedef Coroutine<T> self_t;

public:
	class Yielder {
		friend class Coroutine;
		puller_t &puller;

	public:

		Yielder() = delete;
		Yielder(const Yielder&) = delete;

		T operator ()() {
			auto ret = puller.get();
			puller();
			return ret;
		}

	private:
		Yielder(puller_t& p) : puller{ p } {}
	};

private:
	pusher_t coro;

	template <typename Fn>
	void coro_wrap(puller_t& p, Fn f) {
		//try {
			Yielder y{ p };
			f(y);
		//}
		//catch (boost::coroutines::detail::forced_unwind const& e) {
		//	throw; // required for Boost Coroutine!
		//}
	}

public:
	template<typename Fn>
	Coroutine(Fn f)
		: coro{ 
			std::bind(&self_t::coro_wrap<Fn>,
			this, std::placeholders::_1, f), boost::coroutines::attributes(boost::coroutines::stack_unwind, boost::coroutines::fpu_preserved) } {}

	Coroutine() : coro{} {}

	//Coroutine(const self_t& c) : coro{ c.coro } {}
	Coroutine(const self_t&) = delete;

	Coroutine(self_t&& c) : coro(std::move(c.coro)) {}

	operator bool() {
		return !!coro;
	}

	bool operator ()(T&& arg) {
		if (coro) {
			coro(arg);
		}

		return !!coro;

	}

	template<typename Fn>
	void Reset(Fn f) {
		coro = pusher_t(
			std::bind(&self_t::coro_wrap<Fn>, this, std::placeholders::_1, f),
			boost::coroutines::attributes(boost::coroutines::stack_unwind, boost::coroutines::fpu_preserved));
	}

};

#endif
