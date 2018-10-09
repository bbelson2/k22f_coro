/*
 * stlcommon.h
 *
 *  Created on: 30 Aug 2018
 *      Author: belsonba
 */

#ifndef SOURCES_STLCOMMON_H_
#define SOURCES_STLCOMMON_H_


#pragma once

#ifndef _HAS_EXCEPTIONS
    #define _HAS_EXCEPTIONS 1 // Predefine as 0 to disable exceptions
#endif

 #if _HAS_EXCEPTIONS
  #define _NOEXCEPT	noexcept
  #define _NOEXCEPT_OP(x)	noexcept(x)
 #else /* _HAS_EXCEPTIONS */
  #define _NOEXCEPT	throw ()
  #define _NOEXCEPT_OP(x)
 #endif /* _HAS_EXCEPTIONS */

#ifndef _NOVTABLE
#ifdef __GNUC__
#define _NOVTABLE
#elif __clang__
#define _NOVTABLE
#else
#define _NOVTABLE __declspec(novtable)
#endif
#endif /* _NOVTABLE */

namespace std {

struct _Nil
	{	// empty struct, for unused argument types
	};

	// TEMPLATE CLASS integral_constant
template<class _Ty,
	_Ty _Val>
	struct integral_constant
	{	// convenient template for integral constant types
	static constexpr _Ty value = _Val;

	typedef _Ty value_type;
	typedef integral_constant<_Ty, _Val> type;

	constexpr operator value_type() const _NOEXCEPT
		{	// return stored value
		return (value);
		}

	constexpr value_type operator()() const _NOEXCEPT
		{	// return stored value
		return (value);
		}
	};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;



} // namespace std

#endif /* SOURCES_STLCOMMON_H_ */
