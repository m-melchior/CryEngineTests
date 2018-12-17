/*
 *                         unlikely_vs_normal_if.cpp  -  description
 *                         ------------------------------------------
 *   begin                : written in 2018 by Michael Melchior
 *   copyright            : (c) 2018 by Michael Melchior
 *   description          : This application contains the functional chain of the assert used in the CryEngine LinuxLauncher
 *                        : along with some added comments and printouts and two definitions of the CRY_ASSERT_MESSAGE
 *                        : It runs NUMBER_OF_TRIES each for true and false on both versions and measures the time for execution
 *   license              : Refer to the license description enclosed to this project
                            and the CryEngine License (https://github.com/CRYTEK/CRYENGINE/blob/release/LICENSE.md)
 *
*/

/*  The source code was mainly inspired / taken from the CryEngine repository (https://github.com/CRYTEK/CRYENGINE)
 *  The purpose of this project is to get a deeper understanding of the architecutre and algorithms used in the CryEngine
 *  as well as doing performance tests and comparisons to other coding standards and examine the effectiveness
*/

// standard includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// for bench marking
#include <algorithm> 
#include <chrono> 
#include <iostream> 
using namespace std;
using namespace std::chrono;


// origin: Code/CryEngine/CryCommon/Compiler/GCCspecific.h
#define PREFAST_ASSUME(cond)

#define IF(condition, hint)    if (__builtin_expect(!!(condition), hint))
#define IF_UNLIKELY(condition) if (__builtin_expect(!!(condition), 0))
#define IF_LIKELY(condition)   if (__builtin_expect(!!(condition), 1))


// origin: Code/CryEngine/CryCommon/CryCore/Assert/CryAssert.h
struct SAssertData {
	char const* const szExpression;
	char const* const szFunction;
	char const* const szFile;
	long const line;
};

struct SAssertCond {
	bool bIgnoreAssert;
	bool bLogAssert;
};


// origin: Code/CryEngine/CryCommon/CryCore/Assert/CryAssert_impl.h
void CryAssertTrace(const char*, ...) {
	//printf("CryAssertTrace(const char*, ...)\n");

	// Empty on purpose
}


#if !CRY_PLATFORM_WINDOWS
void CryLogAssert(const char* _pszCondition, const char* _pszFile, unsigned int _uiLine, bool* _pbIgnore) {
	//printf("CryLogAssert(const char* _pszCondition, const char* _pszFile, unsigned int _uiLine, bool* _pbIgnore)\n");

	// Empty on purpose
}
#endif
// [..]


//! Check if assert is enabled (the same on every platform).
bool CryAssertIsEnabled() {
	//printf("CryAssertIsEnabled()\n");

	// commented out due to the complexity of queries on gEnv
	/*
	 #if defined(_DEBUG)
	 static const bool defaultIfUnknown = true;
	 #else
	 static const bool defaultIfUnknown = false;
	 #endif

	 const bool suppressGlobally = gEnv ? gEnv->cryAssertLevel == ECryAssertLevel::Disabled : !defaultIfUnknown;
	 const bool suppressedByUser = gEnv ? gEnv->ignoreAllAsserts : !defaultIfUnknown;
	 #ifdef eCryModule
	 const bool suppressedCurrentModule = gEnv && gEnv->pSystem ? !gEnv->pSystem->AreAssertsEnabledForModule(eCryModule) : !defaultIfUnknown;
	 #else
	 const bool suppressedCurrentModule = false;
	 #endif

	 return !(suppressGlobally || suppressedByUser || suppressedCurrentModule);
	 */

	return true;
}



bool CryAssert(const char*, const char*, unsigned int, bool*) {
	//printf("CryAssert(const char*, const char*, unsigned int, bool*)\n");
	
	// commented out due to ring-dependency
	//assert(false && \"CryAssert not implemented");
	return true;
}


//NO_INLINE
bool CryAssertHandler(SAssertData const& data, SAssertCond& cond) {
	//printf("CryAssertHandler(SAssertData const& data, SAssertCond& cond)\n");

	if (cond.bLogAssert) // Just log assert the first time
	{
		CryLogAssert(data.szExpression, data.szFile, data.line, &cond.bIgnoreAssert);
		cond.bLogAssert = false;
	}

	if (!cond.bIgnoreAssert && CryAssertIsEnabled()) // Don't show assert once it was ignored
			{
		if (CryAssert(data.szExpression, data.szFile, data.line, &cond.bIgnoreAssert))
			return true;
	}
	return false;
}

bool CryAssertHandler(SAssertData const& data, SAssertCond& cond, char const* const szMessage) {
	//printf("CryAssertHandler(SAssertData const& data, SAssertCond& cond, char const* const szMessage)\n");
	CryAssertTrace(szMessage);
	return CryAssertHandler(data, cond);
}



// origin: Code/CryEngine/CryCommon/CryCore/Assert/CryAssert.h
#define CRY_ASSERT_MESSAGE_WITH(condition, ...)                                 \
do                                                                       \
{                                                                        \
	IF_UNLIKELY (!(condition))                                             \
	{                                                                      \
		SAssertData const assertData =                             \
		{                                                                    \
			# condition,                                                       \
			__func__,                                                          \
			__FILE__,                                                          \
			__LINE__                                                           \
		};                                                                   \
		static SAssertCond assertCond =                            \
		{                                                                    \
			false, true                                                        \
		};                                                                   \
		if (CryAssertHandler(assertData, assertCond, __VA_ARGS__)) \
		; \
	}                                                                      \
	PREFAST_ASSUME(condition);                                         \
} while (false)


#define CRY_ASSERT_MESSAGE_WITHOUT(condition, ...)                                 \
do                                                                       \
{                                                                        \
	if (!(condition))                                             \
	{                                                                      \
		SAssertData const assertData =                             \
		{                                                                    \
			# condition,                                                       \
			__func__,                                                          \
			__FILE__,                                                          \
			__LINE__                                                           \
		};                                                                   \
		static SAssertCond assertCond =                            \
		{                                                                    \
			false, true                                                        \
		};                                                                   \
		if (CryAssertHandler(assertData, assertCond, __VA_ARGS__)) \
		; \
	}                                                                      \
} while (false)


#define CRY_ASSERT_WITH(condition) CRY_ASSERT_MESSAGE_WITH(condition, nullptr)
#define CRY_ASSERT_WITHOUT(condition) CRY_ASSERT_MESSAGE_WITHOUT(condition, nullptr)


#define NUMBER_OF_TRIES 1000000

void cryAssertWith() {
	printf("using cry_assert with IF_UNLIKELY\n");

	printf("true\n");

	auto start = high_resolution_clock::now();
	
	for (int i = 0; i < NUMBER_OF_TRIES; i++) {
		CRY_ASSERT_WITH(true);
	} // for (int i=0; i<NUMBER_OF_TRIES; i++) {
	
	auto stop = high_resolution_clock::now();
	
	auto durationTrue = duration_cast<microseconds>(stop - start);
	cout << "time: " << durationTrue.count() << " microseconds" << endl;

	printf("\n");

	printf("false\n");
	
	start = high_resolution_clock::now();
	
	for (int i = 0; i < NUMBER_OF_TRIES; i++) {
		CRY_ASSERT_WITH(false);
	} // for (int i=0; i<NUMBER_OF_TRIES; i++) {
	
	stop = high_resolution_clock::now();
	
	auto durationFalse = duration_cast<microseconds>(stop - start);
	cout << "time: " << durationFalse.count() << " microseconds" << endl;

	printf("\n");

	float durationRatio = (float) durationFalse.count() / (float) durationTrue.count();
	printf("ratio false / true: %f\n", durationRatio);	
}



void cryAssertWithout() {
	printf("using cry_assert without IF_UNLIKELY\n");

	auto start = high_resolution_clock::now();
	
	for (int i = 0; i < NUMBER_OF_TRIES; i++) {
		CRY_ASSERT_WITHOUT(true);
	} // for (int i=0; i<NUMBER_OF_TRIES; i++) {
	
	auto stop = high_resolution_clock::now();
	
	auto durationTrue = duration_cast<microseconds>(stop - start);
	cout << "true time: " << durationTrue.count() << " microseconds" << endl;


	printf("\n");

	
	start = high_resolution_clock::now();
	
	for (int i = 0; i < NUMBER_OF_TRIES; i++) {
		CRY_ASSERT_WITHOUT(false);
	} // for (int i=0; i<NUMBER_OF_TRIES; i++) {
	
	stop = high_resolution_clock::now();
	
	auto durationFalse = duration_cast<microseconds>(stop - start);
	cout << "false time: " << durationFalse.count() << " microseconds" << endl;

	printf("\n");

	float durationRatio = (float) durationFalse.count() / (float) durationTrue.count();
	printf("ratio false / true: %f\n", durationRatio);	
}




int main(int argc, char **argv) {
	cryAssertWith();
	
	printf("\n");
	printf("******************************************\n");
	printf("\n");
	
	cryAssertWithout();

	return 0;
} // int main(int argc, char **argv) {

