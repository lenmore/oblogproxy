/**
 * Copyright (c) 2024 OceanBase
 * OceanBase Migration Service LogProxy is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#include "sink/rename_table_column_object_builder.h"

#include "object/rename_table_column_object.h"
#include "sink/sql_builder_util.h"
namespace etransfer {
namespace sink {
Strings RenameTableColumnObjectBuilder::RealBuildSql(
    ObjectPtr db_object, ObjectPtr parent_object,
    std::shared_ptr<BuildContext> sql_builder_context) {
  std::shared_ptr<RenameTableColumnObject> rename_table_column_object =
      std::dynamic_pointer_cast<RenameTableColumnObject>(db_object);
  Strings res;
  std::string line;
  line.append("RENAME COLUMN ");
  line.append(SqlBuilderUtil::EscapeNormalObjectName(
          rename_table_column_object->GetOriginColumnName(), sql_builder_context))
      .append(" TO ");
  line.append(SqlBuilderUtil::EscapeNormalObjectName(
      rename_table_column_object->GetCurrentColumnName(), sql_builder_context));
  res.push_back(line);
  return res;
}
}  // namespace sink

}  // namespace etransfer
