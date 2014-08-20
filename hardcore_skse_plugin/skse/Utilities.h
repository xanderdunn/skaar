#pragma once

// this has been tested to work for non-varargs functions
// varargs functions end up with 'this' passed as the last parameter (ie. probably broken)
// do NOT use with classes that have multiple inheritance

// if many member functions are to be declared, use MEMBER_FN_PREFIX to create a type with a known name
// so it doesn't need to be restated throughout the member list

// all of the weirdness with the _GetType function is because you can't declare a static const pointer
// inside the class definition. inlining automatically makes the function call go away since it's a const

#define MEMBER_FN_PREFIX(className)	\
	typedef className _MEMBER_FN_BASE_TYPE

#define DEFINE_MEMBER_FN_LONG(className, functionName, retnType, address, ...)		\
	typedef retnType (className::* _##functionName##_type)(__VA_ARGS__);			\
																					\
	inline _##functionName##_type * _##functionName##_GetPtr(void)					\
	{																				\
		static const UInt32 _address = address;										\
		return (_##functionName##_type *)&_address;									\
	}

#define DEFINE_MEMBER_FN(functionName, retnType, address, ...)	\
	DEFINE_MEMBER_FN_LONG(_MEMBER_FN_BASE_TYPE, functionName, retnType, address, __VA_ARGS__)

#define CALL_MEMBER_FN(obj, fn)	\
	((*(obj)).*(*((obj)->_##fn##_GetPtr())))

const std::string & GetRuntimeDirectory(void);
const std::string & GetConfigPath(void);
std::string GetConfigOption(const char * section, const char * key);
bool GetConfigOption_UInt32(const char * section, const char * key, UInt32 * dataOut);

// this is the solution to getting a pointer-to-member-function pointer
template <typename T>
UInt32 GetFnAddr(T src)
{
	union
	{
		UInt32	u32;
		T		t;
	} data;

	data.t = src;

	return data.u32;
}

const char * GetObjectClassName(void * objBase);
void DumpClass(void * theClassPtr, UInt32 nIntsToDump = 512);

const std::string & GetOSInfoStr(void);

// remove_pointer <T>::type is T with one level of indirection removed
// int ** -> int *
// int * -> int
// int -> int
template <class T>
struct remove_pointer
{
	typedef T	type;
};

template <class T>
struct remove_pointer <T *>
{
	typedef T	type;
};

class TESForm;

class FormMatcher
{
public:
	virtual bool Matches(TESForm* pForm) const = 0;
};

// alternative to strtok; doesn't modify src string, supports forward/backward iteration
class Tokenizer
{
public:
	Tokenizer(const char* src, const char* delims);
	~Tokenizer();

	// these return the offset of token in src, or -1 if no token
	UInt32 NextToken(std::string& outStr);
	UInt32 PrevToken(std::string& outStr);

private:
	std::string m_delims;
	size_t		m_offset;
	std::string m_data;
};

void * GetIATAddr(UInt8 * base, const char * searchDllName, const char * searchImportName);
