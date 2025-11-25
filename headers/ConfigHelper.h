#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <string>
#include <fstream>
#include "json.hpp"
#include "Config.h"

struct ConfigHelper {
    static bool loadConfig(const std::string& filename, Config& config);
};

#endif