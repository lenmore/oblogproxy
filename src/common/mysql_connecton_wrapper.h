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
#pragma once

#include <string>
#include "model.h"
#include "conncpp.hpp"
#include "guard.hpp"
#include "timer.h"

namespace oceanbase::logproxy {

class MySQLConnection {
public:
  /*!
   * @brief Initialize database connection
   * @param ip
   * @param port
   * @param user
   * @param password
   * @param properties Connection parameters
   * @return
   */
  int connect(const std::string& ip, int port, std::string user, std::string password, std::string database,
      sql::Properties properties = sql::Properties());

  int connect(const std::string& uds, const std::string& user, const std::string& password,
      sql::Properties properties = sql::Properties());

  /*!
   * @brief Automatically release connection
   */
  virtual ~MySQLConnection();

  std::unique_ptr<sql::Connection> get_conn();

private:
  sql::mariadb::MariaDbDataSource data_source;
};

inline void log_slow_sql_if_exists(int64_t cost_time_us, const std::string& sql)
{
  if (cost_time_us > 1000000) {
    OMS_WARN("Slow sql, cost time: {}, sql: {}", cost_time_us, sql);
  }
}

int execute_update(MySQLConnection& mysql_conn, const std::string& sql, int32_t& affected_rows);

int execute_update(const std::unique_ptr<sql::Connection>& conn, const std::string& sql, int32_t& affected_rows);

sql::ResultSet* execute_query_with_retry(const std::shared_ptr<sql::PreparedStatement>& stmt);

int execute_update_with_retry(const std::shared_ptr<sql::PreparedStatement>& stmt, int32_t& affected_rows);

int execute_query_op(const std::shared_ptr<sql::PreparedStatement>& stmt, sql::ResultSet*& result_set);

int execute_update_op(const std::shared_ptr<sql::PreparedStatement>& stmt, int32_t& affected_rows);

template <class T>
using ResultSetConverter = std::function<void(const sql::ResultSet*, T&)>;

template <class T>
int execute_at_most_one_query(
    MySQLConnection& mysql_conn, const std::string& query_sql, T& object, ResultSetConverter<T>& converter)
{
  const auto conn = mysql_conn.get_conn();
  if (conn == nullptr) {
    OMS_ERROR("Failed to get connection.");
    return OMS_FAILED;
  }

  return execute_at_most_one_query(conn, query_sql, object, converter);
}

template <class T>
int execute_at_most_one_query(const std::unique_ptr<sql::Connection>& conn, const std::string& query_sql, T& object,
    ResultSetConverter<T>& converter)
{
  OMS_DEBUG("Exec sql: {}", query_sql);
  Timer timer;
  try {
    std::shared_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(query_sql));
    sql::ResultSet* res = execute_query_with_retry(stmt);
    if (res == nullptr) {
      OMS_ERROR("Query result set is null, sql: {}", query_sql);
      return OMS_FAILED;
    }

    defer(delete res);
    while (res->next()) {
      converter(res, object);
    }
  } catch (sql::SQLException& e) {
    OMS_ERROR("Failed to execute query, sql: {}, error: {}", query_sql, e.what());
    return OMS_FAILED;
  }
  log_slow_sql_if_exists(timer.elapsed(), query_sql);
  return OMS_OK;
}

template <class T>
int execute_multi_rows_query(MySQLConnection& mysql_conn, const std::string& query_sql, std::vector<T>& object_vec,
    ResultSetConverter<T>& converter)
{
  const auto conn = mysql_conn.get_conn();
  if (conn == nullptr) {
    OMS_ERROR("Failed to get connection.");
    return OMS_FAILED;
  }
  return execute_multi_rows_query(conn, query_sql, object_vec, converter);
}

template <class T>
int execute_multi_rows_query(const std::unique_ptr<sql::Connection>& conn, const std::string& query_sql,
    std::vector<T>& object_vec, ResultSetConverter<T>& converter)
{
  OMS_DEBUG("Exec sql: {}", query_sql);
  Timer timer;
  try {
    std::shared_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(query_sql));
    sql::ResultSet* res = execute_query_with_retry(stmt);
    if (res == nullptr) {
      OMS_ERROR("Query result set is null, sql: {}", query_sql);
      return OMS_FAILED;
    }

    defer(delete res);
    while (res->next()) {
      T obj;
      converter(res, obj);
      object_vec.emplace_back(obj);
    }
  } catch (sql::SQLException& e) {
    OMS_ERROR("Failed to execute query, sql: {}, error: {}", query_sql, e.what());
    return OMS_FAILED;
  }
  log_slow_sql_if_exists(timer.elapsed(), query_sql);
  return OMS_OK;
}

template <typename... Args>
int execute_with_retry(int (*query_func)(Args...), int max_retries, int retry_delay_s, Args&&... args)
{
  int retries = 0;
  int result = OMS_OK;
  while (retries < max_retries) {
    result = query_func(args...);

    if (result == OMS_OK) {
      OMS_DEBUG("execution Succeed!");
      break;
    } else {
      OMS_WARN("Execution failed, wait {} seconds and try again...,the current number of retries is {}",
          retry_delay_s,
          retries);
      retries++;
      sleep(retry_delay_s);
    }
  }

  if (retries == max_retries) {
    OMS_ERROR("The maximum number ({}) of retries was reached and the execution failed.", max_retries);
  }

  return result;
}
}  // namespace oceanbase::logproxy