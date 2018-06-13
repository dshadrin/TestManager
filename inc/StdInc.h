/*
* Copyright (C) 2014-2017 Rhonda Software.
* All rights reserved.
*/

#ifndef __RPC_API_STORAGE_H
#define __RPC_API_STORAGE_H
#pragma once

//////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif // _MSC_VER

#ifdef MINGW
#ifndef __STRICT_ANSI__
#define __STRICT_ANSI__
#endif

#endif

//////////////////////////////////////////////////////////////////////////

#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <cstdint>

#if ((defined _MSC_VER) && (_MSC_VER >= 1800))
#include <filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

//////////////////////////////////////////////////////////////////////////
#endif // __RPC_API_STORAGE_H
