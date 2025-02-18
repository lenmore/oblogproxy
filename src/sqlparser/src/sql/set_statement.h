
#pragma once
#include "SQLStatement.h"

// Note: Implementations of constructors and destructors can be found in statements.cpp.
namespace hsql {

// Represents SQL SET statements.
struct SetStatement : SQLStatement {
  SetStatement();

  ~SetStatement() override;

  std::vector<SetClause*>* sets;
};

struct SetPasswordStatement : SQLStatement {
  SetPasswordStatement();
  ~SetPasswordStatement() override;

  char* user;
  char* password;
};

}  // namespace hsql
