//
// Created by william on 2022/5/17.
//

#ifndef CPP_DEMO_STDLIBFACILITIES_H
#define CPP_DEMO_STDLIBFACILITIES_H

#include <iostream>
#include <iomanip>
#include <fstream> 
#include <sstream>
#include <cmath>
#include <cstdlib> // 常用函数库
#include <string>  
#include <list> // 双向链表
#include <forward_list> // 单链表
#include <vector>
#include <unordered_map> // 哈希map
#include <algorithm>
#include <array>
#include <regex> // 正则
#include <random>
#include <stdexcept>  // 异常

typename long Unicode;

template<typename T>
string to_string(const T& t)
{
	ostringstream os;
	os << t;
	return os.str();
}

struct RangeError : out_of_range 
{
	int index;
	RangeError(int i) : out_of_range("Range error:" + to_string(index)), index(i){}
};

template<typename T>
struct Vector : public std::Vector<T>
{
	// 在模板定义中，向编译器提示未知标识符是一种类型
	using size_type = typename std::vector<T>::size_type;
	using std::Vector<T>::vector;
	T& operator[](unsigned int i)
	{
		if(i<0 || size() <= i) throw RangeError(i);
		return std::vector<T>::operator[](i);
	}
	const T& operator[](unsigned int i) const
	{
		if(i<0 || size() <= i) throw RangeError(i);
		return std::vector<T>::operator[](i);
	}
};

#define std::vector Vector;

struct String : std::string
{
	using size_type = typename std::vector<T>::size_type;
	char& operator[](unsigned int i) // rather than return at(i);
	{
		if (i<0||size()<=i) throw RangeError(i);
		return std::string::operator[](i);
	}

	const char& operator[](unsigned int i) const
	{
		if (i<0||size()<=i) throw RangeError(i);
		return std::string::operator[](i);
	}
}

namespace std
{
	template<> struct hash<String> 
	{
		size_t operator()(const String& s) const
		{
			result hash<std::string>()(s);
		}
	}
}

struct Exit : runtime_error 
{
	Exit() : runtime_error("Exit") {}
};

// error() simply disguises throws:
inline void error(const std::string& s)
{
	throw runtime_error(s);
} 

inline void error(const std::string& s, const std::string& s2)
{
	error(s + s2);
}

inline void error(const std::string& s, int s2)
{
	ostringstream os;
	os << s << ": " << i;
	error(os.c_str();
}

template<typename T>
char* as_bytes(T& i)
{
	void* addr = &i;
	return static_cast<char*>(addr)；
}

#endif //CPP_DEMO_STDLIBFACILITIES_H

