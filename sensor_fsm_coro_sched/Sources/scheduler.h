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

namespace scheduling {

/**
 * ufuture
 */

enum ufuture_error_code {
	none = 0,
	no_state,
	future_already_retrieved,
};

template<class T, class E>
class ufuture;

template<class T, class E>
class ushared_state
{
public:
	ushared_state() : result_(T()), error_(E()), is_ready_(false), is_retrieved_(false), is_running_(false) { }

	bool is_ready() const { return is_ready_; }
	bool is_retrieved() const { return is_retrieved_; }
	bool is_running() const { return is_running_; }

	const T& get() const { return result_; }
	bool has_error() const { return error_ != E(); }

	void set(T&& data) { result_ = data; }
	void set(const T& data) { result_ = data; }
	void set(T data) { result_ = data; }

	void set_error(E&& error) { error_ = error; }
	void set_error(const E& error) { error_ = error; }
	void set_error(E error) { error_ = error; }
private:
	T result_;
	E error_;
	bool is_ready_;
	bool is_retrieved_;
	bool is_running_;
};

template<class T, class E>
class upromise
{
public:
	upromise();
	upromise(upromise&& rhs) noexcept;
	upromise(const upromise& rhs) = delete;

	upromise& operator=(upromise&& rhs) noexcept;
	upromise& operator=(const upromise& rhs) = delete;
	void swap(upromise& other) noexcept;


	ufuture<T, E> get_future();
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

	void set_value(const T& t) { get_state_for_set()->set_value(t); }
	void set_value(T&& t) { get_state_for_set()->set_value(t); }

	void set_error(E&& error);
	void set_error(const E& error);
	void set_error(E error);
protected:
	ushared_state<T, E>* get_state_for_future();
	ushared_state<T, E>* get_state_for_set();
private:
	ushared_state<T, E> *state_;
};

template<class T, class E>
class ufuture
{
public:
	ufuture() noexcept;
	ufuture(ufuture&& rhs) noexcept;
	ufuture(const ufuture& rhs) = delete;
	~ufuture();
	ufuture& operator=(const ufuture& rhs) = delete;
	ufuture& operator=(ufuture&&) noexcept;

	T get();

	bool valid();

	void wait() const;
	// timers TODO

	// Coroutine
	struct promise_type {
		ufuture<T, E> get_return_object() {
			return ufuture<T, E>(
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
};


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
typedef void taskfn_t(task_control_block_t*);

const task_priority_t PRIORITY_LOWEST 	= 0x00;
const task_priority_t PRIORITY_LOW 		= 0x10;
const task_priority_t PRIORITY_NORMAL 	= 0x80;
const task_priority_t PRIORITY_HIGH 	= 0xe0;
const task_priority_t PRIORITY_CRITICAL = 0xf0;

/**
 * Task type
 */

struct task_control_block_t {
	taskfn_t 				*taskfn;
	task_data_t 			data;
	task_priority_t 		priority;
	volatile task_state_t 	state;

	task_control_block_t(taskfn_t* taskfn_, task_data_t data_,
		task_priority_t priority_, task_state_t state_ = task_state_t::TASK_UNINITIALISED)
	  : taskfn(taskfn_), data(data_), priority(priority_), state(state_)
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
