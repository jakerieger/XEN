#pragma once

#ifdef XENENGINE_CORE_EXPORTS
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif

#include "CoreTypes.h"