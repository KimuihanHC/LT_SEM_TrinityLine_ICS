#pragma once

namespace std
{
	template<typename Type> struct char_traits;
	template<typename Type> class allocator;
	template<typename Type> struct default_delete;

	template<typename Type, typename Traits, typename Allocator> class basic_string;
	template<typename Type, typename Traits, typename Allocator> class basic_istringstream;
	template<typename Type, typename Traits, typename Allocator> class basic_ostringstream;
	template<typename Type, typename Traits> class basic_ifstream;
	template<typename Type, typename Traits> class basic_ofstream;

	template<typename Type, typename Deleter> class unique_ptr;

	template<typename Type, typename Allocator> class vector;
	template<typename Type, typename Allocator> class deque;
	template<typename Type, typename Allocator> class list;
	template<typename Type, typename Container> class queue;

	template<typename Type1, typename Type2> struct pair;
	template<typename... Types> class tuple;

	template<typename Type> struct less;

	template<typename Key, typename Predicator, typename Allocator> class set;
	template<typename Key, typename Predicator, typename Allocator> class multiset;
	template<typename Key, typename Value, typename Predicator, typename Allocator> class map;
	template<typename Key, typename Value, typename Predicator, typename Allocator> class multimap;

	template<typename Type> class future;

	class thread;
	class mutex;
	class recursive_mutex;
	class timed_mutex;
	class recursive_timed_mutex;

	template <typename Mutex> class lock_guard;
	template <typename Mutex> class unique_lock;
};

namespace lt
{
	using int8 = __int8;
	using int16 = __int16;
	using int32 = __int32;
	using int64 = __int64;
	using uint8 = unsigned __int8;
	using uint16 = unsigned __int16;
	using uint32 = unsigned __int32;
	using uint64 = unsigned __int64;
	using uchar = unsigned char;
	using ushort = unsigned short;
	using uint = unsigned int;
	using ulong = unsigned long;
	using ulonglong = unsigned long long;

	#if defined(_WIN64)
	using intPtr = int64;
	using uintPtr = uint64;
	using longPtr = int64;
	using ulongPtr = uint64;
	#else
	using intPtr = int;
	using uintPtr = uint;
	using longPtr = long;
	using ulongPtr = ulong;
	#endif

	using SSize = tagSIZE;
	using SPoint = tagPOINT;
	using SRect = tagRECT;

#if defined(UNICODE)
	using tchar = wchar_t;
#else
	using tchar = char;
#endif

	using StringW = wchar_t *;
	using ConstStringW = const wchar_t *;

	using StringA = char *;
	using ConstStringA = const char *;

	using StringT = tchar *;
	using ConstStringT = const tchar *;

	template<typename Type,
			 typename CharTraits = std::char_traits<Type>,
			 typename Allocator = std::allocator<Type>>
	struct StdString
	{
		using String = std::basic_string<Type, CharTraits, Allocator>;
		using ISStream = std::basic_istringstream<Type, CharTraits, Allocator>;
		using OSStream = std::basic_ostringstream<Type, CharTraits, Allocator>;
		using IFStream = std::basic_ifstream<Type, CharTraits>;
		using OFStream = std::basic_ofstream<Type, CharTraits>;
	};

	using StdStringW = StdString<wchar_t>::String;
	using StdISStreamW = StdString<wchar_t>::ISStream;
	using StdOSStreamW = StdString<wchar_t>::OSStream;
	using StdIFStreamW = StdString<wchar_t>::IFStream;
	using StdOFStreamW = StdString<wchar_t>::OFStream;

	using StdStringA = StdString<char>::String;
	using StdISStreamA = StdString<char>::ISStream;
	using StdOSStreamA = StdString<char>::OSStream;
	using StdIFStreamA = StdString<char>::IFStream;
	using StdOFStreamA = StdString<char>::OFStream;

	using StdStringT = StdString<tchar>::String;
	using StdISStreamT = StdString<tchar>::ISStream;
	using StdOSStreamT = StdString<tchar>::OSStream;
	using StdIFStreamT = StdString<tchar>::IFStream;
	using StdOFStreamT = StdString<tchar>::OFStream;

	template<typename Type, typename Deleter = std::default_delete<Type>>
	using StdUniquePtr = std::unique_ptr<Type, Deleter>;

	template<typename Type1, typename Type2>
	using StdPair = std::pair<Type1, Type2>;

	template<typename... Types>
	using StdTuple = std::tuple<Types...>;

	template<typename Type,
			 typename Allocator = std::allocator<Type>>
	struct StdSequenceContainer
	{
		using Vector = std::vector<Type, Allocator>;
		using Deque = std::deque<Type, Allocator>;
		using List = std::list<Type, Allocator>;
		using Queue = std::queue<Type, Deque>;
	};

	template<typename Key,
			 typename Predicator = std::less<Key>,
			 typename Allocator = std::allocator<Key>>
	struct StdSetContainer
	{
		using Set = std::set<Key, Predicator, Allocator>;
		using MultiSet = std::multiset<Key, Predicator, Allocator>;
	};

	template<typename Key,
			 typename Value,
			 typename Predicator = std::less<Key>,
			 typename Allocator = std::allocator<std::pair<Key, Value>>>
	struct StdMapContainer
	{
		using Map = std::map<Key, Value, Predicator, Allocator>;
		using MultiMap = std::multimap<Key, Value, Predicator, Allocator>;
	};

	template<typename Type>
	using StdFuture = std::future<Type>;

	using StdThread = std::thread;
	using StdMutex = std::mutex;
	using StdRecursiveMutex = std::recursive_mutex;
	using StdTimedMutex = std::timed_mutex;
	using StdRecursiveTimedMutex = std::recursive_timed_mutex;

	template<typename Mutex = StdMutex>
	struct StdLocker
	{
		using SimpleLocker = std::lock_guard<Mutex>;
		using UniqueLocker = std::unique_lock<Mutex>;
	};
}

#if defined(UNICODE)
	#define __FILET__ __FILEW__
	#define __FUNCTIONT__ __FUNCTIONW__
#else
	#define __FILET__ __FILE__
	#define __FUNCTIONT__ __FUNCTION__
#endif
