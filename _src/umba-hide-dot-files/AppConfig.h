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
#include <vector>


struct CommandInfo
{
    Command       cmd     = Command::none;
    bool          recurse = false;
    std::string   path;

};

struct AppConfig
{
    std::vector<CommandInfo>       commands  ;
    CommandInfo                    curCommand;

    /*
        Добавляем команду в очередь только при получении новой команды,
        или после окончания обработки аргументов командной строки, если текущая команда не Command::none.

        При добавлении команды всё обнуляем в текущей, ставим cmd = Command::none
     */


    bool startNewCommand(Command c)
    {
        // Проверку валидности команд сделаем перед обработкой
        commands.emplace_back(curCommand);
        curCommand = CommandInfo{};
    }

    bool finalizeCommands()
    {
        if (curCommand.cmd==Command::none)
        {
            return curCommand.path.empty();
        }

        commands.emplace_back(curCommand);
        curCommand = CommandInfo{};
        return true;
    }

    bool setPath(const std::string &p)
    {
        if (curCommand.cmd==Command::none)
            return false;

        if (!curCommand.path.empty())
            return false;

        curCommand.path = p;

        return true;
    }


}; // struct AppConfig



