/*! \file
    \brief 
 */


// Должна быть первой
#include "umba/umba.h"
//---
#include "umba/app_main.h"
//
#include "umba/debug_helpers.h"
#include "umba/shellapi.h"
#include "umba/program_location.h"
#include "umba/cli_tool_helpers.h"
#include "umba/cmd_line.h"
#include "umba/filesys.h"
#include "umba/filesys_scanners.h"
//

//#-sort
#include "umba/simple_formatter.h"
#include "umba/char_writers.h"
//#+sort

#include "umba/debug_helpers.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <exception>
#include <stdexcept>

#include "umba/debug_helpers.h"
#include "umba/string_plus.h"
#include "umba/program_location.h"
#include "umba/scope_exec.h"
#include "umba/macro_helpers.h"
#include "umba/macros.h"
#include "umba/scanners.h"
#include "umba/relops.h"
#include "umba/debug_helpers.h"
#include "umba/rule_of_five.h"

#include "marty_cpp/marty_cpp.h"
#include "marty_cpp/marty_enum.h"
#include "marty_cpp/marty_flags.h"
#include "marty_cpp/sort_includes.h"
#include "marty_cpp/enums.h"
#include "marty_cpp/src_normalization.h"
#include "marty_cpp/marty_ns.h"
#include "marty_cpp/marty_enum_impl_helpers.h"

#include "encoding/encoding.h"
#include "umba/cli_tool_helpers.h"
#include "umba/time_service.h"
#include "umba/shellapi.h"

//
// #include "utils.h"
//


umba::StdStreamCharWriter coutWriter(std::cout);
umba::StdStreamCharWriter cerrWriter(std::cerr);
umba::NulCharWriter       nulWriter;

umba::SimpleFormatter umbaLogStreamErr(&cerrWriter);
umba::SimpleFormatter umbaLogStreamMsg(&coutWriter);
umba::SimpleFormatter umbaLogStreamNul(&nulWriter);

bool umbaLogGccFormat   = false; // true;
bool umbaLogSourceInfo  = false;

bool bOverwrite         = false;

//
#include "log.h"
//
#include "AppConfig.h"

AppConfig appConfig;

#include "ArgParser.h"



int unsafeMain(int argc, char* argv[]);


UMBA_APP_MAIN()
{
    try
    {
        return unsafeMain(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        std::cout << "Unknown error\n";
        return 2;
    }

}



int unsafeMain(int argc, char* argv[])
{

    UMBA_USED(argc);
    UMBA_USED(argv);

    auto argsParser = umba::command_line::makeArgsParser( ArgParser<std::string>()
                                                        , CommandLineOptionCollector()
                                                        , argc, argv
                                                        , umba::program_location::getProgramLocation
                                                            ( argc, argv
                                                            , false // useUserFolder = false
                                                            //, "" // overrideExeName
                                                            )
                                                        );

    // Force set CLI arguments while running under debugger
    if (umba::isDebuggerPresent())
    {
        std::string cwd;
        std::string rootPath = umba::shellapi::getDebugAppRootFolder(&cwd);
        std::cout << "App Root Path: " << rootPath << "\n";
        std::cout << "Working Dir  : " << cwd << "\n";

        argsParser.args.clear();
        argsParser.args.push_back("--hide");
        argsParser.args.push_back(rootPath);

    } // if (umba::isDebuggerPresent())


    // try
    // {
        // Job completed - may be, --where option found
        if (argsParser.mustExit)
            return 0;

        // if (!argsParser.quet)
        // {
        //     printNameVersion();
        // }

        // LOG_INFO("config") << "-----------------------------------------" << "\n";
        // LOG_INFO("config") << "Processing builtin option files\n";
        if (!argsParser.parseStdBuiltins())
        {
            // LOG_INFO("config") << "Error found in builtin option files\n";
            return 1;
        }
        // LOG_INFO("config") << "-----------------------------------------" << "\n";

        if (argsParser.mustExit)
            return 0;

        // LOG_INFO("config") << "-----------------------------------------" << "\n";
        // LOG_INFO("config") << "Processing command line arguments\n";
        if (!argsParser.parse())
        {
            // LOG_INFO("config") << "Error found while parsing command line arguments\n";
            return 1;
        }
        // LOG_INFO("config") << "-----------------------------------------" << "\n";

        if (argsParser.mustExit)
            return 0;
    // }
    // catch(const std::exception &e)
    // {
    //     LOG_ERR << e.what() << "\n";
    //     return -1;
    // }
    // catch(const std::exception &e)
    // {
    //     LOG_ERR << "command line arguments parsing error" << "\n";
    //     return -1;
    // }

    appConfig.finalizeCommands();

    if (!argsParser.quet  /* && !hasHelpOption */ )
    {
        //printNameVersion();
        //LOG_MSG<<"\n";
        umba::cli_tool_helpers::printNameVersion(umbaLogStreamMsg);
    }


    // На самом деле делаем выполнение не одной команды, а пакетный режим.

    if (appConfig.commands.empty())
    {
        LOG_ERR<<"No commands taken, nothing to do."<<"\n";
    }


    for(auto c : appConfig.commands)
    {
        if (c.cmd==Command::hide || c.cmd==Command::unhide)
        {

#if defined(WIN32) || defined(_WIN32)

            c.checkUpdatePath();

            std::vector<std::string> rootScanPaths; rootScanPaths.emplace_back(c.path);
            std::vector<std::string> foundFiles   ;
            std::vector<std::string> excludedFiles;
            std::set<std::string>    foundExtentions;

            umba::filesys::scanners::scanFolders( rootScanPaths
                                                , std::vector<std::string>() // includeFilesMaskList
                                                , std::vector<std::string>() // excludeFilesMaskList
                                                , umbaLogStreamNul
                                                , foundFiles   
                                                , excludedFiles
                                                , foundExtentions
                                                , (std::vector<std::string>*)0 // pFoundFilesRootFolders
                                                , std::vector<std::string>() // excludeFoldersExact
                                                , c.recurse
                                                , false // logFoundHeader
                                                , true  // addFolders
                                                );
            if (c.dotChar==0)
                c.dotChar = '.';

            bool bHide = c.cmd==Command::hide;

            for(auto && fname : foundFiles)
            {
                auto nameOnly = umba::filename::getFileName(fname);
                if (nameOnly.empty())
                    continue;

                if (nameOnly[0]!=c.dotChar)
                    continue;

                bool res = umba::shellapi::win32::fileAttributeHiddenSet(fname, bHide);
                if (!argsParser.quet)
                    umbaLogStreamMsg << (bHide ? "Hide" : "Unhide") << " " << fname << " - " << (res ? "Ok" : "Failed") << "\n";

            } // for(auto && fname : foundFiles)

#else

            if (!argsParser.quet)
                umbaLogStreamMsg << "Hide/unhide commands supported only in Windows\n";

#endif

        } // if (c.cmd==Command::hide || c.cmd==Command::unhide)

        else if (c.cmd==Command::shellHide || c.cmd==Command::shellUnhide)
        {

#if defined(WIN32) || defined(_WIN32)

            bool bHide = c.cmd==Command::shellHide;
            bool res = umba::shellapi::win32::shellParamShowHiddenFilesSet(!bHide);
            if (!argsParser.quet)
                umbaLogStreamMsg << (bHide ? "Hide" : "Unhide") << " hidden files in shell - " << (res ? "Ok" : "Failed") << "\n";

#else

            if (!argsParser.quet)
                umbaLogStreamMsg << "Shell hide/unhide commands supported only in Windows\n";

#endif

        } // else if (appConfig.cmd==Command::shellHide || appConfig.cmd==Command::shellUnhide)

        else if (c.cmd==Command::open)
        {

#if defined(WIN32) || defined(_WIN32)

            c.checkUpdatePath();
            auto path = umba::filename::makeCanonical(c.path);
            bool res = umba::shellapi::executeOpen(path);
            if (!argsParser.quet)
                umbaLogStreamMsg << "Open folder '" << c.path << "' in shell - " << (res ? "Ok" : "Failed") << "\n";

#else

            if (!argsParser.quet)
                umbaLogStreamMsg << "Open folder in shell command supported only in Windows\n";

#endif

        } // else if (appConfig.cmd==open)

    }

#if 0
    if (appConfig.path.empty())
    {
        appConfig.path = umba::filesys::getCurrentDirectory();
    }


    if (appConfig.cmd==hide || appConfig.cmd==unhide)
    {
        bool bHide = appConfig.cmd==hide;

        #if defined(WIN32) || defined(_WIN32)


        #endif

// umba::filesys::scanners::
// template<typename StringType, typename LogMsgType> inline
// void scanFolders( const std::vector<StringType> &rootScanPaths
//                 , const std::vector<StringType> &includeFilesMaskList
//                 , const std::vector<StringType> &excludeFilesMaskList
//                 , LogMsgType                    &logMsg           // logMsg or logNul
//                 , std::vector<StringType>       &foundFiles
//                 , std::vector<StringType>       &excludedFiles
//                 , std::set<StringType>          &foundExtentions
//                 , std::vector<StringType>       *pFoundFilesRootFolders = 0
//                 , const std::vector<StringType> &excludeFoldersExact = std::vector<StringType>()
//                 , bool                          scanRecurse    = true
//                 , bool                          logFoundHeader = true
//                 )

// umba::shellapi::win32::
// bool fileAttributeHiddenSet(const std::string &fname, bool bSet)
// shellParamShowHiddenFilesSet(bool bShow)

        //
    }
    else if (appConfig.cmd==shellHide || appConfig.cmd==shellUnhide)
    {
        #if defined(WIN32) || defined(_WIN32)
        bool bHide = appConfig.cmd==shellHide;
        // https://superuser.com/questions/1151844/how-to-toggle-show-hide-hidden-files-in-windows-through-command-line
        // HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced
        // Show: reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced" /v Hidden /t REG_DWORD /d 1 /f
        // Don't show: reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced" /v Hidden /t REG_DWORD /d 2 /f

        // https://stackoverflow.com/questions/2514523/api-to-toggle-show-hidden-files-folders-and-drives
        // SendNotifyMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, 0);
        // or 
        // RefreshPolicyEx(False, RP_FORCE);
        // or 
        // SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, integer(pchar('Policy')), SMTO_NORMAL or SMTO_ABORTIFHUNG, 5000, c1);

        // SHGetSetSettings
        // https://learn.microsoft.com/ru-ru/windows/win32/api/shlobj_core/nf-shlobj_core-shgetsetsettings
        // https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetsetsettings
        #endif
    }
    else if (appConfig.cmd==open)
    {
    
    }
    else if (appConfig.cmd==makeAlias)
    {
    
    }
#endif

    return 0;
}

