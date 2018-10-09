/*
 * stl_stdexcept.h
 *
 *  Created on: 1 Sep 2018
 *      Author: belsonba
 */

#include <stlcommon.h>
#include <stl_string.h>

// 21.8.2 Class exception

namespace std {
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
}

// 22.2.2 Class logic_error

namespace std {
	class logic_error : public exception {
	public:
		explicit logic_error(const string& what_arg) : exception(what_arg) { }
		explicit logic_error(const char* what_arg) : exception(what_arg) { }
	protected:
		logic_error() {}
	};
}

