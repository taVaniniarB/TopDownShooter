#pragma once

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <tchar.h>
#include <vector>
using std::vector;

#include <list>
using std::list;

#include <map>
using std::map;
using std::make_pair;

#include <string>
using std::string;
using std::wstring;

#include <math.h>
#include <assert.h>

#include <algorithm>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "Gdiplus.lib")


//vs 안에 있는 라이브러리 파일 절대경로로 입력할 필요 없이 자동으로 찾아줌
#pragma comment(lib, "Msimg32.lib")


#include "define.h"
#include "struct.h"
#include "func.h"