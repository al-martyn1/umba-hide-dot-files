#pragma once

#include "marty_cpp/marty_enum.h"

#include <exception>
#include <map>
#include <stdexcept>
#include <string>
#include <unordered_map>




//#!Command
enum class Command : std::uint32_t
{
invalid       = (std::uint32_t)(-1) /*!<  */,
unknown       = (std::uint32_t)(-1) /*!<  */,
none          = 0x0000 /*!<  */,
hide          = 0x0001 /*!<  */,
unhide        = 0x0002 /*!<  */,
open          = 0x0003 /*!<  */,
shellHide     = 0x0004 /*!<  */,
shellUnhide   = 0x0005 /*!<  */,
makeAlias     = 0x0006 /*!<  */

}; // enum 
//#!

MARTY_CPP_MAKE_ENUM_IS_FLAGS_FOR_NON_FLAGS_ENUM(Command)

MARTY_CPP_ENUM_CLASS_SERIALIZE_BEGIN( Command, std::map, 1 )
MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::shellHide     , "ShellHide"   );
MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::open          , "Open"        );
MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::unhide        , "Unhide"      );
MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::hide          , "Hide"        );
MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::none          , "None"        );
MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::makeAlias     , "MakeAlias"   );
MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::shellUnhide   , "ShellUnhide" );
MARTY_CPP_ENUM_CLASS_SERIALIZE_ITEM( Command::invalid       , "Invalid"     );
MARTY_CPP_ENUM_CLASS_SERIALIZE_END( Command, std::map, 1 )

MARTY_CPP_ENUM_CLASS_DESERIALIZE_BEGIN( Command, std::map, 1 )
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::shellHide     , "shell-hide"   );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::shellHide     , "shellhide"    );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::shellHide     , "shell_hide"   );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::open          , "open"         );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::unhide        , "unhide"       );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::hide          , "hide"         );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::none          , "none"         );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::makeAlias     , "make-alias"   );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::makeAlias     , "make_alias"   );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::makeAlias     , "makealias"    );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::shellUnhide   , "shell-unhide" );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::shellUnhide   , "shell_unhide" );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::shellUnhide   , "shellunhide"  );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::invalid       , "unknown"      );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_ITEM( Command::invalid       , "invalid"      );
MARTY_CPP_ENUM_CLASS_DESERIALIZE_END( Command, std::map, 1 )


