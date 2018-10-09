/*
 * stl_system_error.h
 *
 *  Created on: 1 Sep 2018
 *      Author: belsonba
 */

#ifndef SOURCES_STL_SYSTEM_ERROR_H_
#define SOURCES_STL_SYSTEM_ERROR_H_

#include <stlcommon.h>
#include <stl_string.h>

// 22.5.1 Header <system_error> synopsis

namespace std {
	class error_category;
	const error_category& generic_category() _NOEXCEPT;
	const error_category& system_category() _NOEXCEPT;
	class error_code;
	class error_condition;
	class system_error;
	template<class T>
		struct is_error_code_enum : public false_type {};
	template<class T>
		struct is_error_condition_enum : public false_type {};
	enum class errc {
		address_family_not_supported, // EAFNOSUPPORT
		address_in_use, // EADDRINUSE
		address_not_available, // EADDRNOTAVAIL
		already_connected, // EISCONN
		argument_list_too_long, // E2BIG
		argument_out_of_domain, // EDOM
		bad_address, // EFAULT
		bad_file_descriptor, // EBADF
		bad_message, // EBADMSG
		broken_pipe, // EPIPE
		connection_aborted, // ECONNABORTED
		connection_already_in_progress, // EALREADY
		connection_refused, // ECONNREFUSED
		connection_reset, // ECONNRESET
		cross_device_link, // EXDEV
		destination_address_required, // EDESTADDRREQ
		device_or_resource_busy, // EBUSY
		directory_not_empty, // ENOTEMPTY
		executable_format_error, // ENOEXEC
		file_exists, // EEXIST
		file_too_large, // EFBIG
		filename_too_long, // ENAMETOOLONG
		function_not_supported, // ENOSYS
		host_unreachable, // EHOSTUNREACH
		identifier_removed, // EIDRM
		illegal_byte_sequence, // EILSEQ
		inappropriate_io_control_operation, // ENOTTY
		interrupted, // EINTR
		invalid_argument, // EINVAL
		invalid_seek, // ESPIPE
		io_error, // EIO
		is_a_directory, // EISDIR
		message_size, // EMSGSIZE
		network_down, // ENETDOWN
		network_reset, // ENETRESET
		network_unreachable, // ENETUNREACH
		no_buffer_space, // ENOBUFS
		no_child_process, // ECHILD
		no_link, // ENOLINK
		no_lock_available, // ENOLCK
		no_message_available, // ENODATA
		no_message, // ENOMSG
		no_protocol_option, // ENOPROTOOPT
		no_space_on_device, // ENOSPC
		no_stream_resources, // ENOSR
		no_such_device_or_address, // ENXIO
		no_such_device, // ENODEV
		no_such_file_or_directory, // ENOENT
		no_such_process, // ESRCH
		not_a_directory, // ENOTDIR
		not_a_socket, // ENOTSOCK
		not_a_stream, // ENOSTR
		not_connected, // ENOTCONN
		not_enough_memory, // ENOMEM
		not_supported, // ENOTSUP
		operation_canceled, // ECANCELED
		operation_in_progress, // EINPROGRESS
		operation_not_permitted, // EPERM
		operation_not_supported, // EOPNOTSUPP
		operation_would_block, // EWOULDBLOCK
		owner_dead, // EOWNERDEAD
		permission_denied, // EACCES
		protocol_error, // EPROTO
		protocol_not_supported, // EPROTONOSUPPORT
		read_only_file_system, // EROFS
		resource_deadlock_would_occur, // EDEADLK
		resource_unavailable_try_again, // EAGAIN
		result_out_of_range, // ERANGE
		state_not_recoverable, // ENOTRECOVERABLE
		stream_timeout, // ETIME
		text_file_busy, // ETXTBSY
		timed_out, // ETIMEDOUT
		too_many_files_open_in_system, // ENFILE
		too_many_files_open, // EMFILE
		too_many_links, // EMLINK
		too_many_symbolic_link_levels, // ELOOP
		value_too_large, // EOVERFLOW
		wrong_protocol_type, // EPROTOTYPE
	};
	template<> struct is_error_condition_enum<errc> : true_type {};

	// 22.5.3.5, non-member functions
	error_code make_error_code(errc e) _NOEXCEPT;
#if 0
	template<class charT, class traits>
		basic_ostream<charT, traits>&
			operator<<(basic_ostream<charT, traits>& os, const error_code& ec);
#endif

	// 22.5.4.5, non-member functions
	error_condition make_error_condition(errc e) _NOEXCEPT;
	// 22.5.5, comparison functions
	bool operator<(const error_code& lhs, const error_code& rhs) _NOEXCEPT;
	bool operator<(const error_condition& lhs, const error_condition& rhs) _NOEXCEPT;
	bool operator==(const error_code& lhs, const error_code& rhs) _NOEXCEPT;
	bool operator==(const error_code& lhs, const error_condition& rhs) _NOEXCEPT;
	bool operator==(const error_condition& lhs, const error_code& rhs) _NOEXCEPT;
	bool operator==(const error_condition& lhs, const error_condition& rhs) _NOEXCEPT;
	bool operator!=(const error_code& lhs, const error_code& rhs) _NOEXCEPT;
	bool operator!=(const error_code& lhs, const error_condition& rhs) _NOEXCEPT;
	bool operator!=(const error_condition& lhs, const error_code& rhs) _NOEXCEPT;
	bool operator!=(const error_condition& lhs, const error_condition& rhs) _NOEXCEPT;

	// 22.5.6, hash support
	template<class T> struct hash;
	template<> struct hash<error_code>;
	template<> struct hash<error_condition>;

	// 22.5, system error support
	template<class T>
		inline constexpr bool is_error_code_enum_v = is_error_code_enum<T>::value;
	template<class T>
		inline constexpr bool is_error_condition_enum_v = is_error_condition_enum<T>::value;
}

// 22.5.2 Class error_category

namespace std {
	class error_category {
	public:
		//constexpr
		error_category() _NOEXCEPT {
			_addr = 0; //reinterpret_cast<uintptr_t>(this);
		}
		virtual ~error_category() {}
		error_category(const error_category&) = delete;
		error_category& operator=(const error_category&) = delete;
		virtual const char* name() const _NOEXCEPT = 0;
		virtual error_condition default_error_condition(int ev) const _NOEXCEPT;
		virtual bool equivalent(int code, const error_condition& condition) const _NOEXCEPT;
		virtual bool equivalent(const error_code& code, int condition) const _NOEXCEPT;
		virtual string message(int ev) const = 0;
		bool operator==(const error_category& rhs) const _NOEXCEPT {
			return (_addr == rhs._addr);
		}
		bool operator!=(const error_category& rhs) const _NOEXCEPT {
			return (!(*this == rhs));
		}
		bool operator<(const error_category& rhs) const _NOEXCEPT {
			return (_addr < rhs._addr);
		}

	protected:
		uintptr_t _addr;

		enum : uintptr_t
			{	// imaginary addresses for Standard error_category objects
			_Future_addr = 1,
			_Generic_addr = 3,
			_Iostream_addr = 5,
			_System_addr = 7
			};
		error_category(uintptr_t addr) _NOEXCEPT {
			_addr = reinterpret_cast<uintptr_t>(addr);
		}
	};

	const error_category& generic_category() _NOEXCEPT;
	const error_category& system_category() _NOEXCEPT;
}

namespace std {
	class system_error_category : public error_category
	{	// categorize an operating system error
	public:
		system_error_category() _NOEXCEPT : error_category(_System_addr)
		{
		}

		virtual const char *name() const _NOEXCEPT
		{	// get name of category
			return ("system");
		}

		virtual string message(int _Errcode) const
		{	// convert to name of error
			return ("unknown error");
		}

		virtual error_condition default_error_condition(int _Errval) const _NOEXCEPT;
	};
}

namespace std {
	class generic_error_category : public error_category
	{	// categorize a generic error
public:
		generic_error_category() _NOEXCEPT : error_category(_Generic_addr)
		{
		}

		virtual const char *name() const _NOEXCEPT
		{	// get name of category
			return ("generic");
		}

		virtual string message(int _Errcode) const
		{	// convert to name of error
			return string("");
		}
	};
}

// 22.5.3 Class error_code
namespace std {
	class error_code {
	public:
		// 22.5.3.2, constructors
		error_code() _NOEXCEPT {
			val_ = 0;
			cat_ = &system_category();
		}
		error_code(int val, const error_category& cat) _NOEXCEPT {
			val_ = val;
			cat_ = &cat;
		}
		template<class ErrorCodeEnum>
			error_code(ErrorCodeEnum e) _NOEXCEPT {
			*this = make_error_code(e);
		}
		// 22.5.3.3, modifiers
		void assign(int val, const error_category& cat) _NOEXCEPT {
			val_ = val;
			cat_ = &cat;
		}
		template<class ErrorCodeEnum>
			error_code& operator=(ErrorCodeEnum e) _NOEXCEPT {
			*this = make_error_code(e);
			return *this;
		}
		void clear() _NOEXCEPT {
			val_ = 0;
			cat_ = &system_category();
		}
		// 22.5.3.4, observers
		int value() const _NOEXCEPT {
			return val_;
		}
		const error_category& category() const _NOEXCEPT {
			return *cat_;
		}
		error_condition default_error_condition() const _NOEXCEPT;
		string message() const {
			return category().message(value());
		}
		explicit operator bool() const _NOEXCEPT {
			return value() != 0;
		}

	private:
		int val_;
		const error_category* cat_;
	};
	// 22.5.3.5, non-member functions
	error_code make_error_code(errc e) _NOEXCEPT;
#if 0
	template<class charT, class traits>
		basic_ostream<charT, traits>&
			operator<<(basic_ostream<charT, traits>& os, const error_code& ec);
#endif
}

// 22.5.4 Class error_condition

namespace std {
	class error_condition {
	public:
		// 22.5.4.2, constructors
		error_condition() _NOEXCEPT {
			val_ = 0;
			cat_ = &generic_category();
		}
		error_condition(int val, const error_category& cat) _NOEXCEPT {
			val_ = val;
			cat_ = &cat;
		}
		template<class ErrorConditionEnum>
			error_condition(ErrorConditionEnum e) _NOEXCEPT {
			*this = make_error_condition(e);
		}
		// 22.5.4.3, modifiers
		void assign(int val, const error_category& cat) _NOEXCEPT {
			val_ = val;
			cat_ = &cat;
		}
		template<class ErrorConditionEnum>
			error_condition& operator=(ErrorConditionEnum e) _NOEXCEPT {
			*this = make_error_condition(e);
			return *this;
		}
		void clear() _NOEXCEPT {
			val_ = 0;
			cat_ = &generic_category();
		}
		// 22.5.4.4, observers
		int value() const _NOEXCEPT {
			return val_;
		}
		const error_category& category() const _NOEXCEPT {
			return *cat_;
		}
		string message() const {
			return category().message(value());
		}
		explicit operator bool() const _NOEXCEPT {
			return value() != 0;
		}
	private:
		int val_;
		const error_category* cat_;
	};
}

#endif /* SOURCES_STL_SYSTEM_ERROR_H_ */
