/*
 * future.h
 *
 *  Created on: 30 Aug 2018
 *      Author: belsonba
 */

#ifndef SOURCES_STL_FUTURE_H_
#define SOURCES_STL_FUTURE_H_

// Based on http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4727.pdf


#if 0

// 33.6.2 Header <future> synopsis [future.syn]
namespace std {
	enum class future_errc {
		broken_promise = implementation-defined ,
		future_already_retrieved = implementation-defined ,
		promise_already_satisfied = implementation-defined ,
		no_state = implementation-defined
	};

	enum class launch : unspecified {
		async = unspecified ,
		deferred = unspecified ,
		implementation-defined
	};

	enum class future_status {
		ready,
		timeout,
		deferred
	};

	template<> struct is_error_code_enum<future_errc> : public true_type { };
	error_code make_error_code(future_errc e) noexcept;
	error_condition make_error_condition(future_errc e) noexcept;

	const error_category& future_category() noexcept;

	class future_error;

	template<class R> class promise;
	template<class R> class promise<R&>;
	template<> class promise<void>;

	template<class R>
		void swap(promise<R>& x, promise<R>& y) noexcept;

	template<class R, class Alloc>
		struct uses_allocator<promise<R>, Alloc>;

	template<class R> class future;
	template<class R> class future<R&>;
	template<> class future<void>;

	template<class R> class shared_future;
	template<class R> class shared_future<R&>;
	template<> class shared_future<void>;

	template<class> class packaged_task; // not defined
	template<class R, class... ArgTypes>
		class packaged_task<R(ArgTypes...)>;

	template<class R, class... ArgTypes>
		void swap(packaged_task<R(ArgTypes...)>&, packaged_task<R(ArgTypes...)>&) noexcept;
	template<class F, class... Args>
		[[nodiscard]] future<invoke_result_t<decay_t<F>, decay_t<Args>...>>
			async(F&& f, Args&&... args);
	template<class F, class... Args>
		[[nodiscard]] future<invoke_result_t<decay_t<F>, decay_t<Args>...>>
			async(launch policy, F&& f, Args&&... args);
}
#endif

#include <stl_system_error.h>
#include <stl_stdexcept.h>
#include <stl_mutex.h>

namespace std {

	// 33.6.2 Header <future> synopsis [future.syn]
	enum class future_errc {
		broken_promise = 1,
		future_already_retrieved,
		promise_already_satisfied,
		no_state
	};

	enum class launch {
		async = 0x01,
		deferred = 0x02
		// Is there a special value for 'serviced by ISR'?
	};

#if 0
	// This is an example of launch operator arithmetic - do we need it?
	inline constexpr launch operator&(launch _Left, launch _Right)
	{	/* return _Left&_Right */
		return (static_cast<launch>(static_cast<unsigned int>(_Left)
				& static_cast<unsigned int>(_Right)));
	}
#endif

	enum class future_status {
		ready,
		timeout,
		deferred
	};

	template<> struct is_error_code_enum<future_errc> : public true_type { };
	error_code make_error_code(future_errc e) noexcept;
	error_condition make_error_condition(future_errc e) _NOEXCEPT;
	const error_category& future_category() _NOEXCEPT;

	class future_error;
	template<class R> class promise;
	template<class R> class promise<R&>;
	template<> class promise<void>;

	template<class R>
		void swap(promise<R>& x, promise<R>& y) _NOEXCEPT;
#if 0
	template<class R, class Alloc>
		struct uses_allocator<promise<R>, Alloc>;
#endif
	template<class R> class future;
	template<class R> class future<R&>;
	template<> class future<void>;
	template<class R> class shared_future;
	template<class R> class shared_future<R&>;
	template<> class shared_future<void>;
	template<class> class packaged_task; // not defined
	template<class R, class... ArgTypes>
		class packaged_task<R(ArgTypes...)>;
	template<class R, class... ArgTypes>
		void swap(packaged_task<R(ArgTypes...)>&, packaged_task<R(ArgTypes...)>&) _NOEXCEPT;
#if 0
	// TODO
	template<class F, class... Args>
		[[nodiscard]] future<invoke_result_t<decay_t<F>, decay_t<Args>...>>
			async(F&& f, Args&&... args);
	template<class F, class... Args>
		[[nodiscard]] future<invoke_result_t<decay_t<F>, decay_t<Args>...>>
			async(launch policy, F&& f, Args&&... args);
#endif
} // namespace std


namespace std {
	class future_error_category : public error_category
	{	// categorize a generic error
public:
		future_error_category() _NOEXCEPT : error_category(_Future_addr)
		{
		}

		virtual const char *name() const _NOEXCEPT
		{	// get name of category
			return ("future");
		}

		virtual string message(int _Errcode) const
		{	// convert to name of error
			return string("");
		}
	};
}

// 33.6.4 Class future_error

namespace std {
	class future_error : public logic_error {
	public:
		explicit future_error(future_errc e) : ec_(error_code(e)), logic_error(error_code(e).message()) { }
		const error_code& code() const noexcept {
			return ec_;
		}
		const char* what() const noexcept {
			return ec_.message();
		}
	private:
		error_code ec_;
	};


	inline const char *future_error_map(int _Errcode) _NOEXCEPT
	{	// convert to name of future error
		switch (static_cast<future_errc>(_Errcode))
		{	// switch on error code value
		case future_errc::broken_promise:
			return ("broken promise");

		case future_errc::future_already_retrieved:
			return ("future already retrieved");

		case future_errc::promise_already_satisfied:
			return ("promise already satisfied");

		case future_errc::no_state:
			return ("no state");

		default:
			return (0);
		}
	}
}

// 33.6.5 Shared state

namespace std {
template<class R>
	class future_shared_state {
	public:
		constexpr future_shared_state() _NOEXCEPT
		: mtx_()
		{
		}
		//future_shared_state()
		future_shared_state(const future_shared_state& rhs) = delete;
		future_shared_state(future_shared_state&& rhs) = delete;
	private:
		R result_;
		//atomic
		//exception_ptr exception_;
		mutex mtx_;
		bool retrieved_;
		bool ready_;
		bool running_;
		bool ready_at_thread_exit_;
	};
}

namespace std {
template<class T>
	class static_shared_state_allocator {
	public:
		using value_type = T;
		static_shared_state_allocator(T*& static_ptr, T* static_buffer) _NOEXCEPT
		{
			ptr_ = &static_ptr;
			*ptr_ = 0;
		}
		static_shared_state_allocator(const static_shared_state_allocator& rhs) {
			ptr_ = rhs.ptr_;
			buffer_ = rhs.buffer_;
		}
		[[nodiscard]] T* allocate(size_t n) {
			//assert(n == sizeof(T));
			return (*ptr_ = buffer_);
		}
		void deallocate(T* p, size_t n) {
			//assert(p == buffer_);
			//assert(*ptr_ == buffer_);
			*ptr_ = 0;
		}
	private:
		T** ptr_;
		T*  buffer_;
	};
}

// 33.6.6 Class template promise

#if 0
namespace std {
	template<class R>
		class promise {
		public:
			promise();
			template<class Allocator>
				promise(allocator_arg_t, const Allocator& a);
			promise(promise&& rhs) noexcept;
			promise(const promise& rhs) = delete;
			~promise();
			// assignment
			promise& operator=(promise&& rhs) noexcept;
			promise& operator=(const promise& rhs) = delete;
			void swap(promise& other) noexcept;
			// retrieving the result
			future<R> get_future();
			// setting the result
			void set_value(see below );
			void set_exception(exception_ptr p);
			// setting the result with deferred notification
			void set_value_at_thread_exit(see below );
			void set_exception_at_thread_exit(exception_ptr p);
		};
	template<class R>
		void swap(promise<R>& x, promise<R>& y) noexcept;
	template<class R, class Alloc>
		struct uses_allocator<promise<R>, Alloc>;
}
#endif

#endif /* SOURCES_STL_FUTURE_H_ */
