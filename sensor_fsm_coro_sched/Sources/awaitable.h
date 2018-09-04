/*
 * awaitable.h
 *
 *  Created on: 7 Aug 2018
 *      Author: belsonba
 */

#ifndef SOURCES_AWAITABLE_H_
#define SOURCES_AWAITABLE_H_


template <typename T>
struct future_T {
	bool await_ready();
	void await_suspend(std::experimental::coroutine_handle<>);
	T await_resume();
};


#endif /* SOURCES_AWAITABLE_H_ */
