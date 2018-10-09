/*
 * string.h
 *
 *  Created on: 31 Aug 2018
 *      Author: belsonba
 */

#ifndef SOURCES_STL_STRING_H_
#define SOURCES_STL_STRING_H_

#include <stlcommon.h>
//#include <stl_xmemory.h>
#include <stdio.h>
#include <string.h>

#ifndef EOF
#define EOF (-1)
#endif

// 24.2 Character traits

namespace std {
/*
template<class _Elem,
	class _Int_type>
	struct _Char_traits
{
typedef _Elem char_type;
typedef _Int_type int_type;
typedef streampos pos_type;
typedef streamoff off_type;
typedef _Mbstatet state_type;

static int compare(const _Elem * _First1,
	_In_reads_(_Count) const _Elem * _First2, size_t _Count) _NOEXCEPT // strengthened
	{	// compare [_First1, _First1 + _Count) with [_First2, ...)
	for (; 0 < _Count; --_Count, ++_First1, ++_First2)
		if (!eq(*_First1, *_First2))
			return (lt(*_First1, *_First2) ? -1 : +1);
	return (0);
	}

static size_t __CLRCALL_OR_CDECL length(_In_z_ const _Elem * _First) _NOEXCEPT // strengthened
	{	// find length of null-terminated sequence
	size_t _Count;
	for (_Count = 0; !eq(*_First, _Elem()); ++_First)
		++_Count;
	return (_Count);
	}

static _Elem * __CLRCALL_OR_CDECL copy(_Out_writes_all_(_Count) _Elem * const _First1,
	_In_reads_(_Count) const _Elem * _First2, size_t _Count) _NOEXCEPT // strengthened
	{	// copy [_First2, _First2 + _Count) to [_First1, ...)
	_Elem * _Next = _First1;
	for (; 0 < _Count; --_Count, ++_Next, ++_First2)
		assign(*_Next, *_First2);
	return (_First1);
	}

_Pre_satisfies_(_Dest_size >= _Count) static _Elem * __CLRCALL_OR_CDECL _Copy_s(
	_Out_writes_all_(_Dest_size) _Elem * const _First1, const size_t _Dest_size,
	_In_reads_(_Count) const _Elem * const _First2, const size_t _Count) _NOEXCEPT
	{	// copy [_First2, _First2 + _Count) to [_First1, _First1 + _Dest_size)
	_SCL_SECURE_CRT_VALIDATE(_Count <= _Dest_size, NULL);
	return (copy(_First1, _First2, _Count));
	}

static const _Elem * __CLRCALL_OR_CDECL find(_In_reads_(_Count) const _Elem * _First,
	size_t _Count, const _Elem& _Ch) _NOEXCEPT // strengthened
	{	// look for _Ch in [_First, _First + _Count)
	for (; 0 < _Count; --_Count, ++_First)
		if (eq(*_First, _Ch))
			return (_First);
	return (0);
	}

static _Elem * __CLRCALL_OR_CDECL move(_Out_writes_all_(_Count) _Elem * const _First1,
	_In_reads_(_Count) const _Elem * _First2, size_t _Count) _NOEXCEPT // strengthened
	{	// copy [_First2, _First2 + _Count) to [_First1, ...)
	_Elem * _Next = _First1;
	if (_First2 < _Next && _Next < _First2 + _Count)
		for (_Next += _Count, _First2 += _Count; 0 < _Count; --_Count)
			assign(*--_Next, *--_First2);
	else
		for (; 0 < _Count; --_Count, ++_Next, ++_First2)
			assign(*_Next, *_First2);
	return (_First1);
	}

static _Elem * __CLRCALL_OR_CDECL assign(_Out_writes_all_(_Count) _Elem * const _First,
	size_t _Count, const _Elem _Ch) _NOEXCEPT // strengthened
	{	// assign _Count * _Ch to [_First, ...)
	_Elem * _Next = _First;
	for (; 0 < _Count; --_Count, ++_Next)
		assign(*_Next, _Ch);
	return (_First);
	}

static void __CLRCALL_OR_CDECL assign(_Elem& _Left, const _Elem& _Right) _NOEXCEPT
	{	// assign an element
	_Left = _Right;
	}

static constexpr bool __CLRCALL_OR_CDECL eq(const _Elem& _Left, const _Elem& _Right) _NOEXCEPT
	{	// test for element equality
	return (_Left == _Right);
	}

static constexpr bool __CLRCALL_OR_CDECL lt(const _Elem& _Left, const _Elem& _Right) _NOEXCEPT
	{	// test if _Left precedes _Right
	return (_Left < _Right);
	}

static constexpr _Elem __CLRCALL_OR_CDECL to_char_type(const int_type& _Meta) _NOEXCEPT
	{	// convert metacharacter to character
	return ((_Elem)_Meta);
	}

static constexpr int_type __CLRCALL_OR_CDECL to_int_type(const _Elem& _Ch) _NOEXCEPT
	{	// convert character to metacharacter
	return ((int_type)_Ch);
	}

static constexpr bool __CLRCALL_OR_CDECL eq_int_type(const int_type& _Left, const int_type& _Right) _NOEXCEPT
	{	// test for metacharacter equality
	return (_Left == _Right);
	}

static constexpr int_type __CLRCALL_OR_CDECL not_eof(const int_type& _Meta) _NOEXCEPT
	{	// return anything but EOF
	return (_Meta != eof() ? _Meta : !eof());
	}

static constexpr int_type __CLRCALL_OR_CDECL eof() _NOEXCEPT
	{	// return end-of-file metacharacter
	return ((int_type)EOF);
	}
};

*/
#ifndef _IOSFWD_
	typedef long long streamoff;	// not used
	typedef long long streampos;	// not used
	typedef unsigned long mbstate_t; 	// not used
#endif

	template<class charT> struct char_traits {
	public:
		using char_type = charT;
		using int_type = int;
		using off_type = streamoff;
		using pos_type = streampos;
		using state_type = mbstate_t;

		static constexpr void assign(char_type& c1, const char_type& c2) _NOEXCEPT {
			c1 = c2;
		}
		static constexpr bool eq(char_type c1, char_type c2) _NOEXCEPT {
			return c1 == c2;
		}
		static constexpr bool lt(char_type c1, char_type c2) _NOEXCEPT {
			return c1 < c2;
		}
		static constexpr int compare(const char_type* s1, const char_type* s2, size_t n) {
			for (; 0 < n; --n, ++s1, ++s2) {
				if (!eq(*s1, *s2)) {
					return (lt(*s1, *s2) ? -1 : 1);
				}
			}
			return 0;
		}
		static constexpr size_t length(const char_type* s) {
			size_t i = 0;
			for (; !eq(*s, charT()); ++s) {
				++i;
			}
			return i;
		}
		static constexpr const char_type* find(const char_type* s, size_t n, const char_type& a) {
			for (; 0 < n; --n, ++s) {
				if (!eq(*s, a)) {
					return s;
				}
			}
			return 0;
		}
		static char_type* move(char_type* s1, const char_type* s2, size_t n) {
			char_type* r = s1;
			if (s2 < s1 && s1 < s2 + n) {
				for (s1 += n, s2 += n; 0 < n; --n) {
					assign(*--s1, *--s2);
				}
			}
			else {
				for (; 0 < n; --n, ++s1, ++s2) {
					assign(*s1, *s2);
				}
			}
			return r;
		}
		static char_type* copy(char_type* s1, const char_type* s2, size_t n) {
			char_type* r = s1;
			for (; 0 < n; --n, ++s1, ++s2) {
				assign(*s1, *s2);
			}
			return r;
		}
		static char_type* assign(char_type* s, size_t n, char_type a) {
			char_type* r = s;
			for (; 0 < n; --n, ++s) {
				assign(*s, a);
			}
			return r;
		}
		static constexpr int_type not_eof(int_type c) _NOEXCEPT {
			return (c != eof()) ? c : eof();
		}
		static constexpr char_type to_char_type(int_type c) _NOEXCEPT {
			return (char_type)c;
		}
		static constexpr int_type to_int_type(char_type c) _NOEXCEPT {
			return (int_type)c;
		}
		static constexpr bool eq_int_type(int_type c1, int_type c2) _NOEXCEPT {
			return c1 == c2;
		}
		static constexpr int_type eof() _NOEXCEPT {
			return (int_type)EOF;
		}

	};
}

// 24.2.3.1 struct char_traits<char> [char.traits.specializations.char]

namespace std {
	template<> struct char_traits<char> {
		typedef char _Elem;
		using char_type = char;
		using int_type = int;
		using off_type = streamoff;
		using pos_type = streampos;
		using state_type = mbstate_t;
		using cmp_type = unsigned char;
		static constexpr void assign(char_type& c1, const char_type& c2) _NOEXCEPT {
			c1 = c2;
		}
		static constexpr bool eq(char_type c1, char_type c2) _NOEXCEPT {
			return c1 == c2;
		}
		static constexpr bool lt(char_type c1, char_type c2) _NOEXCEPT {
			return (cmp_type)c1 < (cmp_type)c2;
		}
		static constexpr int compare(const char_type* s1, const char_type* s2, size_t n) {
			return __builtin_memcmp(s1, s2, n);
		}
		static constexpr size_t length(const char_type* s) {
			return __builtin_strlen(s);
		}
		static constexpr const char_type* find(const char_type* s, size_t n,
				const char_type& a) {
			return (const char_type*)__builtin_memchr(s, a, n);
		}
		static char_type* move(char_type* s1, const char_type* s2, size_t n) {
			return (char_type*)::memmove(s1, s2, n);
		}
		static char_type* copy(char_type* s1, const char_type* s2, size_t n) {
			return (char_type*)::memcpy(s1, s2, n);
		}
		static char_type* assign(char_type* s, size_t n, char_type a) {
			return (char_type*)::memset(s, a, n);
		}
		static constexpr int_type not_eof(int_type c) _NOEXCEPT {
			return (c != eof()) ? c : eof();
		}
		static constexpr char_type to_char_type(int_type c) _NOEXCEPT {
			return (char_type)c;
		}
		static constexpr int_type to_int_type(char_type c) _NOEXCEPT {
			return (int_type)(unsigned char)c;
		}
		static constexpr bool eq_int_type(int_type c1, int_type c2) _NOEXCEPT {
			return c1 == c2;
		}
		static constexpr int_type eof() _NOEXCEPT {
			return (EOF);
		}
	};
}

// 24.3.2 Class template basic_string
#if 0
namespace std {
	template<class charT, class traits = char_traits<charT>,
	class Allocator = allocator<charT>>
	class basic_string {
	public:
		// types
		using traits_type = traits;
		using value_type = charT;
		using allocator_type = Allocator;
		using size_type = typename allocator_traits<Allocator>::size_type;
		using difference_type = typename allocator_traits<Allocator>::difference_type;
		using pointer = typename allocator_traits<Allocator>::pointer;
		using const_pointer = typename allocator_traits<Allocator>::const_pointer;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = implementation-defined ; // see 26.2
		using const_iterator = implementation-defined ; // see 26.2
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		static const size_type npos = -1;

		// 24.3.2.2, construct/copy/destroy
		basic_string() noexcept(noexcept(Allocator())) : basic_string(Allocator()) { }
		explicit basic_string(const Allocator& a) noexcept;
		basic_string(const basic_string& str);
		basic_string(basic_string&& str) noexcept;
		basic_string(const basic_string& str, size_type pos, const Allocator& a = Allocator());
		basic_string(const basic_string& str, size_type pos, size_type n,
		const Allocator& a = Allocator());
		template<class T>
		basic_string(const T& t, size_type pos, size_type n, const Allocator& a = Allocator());
		explicit basic_string(basic_string_view<charT, traits> sv, const Allocator& a = Allocator());
		basic_string(const charT* s, size_type n, const Allocator& a = Allocator());
		basic_string(const charT* s, const Allocator& a = Allocator());
		basic_string(size_type n, charT c, const Allocator& a = Allocator());
		template<class InputIterator>
		basic_string(InputIterator begin, InputIterator end, const Allocator& a = Allocator());
		basic_string(initializer_list<charT>, const Allocator& = Allocator());
		basic_string(const basic_string&, const Allocator&);
		basic_string(basic_string&&, const Allocator&);
		~basic_string();
		basic_string& operator=(const basic_string& str);
		basic_string& operator=(basic_string&& str)
		noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		allocator_traits<Allocator>::is_always_equal::value);
		basic_string& operator=(basic_string_view<charT, traits> sv);
		basic_string& operator=(const charT* s);
		basic_string& operator=(charT c);
		basic_string& operator=(initializer_list<charT>);

		// 24.3.2.3, iterators
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;
		reverse_iterator rbegin() noexcept;
		const_reverse_iterator rbegin() const noexcept;
		reverse_iterator rend() noexcept;
		const_reverse_iterator rend() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
		const_reverse_iterator crbegin() const noexcept;
		const_reverse_iterator crend() const noexcept;

		// 24.3.2.4, capacity
		size_type size() const noexcept;
		size_type length() const noexcept;
		size_type max_size() const noexcept;
		void resize(size_type n, charT c);
		void resize(size_type n);
		size_type capacity() const noexcept;
		void reserve(size_type res_arg = 0);
		void shrink_to_fit();
		void clear() noexcept;
		[[nodiscard]] bool empty() const noexcept;

		// 24.3.2.5, element access
		const_reference operator[](size_type pos) const;
		reference operator[](size_type pos);
		const_reference at(size_type n) const;
		reference at(size_type n);
		const charT& front() const;
		charT& front();
		const charT& back() const;
		charT& back();

		// 24.3.2.6, modifiers
		basic_string& operator+=(const basic_string& str);
		basic_string& operator+=(basic_string_view<charT, traits> sv);
		basic_string& operator+=(const charT* s);
		basic_string& operator+=(charT c);
		basic_string& operator+=(initializer_list<charT>);
		basic_string& append(const basic_string& str);
		basic_string& append(const basic_string& str, size_type pos, size_type n = npos);
		basic_string& append(basic_string_view<charT, traits> sv);
		template<class T>
		basic_string& append(const T& t, size_type pos, size_type n = npos);
		basic_string& append(const charT* s, size_type n);
		basic_string& append(const charT* s);
		basic_string& append(size_type n, charT c);
		template<class InputIterator>
		basic_string& append(InputIterator first, InputIterator last);
		basic_string& append(initializer_list<charT>);
		void push_back(charT c);
		basic_string& assign(const basic_string& str);
		basic_string& assign(basic_string&& str)
		noexcept(allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		allocator_traits<Allocator>::is_always_equal::value);
		basic_string& assign(const basic_string& str, size_type pos, size_type n = npos);
		basic_string& assign(basic_string_view<charT, traits> sv);
		template<class T>
		basic_string& assign(const T& t, size_type pos, size_type n = npos);
		basic_string& assign(const charT* s, size_type n);
		basic_string& assign(const charT* s);
		basic_string& assign(size_type n, charT c);
		template<class InputIterator>
		basic_string& assign(InputIterator first, InputIterator last);
		basic_string& assign(initializer_list<charT>);

		basic_string& insert(size_type pos, basic_string_view<charT, traits> sv);
		template<class T>
		basic_string& insert(size_type pos1, const T& t, size_type pos2, size_type n = npos);
		basic_string& insert(size_type pos, const charT* s, size_type n);
		basic_string& insert(size_type pos, const charT* s);
		basic_string& insert(size_type pos, size_type n, charT c);
		iterator insert(const_iterator p, charT c);
		iterator insert(const_iterator p, size_type n, charT c);
		template<class InputIterator>
		iterator insert(const_iterator p, InputIterator first, InputIterator last);
		iterator insert(const_iterator p, initializer_list<charT>);
		basic_string& erase(size_type pos = 0, size_type n = npos);
		iterator erase(const_iterator p);
		iterator erase(const_iterator first, const_iterator last);
		void pop_back();
		basic_string& replace(size_type pos1, size_type n1, const basic_string& str);
		basic_string& replace(size_type pos1, size_type n1, const basic_string& str,
		size_type pos2, size_type n2 = npos);
		basic_string& replace(size_type pos1, size_type n1, basic_string_view<charT, traits> sv);
		template<class T>
		basic_string& replace(size_type pos1, size_type n1, const T& t,
		size_type pos2, size_type n2 = npos);
		basic_string& replace(size_type pos, size_type n1, const charT* s, size_type n2);
		basic_string& replace(size_type pos, size_type n1, const charT* s);
		basic_string& replace(size_type pos, size_type n1, size_type n2, charT c);
		basic_string& replace(const_iterator i1, const_iterator i2, const basic_string& str);
		basic_string& replace(const_iterator i1, const_iterator i2,
		basic_string_view<charT, traits> sv);
		basic_string& replace(const_iterator i1, const_iterator i2, const charT* s, size_type n);
		basic_string& replace(const_iterator i1, const_iterator i2, const charT* s);
		basic_string& replace(const_iterator i1, const_iterator i2, size_type n, charT c);
		template<class InputIterator>
		basic_string& replace(const_iterator i1, const_iterator i2,
		InputIterator j1, InputIterator j2);
		basic_string& replace(const_iterator, const_iterator, initializer_list<charT>);
		size_type copy(charT* s, size_type n, size_type pos = 0) const;
		void swap(basic_string& str)
		noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value ||
		allocator_traits<Allocator>::is_always_equal::value);

		// 24.3.2.7, string operations
		const charT* c_str() const noexcept;
		const charT* data() const noexcept;
		charT* data() noexcept;
		operator basic_string_view<charT, traits>() const noexcept;
		allocator_type get_allocator() const noexcept;
		size_type find (basic_string_view<charT, traits> sv, size_type pos = 0) const noexcept;
		size_type find (const basic_string& str, size_type pos = 0) const noexcept;
		size_type find (const charT* s, size_type pos, size_type n) const;
		size_type find (const charT* s, size_type pos = 0) const;
		size_type find (charT c, size_type pos = 0) const;
		size_type rfind(basic_string_view<charT, traits> sv, size_type pos = npos) const noexcept;
		size_type rfind(const basic_string& str, size_type pos = npos) const noexcept;
		size_type rfind(const charT* s, size_type pos, size_type n) const;
		size_type rfind(const charT* s, size_type pos = npos) const;
		size_type rfind(charT c, size_type pos = npos) const;
		size_type find_first_of(basic_string_view<charT, traits> sv,
		size_type pos = 0) const noexcept;
		size_type find_first_of(const basic_string& str, size_type pos = 0) const noexcept;
		size_type find_first_of(const charT* s, size_type pos, size_type n) const;
		size_type find_first_of(const charT* s, size_type pos = 0) const;
		size_type find_first_of(charT c, size_type pos = 0) const;
		size_type find_last_of (basic_string_view<charT, traits> sv,
		size_type pos = npos) const noexcept;
		size_type find_last_of (const basic_string& str, size_type pos = npos) const noexcept;
		size_type find_last_of (const charT* s, size_type pos, size_type n) const;
		size_type find_last_of (const charT* s, size_type pos = npos) const;
		size_type find_last_of (charT c, size_type pos = npos) const;
		size_type find_first_not_of(basic_string_view<charT, traits> sv,
		size_type pos = 0) const noexcept;
		size_type find_first_not_of(const basic_string& str, size_type pos = 0) const noexcept;
		size_type find_first_not_of(const charT* s, size_type pos, size_type n) const;
		size_type find_first_not_of(const charT* s, size_type pos = 0) const;
		size_type find_first_not_of(charT c, size_type pos = 0) const;
		size_type find_last_not_of (basic_string_view<charT, traits> sv,
		size_type pos = npos) const noexcept;
		size_type find_last_not_of (const basic_string& str, size_type pos = npos) const noexcept;
		size_type find_last_not_of (const charT* s, size_type pos, size_type n) const;
		size_type find_last_not_of (const charT* s, size_type pos = npos) const;
		size_type find_last_not_of (charT c, size_type pos = npos) const;
		basic_string substr(size_type pos = 0, size_type n = npos) const;
		int compare(basic_string_view<charT, traits> sv) const noexcept;
		int compare(size_type pos1, size_type n1, basic_string_view<charT, traits> sv) const;
		template<class T>
		int compare(size_type pos1, size_type n1, const T& t,
		size_type pos2, size_type n2 = npos) const;
		int compare(const basic_string& str) const noexcept;
		int compare(size_type pos1, size_type n1, const basic_string& str) const;
		int compare(size_type pos1, size_type n1, const basic_string& str,
		size_type pos2, size_type n2 = npos) const;
		int compare(const charT* s) const;
		int compare(size_type pos1, size_type n1, const charT* s) const;
		int compare(size_type pos1, size_type n1, const charT* s, size_type n2) const;
		bool starts_with(basic_string_view<charT, traits> x) const noexcept;
		bool starts_with(charT x) const noexcept;
		bool starts_with(const charT* x) const;
		bool ends_with(basic_string_view<charT, traits> x) const noexcept;
		bool ends_with(charT x) const noexcept;
		bool ends_with(const charT* x) const;
	};

	template<class InputIterator,
	class Allocator = allocator<typename iterator_traits<InputIterator>::value_type>>
	basic_string(InputIterator, InputIterator, Allocator = Allocator())
	-> basic_string<typename iterator_traits<InputIterator>::value_type,
	char_traits<typename iterator_traits<InputIterator>::value_type>,
	Allocator>;
}
#endif
namespace std {
#if 0
	template<class charT, class traits = char_traits<charT>,
	class Allocator = allocator<charT>>
	class basic_string {
	public:
#if 0
		// types
		using traits_type = traits;
		using value_type = charT;
		using allocator_type = Allocator;
		using size_type = typename allocator_traits<Allocator>::size_type;
		using difference_type = typename allocator_traits<Allocator>::difference_type;
		using pointer = typename allocator_traits<Allocator>::pointer;
		using const_pointer = typename allocator_traits<Allocator>::const_pointer;
		using reference = value_type&;
		using const_reference = const value_type&;
		using iterator = implementation-defined ; // see 26.2
		using const_iterator = implementation-defined ; // see 26.2
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		static const size_type npos = -1;
#endif

		// 24.3.2.2, construct/copy/destroy
		basic_string() noexcept(noexcept(Allocator())) : basic_string(Allocator()) { }
		explicit basic_string(const Allocator& a) _NOEXCEPT;
		basic_string(const basic_string& str);
		basic_string(basic_string&& str) _NOEXCEPT;
		basic_string(const basic_string& str, size_type pos, const Allocator& a = Allocator());
		basic_string(const basic_string& str, size_type pos, size_type n,
		const Allocator& a = Allocator());
		template<class T>
		basic_string(const T& t, size_type pos, size_type n, const Allocator& a = Allocator());
		explicit basic_string(basic_string_view<charT, traits> sv, const Allocator& a = Allocator());
		basic_string(const charT* s, size_type n, const Allocator& a = Allocator());
		basic_string(const charT* s, const Allocator& a = Allocator());
		basic_string(size_type n, charT c, const Allocator& a = Allocator());
		template<class InputIterator>
		basic_string(InputIterator begin, InputIterator end, const Allocator& a = Allocator());
		basic_string(initializer_list<charT>, const Allocator& = Allocator());
		basic_string(const basic_string&, const Allocator&);
		basic_string(basic_string&&, const Allocator&);
		~basic_string();
		basic_string& operator=(const basic_string& str);
		basic_string& operator=(basic_string&& str)
		_NOEXCEPT(allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		allocator_traits<Allocator>::is_always_equal::value);
		basic_string& operator=(basic_string_view<charT, traits> sv);
		basic_string& operator=(const charT* s);
		basic_string& operator=(charT c);
		basic_string& operator=(initializer_list<charT>);

		// 24.3.2.3, iterators
#if 0
		iterator begin() _NOEXCEPT;
		const_iterator begin() const _NOEXCEPT;
		iterator end() _NOEXCEPT;
		const_iterator end() const _NOEXCEPT;
		reverse_iterator rbegin() _NOEXCEPT;
		const_reverse_iterator rbegin() const _NOEXCEPT;
		reverse_iterator rend() _NOEXCEPT;
		const_reverse_iterator rend() const _NOEXCEPT;
		const_iterator cbegin() const _NOEXCEPT;
		const_iterator cend() const _NOEXCEPT;
		const_reverse_iterator crbegin() const _NOEXCEPT;
		const_reverse_iterator crend() const _NOEXCEPT;
#endif

		// 24.3.2.4, capacity
#if 0
		size_type size() const _NOEXCEPT;
		size_type length() const _NOEXCEPT;
		size_type max_size() const _NOEXCEPT;
		void resize(size_type n, charT c);
		void resize(size_type n);
		size_type capacity() const _NOEXCEPT;
		void reserve(size_type res_arg = 0);
		void shrink_to_fit();
		void clear() _NOEXCEPT;
		[[nodiscard]] bool empty() const _NOEXCEPT;
#endif

		// 24.3.2.5, element access
#if 0
		const_reference operator[](size_type pos) const;
		reference operator[](size_type pos);
		const_reference at(size_type n) const;
		reference at(size_type n);
		const charT& front() const;
		charT& front();
		const charT& back() const;
		charT& back();
#endif

		// 24.3.2.6, modifiers
#if 0
		basic_string& operator+=(const basic_string& str);
		basic_string& operator+=(basic_string_view<charT, traits> sv);
		basic_string& operator+=(const charT* s);
		basic_string& operator+=(charT c);
		basic_string& operator+=(initializer_list<charT>);
		basic_string& append(const basic_string& str);
		basic_string& append(const basic_string& str, size_type pos, size_type n = npos);
		basic_string& append(basic_string_view<charT, traits> sv);
		template<class T>
		basic_string& append(const T& t, size_type pos, size_type n = npos);
		basic_string& append(const charT* s, size_type n);
		basic_string& append(const charT* s);
		basic_string& append(size_type n, charT c);
		template<class InputIterator>
		basic_string& append(InputIterator first, InputIterator last);
		basic_string& append(initializer_list<charT>);
		void push_back(charT c);
		basic_string& assign(const basic_string& str);
		basic_string& assign(basic_string&& str)
		_NOEXCEPT(allocator_traits<Allocator>::propagate_on_container_move_assignment::value ||
		allocator_traits<Allocator>::is_always_equal::value);
		basic_string& assign(const basic_string& str, size_type pos, size_type n = npos);
		basic_string& assign(basic_string_view<charT, traits> sv);
		template<class T>
		basic_string& assign(const T& t, size_type pos, size_type n = npos);
		basic_string& assign(const charT* s, size_type n);
		basic_string& assign(const charT* s);
		basic_string& assign(size_type n, charT c);
		template<class InputIterator>
		basic_string& assign(InputIterator first, InputIterator last);
		basic_string& assign(initializer_list<charT>);

		basic_string& insert(size_type pos, basic_string_view<charT, traits> sv);
		template<class T>
		basic_string& insert(size_type pos1, const T& t, size_type pos2, size_type n = npos);
		basic_string& insert(size_type pos, const charT* s, size_type n);
		basic_string& insert(size_type pos, const charT* s);
		basic_string& insert(size_type pos, size_type n, charT c);
		iterator insert(const_iterator p, charT c);
		iterator insert(const_iterator p, size_type n, charT c);
		template<class InputIterator>
		iterator insert(const_iterator p, InputIterator first, InputIterator last);
		iterator insert(const_iterator p, initializer_list<charT>);
		basic_string& erase(size_type pos = 0, size_type n = npos);
		iterator erase(const_iterator p);
		iterator erase(const_iterator first, const_iterator last);
		void pop_back();
		basic_string& replace(size_type pos1, size_type n1, const basic_string& str);
		basic_string& replace(size_type pos1, size_type n1, const basic_string& str,
		size_type pos2, size_type n2 = npos);
		basic_string& replace(size_type pos1, size_type n1, basic_string_view<charT, traits> sv);
		template<class T>
		basic_string& replace(size_type pos1, size_type n1, const T& t,
		size_type pos2, size_type n2 = npos);
		basic_string& replace(size_type pos, size_type n1, const charT* s, size_type n2);
		basic_string& replace(size_type pos, size_type n1, const charT* s);
		basic_string& replace(size_type pos, size_type n1, size_type n2, charT c);
		basic_string& replace(const_iterator i1, const_iterator i2, const basic_string& str);
		basic_string& replace(const_iterator i1, const_iterator i2,
		basic_string_view<charT, traits> sv);
		basic_string& replace(const_iterator i1, const_iterator i2, const charT* s, size_type n);
		basic_string& replace(const_iterator i1, const_iterator i2, const charT* s);
		basic_string& replace(const_iterator i1, const_iterator i2, size_type n, charT c);
		template<class InputIterator>
		basic_string& replace(const_iterator i1, const_iterator i2,
		InputIterator j1, InputIterator j2);
		basic_string& replace(const_iterator, const_iterator, initializer_list<charT>);
		size_type copy(charT* s, size_type n, size_type pos = 0) const;
		void swap(basic_string& str)
		_NOEXCEPT(allocator_traits<Allocator>::propagate_on_container_swap::value ||
		allocator_traits<Allocator>::is_always_equal::value);
#endif
		// 24.3.2.7, string operations
#if 0
		const charT* c_str() const _NOEXCEPT;
		const charT* data() const _NOEXCEPT;
		charT* data() _NOEXCEPT;
		operator basic_string_view<charT, traits>() const _NOEXCEPT;
		allocator_type get_allocator() const _NOEXCEPT;
		size_type find (basic_string_view<charT, traits> sv, size_type pos = 0) const _NOEXCEPT;
		size_type find (const basic_string& str, size_type pos = 0) const _NOEXCEPT;
		size_type find (const charT* s, size_type pos, size_type n) const;
		size_type find (const charT* s, size_type pos = 0) const;
		size_type find (charT c, size_type pos = 0) const;
		size_type rfind(basic_string_view<charT, traits> sv, size_type pos = npos) const _NOEXCEPT;
		size_type rfind(const basic_string& str, size_type pos = npos) const _NOEXCEPT;
		size_type rfind(const charT* s, size_type pos, size_type n) const;
		size_type rfind(const charT* s, size_type pos = npos) const;
		size_type rfind(charT c, size_type pos = npos) const;
		size_type find_first_of(basic_string_view<charT, traits> sv,
		size_type pos = 0) const _NOEXCEPT;
		size_type find_first_of(const basic_string& str, size_type pos = 0) const _NOEXCEPT;
		size_type find_first_of(const charT* s, size_type pos, size_type n) const;
		size_type find_first_of(const charT* s, size_type pos = 0) const;
		size_type find_first_of(charT c, size_type pos = 0) const;
		size_type find_last_of (basic_string_view<charT, traits> sv,
		size_type pos = npos) const _NOEXCEPT;
		size_type find_last_of (const basic_string& str, size_type pos = npos) const _NOEXCEPT;
		size_type find_last_of (const charT* s, size_type pos, size_type n) const;
		size_type find_last_of (const charT* s, size_type pos = npos) const;
		size_type find_last_of (charT c, size_type pos = npos) const;
		size_type find_first_not_of(basic_string_view<charT, traits> sv,
		size_type pos = 0) const _NOEXCEPT;
		size_type find_first_not_of(const basic_string& str, size_type pos = 0) const _NOEXCEPT;
		size_type find_first_not_of(const charT* s, size_type pos, size_type n) const;
		size_type find_first_not_of(const charT* s, size_type pos = 0) const;
		size_type find_first_not_of(charT c, size_type pos = 0) const;
		size_type find_last_not_of (basic_string_view<charT, traits> sv,
		size_type pos = npos) const _NOEXCEPT;
		size_type find_last_not_of (const basic_string& str, size_type pos = npos) const _NOEXCEPT;
		size_type find_last_not_of (const charT* s, size_type pos, size_type n) const;
		size_type find_last_not_of (const charT* s, size_type pos = npos) const;
		size_type find_last_not_of (charT c, size_type pos = npos) const;
		basic_string substr(size_type pos = 0, size_type n = npos) const;
		int compare(basic_string_view<charT, traits> sv) const _NOEXCEPT;
		int compare(size_type pos1, size_type n1, basic_string_view<charT, traits> sv) const;
		template<class T>
		int compare(size_type pos1, size_type n1, const T& t,
		size_type pos2, size_type n2 = npos) const;
		int compare(const basic_string& str) const _NOEXCEPT;
		int compare(size_type pos1, size_type n1, const basic_string& str) const;
		int compare(size_type pos1, size_type n1, const basic_string& str,
		size_type pos2, size_type n2 = npos) const;
		int compare(const charT* s) const;
		int compare(size_type pos1, size_type n1, const charT* s) const;
		int compare(size_type pos1, size_type n1, const charT* s, size_type n2) const;
		bool starts_with(basic_string_view<charT, traits> x) const _NOEXCEPT;
		bool starts_with(charT x) const _NOEXCEPT;
		bool starts_with(const charT* x) const;
		bool ends_with(basic_string_view<charT, traits> x) const _NOEXCEPT;
		bool ends_with(charT x) const _NOEXCEPT;
		bool ends_with(const charT* x) const;
#endif
	};

	template<class InputIterator,
	class Allocator = allocator<typename iterator_traits<InputIterator>::value_type>>
	basic_string(InputIterator, InputIterator, Allocator = Allocator())
	-> basic_string<typename iterator_traits<InputIterator>::value_type,
	char_traits<typename iterator_traits<InputIterator>::value_type>,
	Allocator>;
#endif
	// Extremely minimal implementation to satisfy ONLY the needs of future_category
	// base class, error_category.
	template<class charT, class traits = char_traits<charT>>
	class basic_string {
	public:
		basic_string() : basic_string(0) {}
		basic_string(const charT* p) : data_(p) {}
		const charT* c_str() const _NOEXCEPT {
			return data();
		}
		const charT* data() const _NOEXCEPT {
			return data_;
		}
		operator const charT*() const { return c_str(); }
	private:
		const charT* data_;
	};
	typedef basic_string<char> string;
}

#endif /* SOURCES_STL_STRING_H_ */
