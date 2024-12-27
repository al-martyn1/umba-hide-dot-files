#pragma once


#include "umba/umba.h"
//
#include "umba/string_plus.h"
#include "umba/filesys.h"
//
#include "enums.h"

//
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <umba/the.h>


struct CommandInfo
{
    Command       cmd     = Command::none;
    bool          recurse = false;
    char          dotChar = '.';
    std::string   path;

    void checkUpdatePath()
    {
        if (path.empty())
        {
            path = umba::filesys::getCurrentDirectory();
        }
    }

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
        bool recurse = curCommand.recurse;
        commands.emplace_back(curCommand);
        curCommand = CommandInfo{};
        curCommand.cmd = c;
        curCommand.recurse = recurse;
        return true;
    }

    bool isCommandSet() const
    {
        return curCommand.cmd!=Command::none;
    }

    bool isPathSet() const
    {
        return !curCommand.path.empty();
    }

    bool finalizeCommands()
    {
        if (!isCommandSet())
        {
            return true; // curCommand.path.empty();
        }

        commands.emplace_back(curCommand);
        curCommand = CommandInfo{};
        return true;
    }

    bool setPath(const std::string &p)
    {
        if (!isCommandSet())
            return false;

        if (!curCommand.path.empty())
            return false;

        if (!umba::TheValue(curCommand.cmd).oneOf(Command::hide, Command::unhide, Command::open))
        {
            return false;
        }

        curCommand.path = p;

        return true;
    }

    bool setDotChar(char ch)
    {
        if (!isCommandSet())
            return false;

        if (!umba::TheValue(curCommand.cmd).oneOf(Command::hide, Command::unhide))
        {
            return false;
        }

        curCommand.dotChar = ch;

        return true;
    }

    bool setRecurse(bool bRecurse)
    {
        // if (!isCommandSet())
        //     return false;

        curCommand.recurse = bRecurse;
        return true;
    }




}; // struct AppConfig



