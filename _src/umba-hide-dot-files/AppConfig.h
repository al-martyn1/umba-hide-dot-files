#pragma once


#include "umba/umba.h"
//
#include "umba/string_plus.h"
//
#include "enums.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>


struct AppConfig
{
    Command       cmd = Command::none;
    std::string   aliasName;
    std::string   path;

}; // struct AppConfig

