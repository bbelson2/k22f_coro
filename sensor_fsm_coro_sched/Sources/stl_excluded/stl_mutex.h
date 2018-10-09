/*
 * stl_mutex.h
 *
 *  Created on: 3 Sep 2018
 *      Author: belsonba
 */

#ifndef SOURCES_STL_MUTEX_H_
#define SOURCES_STL_MUTEX_H_

#include <stlcommon.h>

// 33.4.1 Header <mutex> synopsis [mutex.syn]
namespace std {
	class mutex;
	class recursive_mutex;
	class timed_mutex;
	class recursive_timed_mutex;
	struct defer_lock_t { explicit defer_lock_t() = default; };
	struct try_to_lock_t { explicit try_to_lock_t() = default; };
	struct adopt_lock_t { explicit adopt_lock_t() = default; };
	inline constexpr defer_lock_t defer_lock { };
	inline constexpr try_to_lock_t try_to_lock { };
	inline constexpr adopt_lock_t adopt_lock { };
	template<class Mutex> class lock_guard;
	template<class... MutexTypes> class scoped_lock;
	template<class Mutex> class unique_lock;
	template<class Mutex>
	void swap(unique_lock<Mutex>& x, unique_lock<Mutex>& y) noexcept;
	template<class L1, class L2, class... L3> int try_lock(L1&, L2&, L3&...);
	template<class L1, class L2, class... L3> void lock(L1&, L2&, L3&...);
	struct once_flag;
	template<class Callable, class... Args>
	void call_once(once_flag& flag, Callable&& func, Args&&... args);
}

// 33.4.3.2.1 Class mutex [thread.mutex.class]
namespace std {
	class mutex {
	public:
		//constexpr mutex() _NOEXCEPT;
		mutex();
		~mutex();
		mutex(const mutex&) = delete;
		mutex& operator=(const mutex&) = delete;
		void lock();
		bool try_lock();
		void unlock();
		using native_handle_type = unsigned int; // see 33.2.3
		native_handle_type native_handle(); // see 33.2.3
	};
}

#endif /* SOURCES_STL_MUTEX_H_ */
