//
// Created by 花轻 on 2023/6/13.
//

#include "show_binary_logs.h"

// Note: Implementations of constructors and destructors can be found in statements.cpp.
namespace hsql {

ShowBinaryLogsStatement::ShowBinaryLogsStatement() : SQLStatement(COM_SHOW_BINLOGS)
{}

ShowBinaryLogsStatement::~ShowBinaryLogsStatement()
{}
}  // namespace hsql
