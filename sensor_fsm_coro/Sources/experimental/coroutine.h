
/*
 * coroutine.h
 *
 *  Created on: 11 Jun 2017
 *      Author: belsonba
 */

#ifndef SOURCES_EXPERIMENTAL_COROUTINE_H_
#define SOURCES_EXPERIMENTAL_COROUTINE_H_

#if 0

namespace std {
typedef decltype(nullptr) nullptr_t;
}

// References from http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0057r7.pdf

// 18.11 Coroutines support library [support.coroutine]

namespace std {
namespace experimental {
inline namespace coroutines_v1 {

// 18.11.1 coroutine traits
	template <typename R, typename... ArgTypes>
	class coroutine_traits;
// 18.11.2 coroutine handle
	template <typename Promise = void>
	class coroutine_handle;
// 18.11.2.7 comparison operators:
	bool operator==(coroutine_handle<> x, coroutine_handle<> y) noexcept;
	bool operator<(coroutine_handle<> x, coroutine_handle<> y) noexcept;
	bool operator!=(coroutine_handle<> x, coroutine_handle<> y) noexcept;
	bool operator<=(coroutine_handle<> x, coroutine_handle<> y) noexcept;
	bool operator>=(coroutine_handle<> x, coroutine_handle<> y) noexcept;
	bool operator>(coroutine_handle<> x, coroutine_handle<> y) noexcept;
// 18.11.3 trivial awaitables
	struct suspend_never;
	struct suspend_always;

// 18.11.2.8 hash support:
	template<class T> struct hash;
	template<class P> struct hash<experimental::coroutine_handle<P>>;

// 18.11.1 Coroutine traits [coroutine.traits]

	template <typename R, typename... Args>
	struct coroutine_traits {
		using promise_type = typename R::promise_type;
	};

// 18.11.2 Struct template coroutine_handle [coroutine.handle]

	template<>
	struct coroutine_handle<void> {
// 18.11.2.1 construct/reset
		constexpr coroutine_handle() noexcept;
		constexpr coroutine_handle(nullptr_t) noexcept;
		coroutine_handle& operator=(nullptr_t) noexcept;
// 18.11.2.2 export/import
		static coroutine_handle from_address(void* addr) noexcept;
		void* to_address() const noexcept;
// 18.11.2.3 capacity
		explicit operator bool() const noexcept;
// 18.11.2.4 resumption
		void operator()() const;
		void resume() const;
		void destroy() const;
// 18.11.2.5 completion check
		bool done() const noexcept;
	};

	template<typename Promise>
	struct coroutine_handle: coroutine_handle<> {
// 18.11.2.1 construct/reset
		using coroutine_handle<>::coroutine_handle;
		coroutine_handle(Promise*) noexcept;
		coroutine_handle& operator=(nullptr_t) noexcept;
// 18.11.2.6 promise access
		Promise& promise() noexcept;
		Promise const& promise() const noexcept;
	};

// 18.11.3 Trivial awaitables [coroutine.trivial.awaitables]

	struct suspend_never {
		bool await_ready() {
			return true;
		}
		void await_suspend(coroutine_handle<>) {
		}
		void await_resume() {
		}
	};
	struct suspend_always {
		bool await_ready() {
			return false;
		}
		void await_suspend(coroutine_handle<>) {
		}
		void await_resume() {
		}
	};
} // namespace coroutines_v1
}
 // namespace experimental
}// namespace std

#endif

#pragma once

namespace std { namespace experimental { inline namespace coroutines_v1 {

template <typename R, typename...> struct coroutine_traits {
  using promise_type = typename R::promise_type;
};

template <typename Promise = void> struct coroutine_handle;

template <> struct coroutine_handle<void> {
  static coroutine_handle from_address(void *addr) noexcept {
    coroutine_handle me;
    me.ptr = addr;
    return me;
  }
  void operator()() { resume(); }
  void *address() const { return ptr; }
  void resume() const { __builtin_coro_resume(ptr); }
  void destroy() const { __builtin_coro_destroy(ptr); }
  bool done() const { return __builtin_coro_done(ptr); }
  coroutine_handle &operator=(decltype(nullptr)) {
    ptr = nullptr;
    return *this;
  }
  coroutine_handle(decltype(nullptr)) : ptr(nullptr) {}
  coroutine_handle() : ptr(nullptr) {}
//  void reset() { ptr = nullptr; } // add to P0057?
  explicit operator bool() const { return ptr; }

protected:
  void *ptr;
};

template <typename Promise> struct coroutine_handle : coroutine_handle<> {
  using coroutine_handle<>::operator=;

  static coroutine_handle from_address(void *addr) noexcept {
    coroutine_handle me;
    me.ptr = addr;
    return me;
  }

  Promise &promise() const {
    return *reinterpret_cast<Promise *>(
        __builtin_coro_promise(ptr, alignof(Promise), false));
  }
  static coroutine_handle from_promise(Promise &promise) {
    coroutine_handle p;
    p.ptr = __builtin_coro_promise(&promise, alignof(Promise), true);
    return p;
  }
};

  template <typename _PromiseT>
  bool operator==(coroutine_handle<_PromiseT> const& _Left,
    coroutine_handle<_PromiseT> const& _Right) noexcept
  {
    return _Left.address() == _Right.address();
  }

  template <typename _PromiseT>
  bool operator!=(coroutine_handle<_PromiseT> const& _Left,
    coroutine_handle<_PromiseT> const& _Right) noexcept
  {
    return !(_Left == _Right);
  }

struct suspend_always {
  bool await_ready() { return false; }
  void await_suspend(coroutine_handle<>) {}
  void await_resume() {}
};
struct suspend_never {
  bool await_ready() { return true; }
  void await_suspend(coroutine_handle<>) {}
  void await_resume() {}
};

}}}

namespace coro = std::experimental::coroutines_v1;

#endif /* SOURCES_EXPERIMENTAL_COROUTINE_H_ */


