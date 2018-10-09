/*
 * crlib.h
 *
 *  Created on: 18 Sep 2018
 *      Author: Bruce Belson
 */

#ifndef SOURCES_CRLIB_H_
#define SOURCES_CRLIB_H_

#include <cppcommon.h>

namespace crlib {

class exception {
	public:
		exception() _NOEXCEPT {
		}
		exception(const exception& rhs) _NOEXCEPT {
			what_ = rhs.what_;
		}
		exception& operator=(const exception& rhs) _NOEXCEPT {
			what_ = rhs.what_;
			return *this;
		}
		virtual ~exception() {}
		virtual const char* what() const _NOEXCEPT {
			return what_;
		}
	protected:
		exception(const string& what_arg) : what_(what_arg) { }
		exception(const char* what_arg) : what_(what_arg) { }
	private:
		string what_;
	};


enum struct future_error
{
  not_ready,			// get_value called when value not available
  already_acquired	// attempt to get another future
};

template<class T>
class awaitable_state {


};


#endif /* SOURCES_CRLIB_H_ */
