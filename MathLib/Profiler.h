///------------------------------------------------------------------------------------------------
/// Profiler.h
///
/// Defines a system performance profiling counter class.
/// This system takes the easy way and uses #ifdefs to define OS specific code.
///
///------------------------------------------------------------------------------------------------

#ifndef PROFILER_H_INCLUDED
#define PROFILER_H_INCLUDED

#include <iostream>
#include <inttypes.h>

#ifdef _WIN32				/// Are we compiling on a Win32 system?
#include <windows.h>
#endif

/**
* Profiler:
* Data structure representing a high precision profiling utility.
*
* Maintenance:
* On deallocation, all objects of this type will automatically release any allocated memory,
* so no manual cleanup is required.
*/
class Profiler
{
	private:

#ifdef _WIN32
		/// <summary>The number of performance counter ticks per second.</summary>
		static int64_t performanceCounterFrequency;

		/// <summary>The starting time for the profiled segment.</summary>
		int64_t startTime;

		/// <summary>The ending time for the profiled segment.</summary>
		int64_t stopTime;
#endif

	public:

		Profiler()
		{
#ifdef _WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
			QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
#endif
		}

		/// <summary>
		/// Sets the start time for the performance counter.
		/// NOTE: You must call the stop() method before any valid results can be queried.
		/// </summary>
		void start()
		{
#ifdef _WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
#endif
		}

		/// <summary>
		/// Sets the stop time for the performance counter.
		/// NOTE: After this method is called, you can safely call the getTime* methods.
		/// </summary>
		void stop()
		{
#ifdef _WIN32
			QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
#endif
		}

		/// <summary>
		/// Returns the time interval between the start and stop times represented in seconds.
		/// </summary>
		float getTimeSeconds() const
		{
#ifdef _WIN32
			long double time	= ((long double)stopTime - (long double)startTime) /
								  (long double)performanceCounterFrequency;

			return ((float)time);
#endif
		}

		/// <summary>
		/// The time interval between the start and stop times represented in milliseconds.
		/// </summary>
		float getTimeMilliseconds() const
		{
#ifdef _WIN32
			long double time	= (long double)(stopTime - startTime) /
								  (long double)(performanceCounterFrequency);

			return ((float)time * 1000.0f);
#endif
		}

		/// <summary>
		/// Performs any initialization the profiler system needs. 
		/// Depending on the implementation, this may do a lot, or nothing at all.
		/// </summary>
		static void initializePerformanceCounters()
		{
#ifdef _WIN32
			BOOL result	= QueryPerformanceFrequency((LARGE_INTEGER*)&performanceCounterFrequency);
			if (!result)
			{
				std::cerr << "TProfiler::High performance counter not supported on this machine" << std::endl;
				std::cin.get();
				exit(1);
			}
#endif
		}
};

#endif // PROFILER_H_INCLUDED
