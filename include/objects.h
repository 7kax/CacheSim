#pragma once

#include "Config.h"
#include "IO.h"
#include "cache.h"
#include "general.h"
#include "log.h"

namespace CacheSim {
extern configManager cManager;
extern IOManager iManager;
extern logger mLogger;
extern cache mCache;
extern top TOP;
} // namespace CacheSim