#pragma once//保证头文件不按重复包含
#include <SDKDDKver.h>//兼容以前的系统*/
#include <stdio.h>
#include <tchar.h>
// #include "resource.h//资源文件
#include <bitset>/*二进制*/
//#include <Afxwin. h>
//#include <afxwin.h>
#include <conio.h>/*需要在atlstr.h之前*/
//#include "atlstr,h"/*控制台使用CString夹, CcRect,TRACa函数等, debug工程运行库为多绒程(n)5为多线程DLL(/D) “无法使用"*/
//#include <atlimage. h>/*使用CiTmage,控制台使用CString类， TRACE函数等,debug工程运行库为"多线程Mn)威为“多线程DL《V/D)无法*/
#include <iostream>
//#includc<winSock2.h>/*windows网路编程,需要求windows.h之前√*/
//#includcWinhttp.h”
#define INITGUID/*必须在<Guiddef.h>之前,加载策略组*/
#include <Guiddef.h>//策略组.
#include <string>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
using namespace std;


#pragma once
#include<stdio.h>
#include <tchar.h>
#include<iostream>
#include <Windows.h>
#include <algorithm>
#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>
#include<cstring>

// C
#ifndef _GLIBCXX_NO_ASSERT
#include <cassert>
#endif
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#if __cplusplus >= 201103L
#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdalign>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwchar>
#include <cwctype>
#endif

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

#if __cplusplus >= 201103L
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#endif














