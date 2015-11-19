#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#ifdef __GNUC__
#define COMMON_INLINE inline
#define COMMON_ALIGN(x)  __attribute__ ((aligned(x)))		/// Align to x byte boundaries. Must come AFTER the variable name.
#elif _MSC_VER
#define COMMON_INLINE inline
#define COMMON_ALIGN(x) __declspec(align(x))
#else
#error RendererLib::Unsupported compiler!
#endif // __GNUC__

template <class T>
class IsEqualTestable
{

public:

	bool operator==(const T& rhs)
	{
		return this == &rhs;
	}
};


#endif // COMMON_H_INCLUDED
