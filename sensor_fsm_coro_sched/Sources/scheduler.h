/*
 * scheduler.h
 *
 *  Created on: 1 Aug 2018
 *      Author: belsonba
 */

#ifndef SOURCES_SCHEDULER_H_
#define SOURCES_SCHEDULER_H_

#ifndef __PE_Types_H
extern "C" {
#include "PE_Types.h"
}
#endif

//#define CORO_HANDLE std::experimental::coroutine_handle

#define STE std::experimental
#include <cppcommon.h>
#include <experimental/coroutine.h>
#include <resumable.h>

namespace scheduling {

/**
 * ufuture
 */

enum ufuture_error_code {
	none = 0,
	no_state,
	future_already_retrieved,
};

template<class T>
class ufuture;

template<class T>
class ushared_state
{
public:
	typedef byte error_code_t;
public:
	ushared_state() : result_(T()), error_(error_code_t()), is_ready_(false), is_retrieved_(false), is_running_(false) { }

	bool is_ready() const { return is_ready_; }
	bool is_retrieved() const { return is_retrieved_; }
	bool is_running() const { return is_running_; }

	const T& get() const { return result_; }
	bool has_error() const { return error_ != error_code_t(); }

	void set(T&& data) { result_ = data; }
	void set(const T& data) { result_ = data; }

	void set_error(error_code_t error) { error_ = error; }
	void set_retrieved() { is_retrieved_ = true; }
private:
	T result_;
	error_code_t error_;
	bool is_ready_;
	bool is_retrieved_;
	bool is_running_;
};

template<class T>
class upromise
{
public:
	typedef ushared_state<T> state_t;
public:
	upromise() : state_(nullptr) { }
	upromise(upromise&& rhs) noexcept {
		state_ = rhs.state_;
		rhs.state_ = nullptr;
	}
	upromise(const upromise& rhs) = delete;

	upromise& operator=(upromise&& rhs) noexcept;
	upromise& operator=(const upromise& rhs) = delete;
	void swap(upromise& other) noexcept;


	ufuture<T> get_future();// {
//		return ufuture<T>(get_state_for_future());
//	}
	/*
	ufuture<T, E> get_future() {
		if (!state_) {
			// ISO contract says to throw no_state
		}
		else {
			if (is_retrieved_) {
				// ISO contract says to throw future_already_retrieved
			}
			else {
				is_retrieved_ = true;
				return ufuture<T, E>(get_state_for_future());
			}
		}
	}
	*/

	void set_value(const T& t) { get_state_for_set()->set(t); }
	void set_value(T&& t) { get_state_for_set()->set(t); }

	void set_error(typename state_t::error_code_t error);
public:
	state_t* get_state_for_future() {
		if (!state_) {
			return get_state_for_error(state_t::error_code_t::no_state);
		}
		else {
			if (state_->is_retrieved()) {
				return get_state_for_error(state_t::error_code_t::future_already_retrieved);
			}
			else {
				state_->set_retrieved();
				return state_;
			}
		}
	}
	state_t* get_state_for_set();
protected:
	static state_t* get_state_for_error(typename state_t::error_code_t e) {
		ushared_state<T>* t = new ushared_state<T>();
		t->set_error(e);
		return t;
	}
private:
	state_t *state_;
};

template<>
class upromise<void>
{
public:
	typedef ushared_state<int> state_t;
public:
	upromise();
	upromise(upromise&& rhs) noexcept;
	upromise(const upromise& rhs) = delete;

	upromise& operator=(upromise&& rhs) noexcept;
	upromise& operator=(const upromise& rhs) = delete;
	void swap(upromise& other) noexcept;


	ufuture<void> get_future();
	/*
	ufuture<T, E> get_future() {
		if (!state_) {
			// ISO contract says to throw no_state
		}
		else {
			if (is_retrieved_) {
				// ISO contract says to throw future_already_retrieved
			}
			else {
				is_retrieved_ = true;
				return ufuture<T, E>(get_state_for_future());
			}
		}
	}
	*/

	void set_value() { get_state_for_set()->set((int)1); }

	void set_error(typename state_t::error_code_t error);
public:
	state_t* get_state_for_future();
	state_t* get_state_for_set();
private:
	state_t *state_;
};

template<class T>
class ufuture
{
public:
	typedef ushared_state<T> state_t;
public:
	ufuture() noexcept;
	ufuture(ufuture&& rhs) noexcept;
	ufuture(const ufuture& rhs) = delete;
	ufuture(state_t* state) noexcept : state_(state) {
	}
	~ufuture();
	ufuture& operator=(const ufuture& rhs) = delete;
	ufuture& operator=(ufuture&&) noexcept;

	T get();

	bool valid();

	void wait() const;
	// timers TODO

	// Coroutine
	struct promise_type {
		ufuture<T> get_return_object() {
			return ufuture<T>(
					STE::coroutine_handle < promise_type > ::from_promise(*this));
		}
		auto initial_suspend() {
			return STE::suspend_never { };
		}
		auto final_suspend() {
			return STE::suspend_never { };
		}
		void return_void() {
		}
		void unhandled_exception() {}
	};

	// Awaiter
	bool await_ready() {
		return (state_->is_ready());
	}
	void await_suspend(STE::coroutine_handle < promise_type > coro) {
		  // TODO!!!!
			/*
			// change to .then when future gets .then
			thread _WaitingThread([&f, _ResumeCb]{
				f.wait();
				_ResumeCb();
			});
			_WaitingThread.detach();
			 */
	}


private:
	state_t* state_;
};

template<>
class ufuture<void>
{
public:
	typedef ushared_state<int> state_t;
public:
	ufuture() noexcept;
	ufuture(ufuture&& rhs) noexcept;
	ufuture(const ufuture& rhs) = delete;
	~ufuture();
	ufuture& operator=(const ufuture& rhs) = delete;
	ufuture& operator=(ufuture&&) noexcept;

	void get();

	bool valid();

	void wait() const;
	// timers TODO




private:
	//coroutine_handle<promise_type> _coroutine = nullptr;

	/*
	// Coroutine
	struct promise_type {
		ufuture<void> get_return_object() {
			return ufuture<void>(
					STE::coroutine_handle < promise_type > ::from_promise(*this));
		}
		auto initial_suspend() {
			return STE::suspend_never { };
		}
		auto final_suspend() {
			return STE::suspend_never { };
		}
		void return_void() {
		}
		void unhandled_exception() {}
	};
	*/
};

// Awaiter API

#if 0
template<class T>
	bool await_ready(ufuture<T>& f)
	{
	return (f._Is_ready());
	}

template<class T>
	void await_suspend(ufuture<T>& f,
		std::experimental::coroutine_handle<> coro)
	{
	  // TODO!!!!
		/*
	// change to .then when future gets .then
	thread _WaitingThread([&f, _ResumeCb]{
		f.wait();
		_ResumeCb();
	});
	_WaitingThread.detach();
		 */
	}

template<class T>
	auto await_resume(ufuture<T>& f)
	{
	return (f.get());
	}
#endif

} // namespace scheduling

namespace std { namespace experimental { inline namespace coroutines_v1 {
template<class T,
	class... ArgTypes>
	struct coroutine_traits<scheduling::ufuture<T>, ArgTypes...>
	{	// defines resumable traits for functions returning ufuture<T>
		struct promise_type {
			scheduling::upromise<T> promise_;

			scheduling::ufuture<T> get_return_object() {
				return (promise_.get_future());
			}

			bool initial_suspend() const {
				return (false);
			}

			bool final_suspend() const {
				return (false);
			}

			template<class _Ut>
			void return_value(_Ut value) {
				promise_.set_value(value);
			}

			void set_error(typename scheduling::ushared_state<T>::error_code_t e) {
				promise_.set_error(e);
			}
		};
	};

template<class... ArgTypes>
	struct coroutine_traits<scheduling::ufuture<void>, ArgTypes...>
	{	// defines resumable traits for functions returning future<void>
		struct promise_type {
			scheduling::upromise<void> promise_;

			scheduling::ufuture<void> get_return_object() {
				return (promise_.get_future());
			}

			bool initial_suspend() const {
				return (false);
			}

			bool final_suspend() const {
				return (false);
			}

			void return_void() {
				promise_.set_value();
			}

			void set_error(typename scheduling::ushared_state<int>::error_code_t e) {
				promise_.set_error(e);
			}
		};
	};

}}}

namespace scheduling {

/**
 * Task primitive types
 */

struct task_control_block_t;

enum class task_state_t : unsigned char {
	TASK_UNINITIALISED = 0,
	TASK_WAITING,
	TASK_READY,
	TASK_RUNNING,
	TASK_CLOSING,
	TASK_CLOSED
};

typedef unsigned char task_priority_t;
typedef void* task_data_t;
//typedef void taskfn_t(task_control_block_t*);
typedef resumable (*taskfn_t)(task_control_block_t*);

const task_priority_t PRIORITY_LOWEST 	= 0x00;
const task_priority_t PRIORITY_LOW 		= 0x10;
const task_priority_t PRIORITY_NORMAL 	= 0x80;
const task_priority_t PRIORITY_HIGH 	= 0xe0;
const task_priority_t PRIORITY_CRITICAL = 0xf0;

/**
 * Task type
 */

struct task_control_block_t {
	taskfn_t							taskfn;
	task_data_t 					data;
	task_priority_t 			priority;
	volatile task_state_t state;
	resumable 						resumable_;

	task_control_block_t(taskfn_t taskfn_, task_data_t data_,
		task_priority_t priority_, task_state_t state_ = task_state_t::TASK_UNINITIALISED)
	  : resumable_(), taskfn(taskfn_), data(data_), priority(priority_), state(state_)
	{
	}

	task_data_t getData() const;
	task_state_t getState() const;
	task_state_t setState(task_state_t newState) volatile;
	task_priority_t getPriority() const;
	task_priority_t setPriority(task_priority_t newPriority);
	void run();
};

/**
 * Scheduler task limits
 */

#define SCHEDULER_DEFAULT_MAX_TASKS 8

// NB - if SCHEDULER_MAX_TASKS is to be overridden, it must be
// overridden to the same value in scheduler.cpp. This is most
// safely accomplished by setting the value in the project's C++
// build variables.

#ifndef SCHEDULER_MAX_TASKS
#define SCHEDULER_MAX_TASKS SCHEDULER_DEFAULT_MAX_TASKS
#endif

#if (SCHEDULER_MAX_TASKS > 0x00ff)
#error SCHEDULER_MAX_TASKS is more than an 8-bit value
#endif

/**
 * Scheduler type
 */

class scheduler {
public:
	scheduler();
	bool registerTask(task_control_block_t* t);
	int run();
	static scheduler& instance();
	task_control_block_t* activeTask() const;
	static task_control_block_t* getActiveTask();
protected:
	uint8_t getNextTask() const;
	bool hasAnyTasks();
	void updateCounts();
	void runTask(uint8_t taskIndex);

protected:
	static task_control_block_t m_idle_task;
	uint8_t m_taskMax;
	uint8_t m_taskCount;
	task_control_block_t* m_tasks[SCHEDULER_MAX_TASKS];
	uint8_t m_lastTaskIndex;
	uint8_t m_livingTaskCount;
};

} // namespace scheduling

#endif /* SOURCES_SCHEDULER_H_ */
