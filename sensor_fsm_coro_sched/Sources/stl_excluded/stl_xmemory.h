/*
 * stl_xmemory..h
 *
 *  Created on: 31 Aug 2018
 *      Author: belsonba
 */

#ifndef SOURCES_STL_XMEMORY_H_
#define SOURCES_STL_XMEMORY_H_

#include <stdlib.h>
#include <stlcommon.h>


// 23.10.9 Allocator traits

namespace std {
template<class Alloc> struct allocator_traits {
	using allocator_type = Alloc;
	using value_type = typename Alloc::value_type;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using void_pointer = void*;
	using const_void_pointer = const void*;
	using difference_type = ptrdiff_t;
	using size_type = size_t;
	using propagate_on_container_copy_assignment = false_type;
	using propagate_on_container_move_assignment = false_type;
	using propagate_on_container_swap = false_type;
	using is_always_equal = true_type;
	template<class T> using rebind_alloc = allocator<T>;
	template<class T> using rebind_traits = allocator_traits<rebind_alloc<T>>;
	[[nodiscard]] static pointer allocate(Alloc& a, size_type n);
	[[nodiscard]] static pointer allocate(Alloc& a, size_type n, const_void_pointer hint);
	static void deallocate(Alloc& a, pointer p, size_type n);
	template<class T, class... Args>
	static void construct(Alloc& a, T* p, Args&&... args);
	template<class T>
	static void destroy(Alloc& a, T* p);
	static size_type max_size(const Alloc& a) noexcept;
	static Alloc select_on_container_copy_construction(const Alloc& rhs);
};
}

namespace std {

template<class T>
	struct allocator {
		using value_type = T;
		using propagate_on_container_move_assignment = true_type;
		using is_always_equal = true_type;

		allocator() _NOEXCEPT {
		}
		allocator(const allocator& rhs) _NOEXCEPT {
		}

		template<class U> allocator(const allocator<U>& other) _NOEXCEPT {
		}

		[[nodiscard]] T* allocate(size_t n) {
			return (T*)malloc(n);
		}

		void deallocate(T* p, size_t n) {
			free((void*)p);
		}
	};

template<class T, class U>
	bool operator==(const allocator<T>&, const allocator<U>&) _NOEXCEPT {
	return true;
}

template<class T, class U>
	bool operator!=(const allocator<T>&, const allocator<U>&) _NOEXCEPT {
	reurn false;
}

}

#endif /* SOURCES_STL_XMEMORY_H_ */
