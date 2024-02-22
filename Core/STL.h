#pragma once

#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/functional.h>
#include <EASTL/internal/function.h>
#include <EASTL/unordered_map.h>
#include <EASTL/map.h>
#include "Types.h"

#ifdef EASTL_DEBUGPARAMS_LEVEL
    #undef EASTL_DEBUGPARAMS_LEVEL
#endif

#define EASTL_DEBUGPARAMS_LEVEL 0