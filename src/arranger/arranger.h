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

#include <unordered_map>
#include <mutex>
#include "common.h"
#include "source_meta.h"
#include "client_meta.h"
#include "obcdc_config.h"
namespace oceanbase {
namespace logproxy {
class Arranger {
  OMS_SINGLETON(Arranger);
  OMS_AVOID_COPY(Arranger);

public:
  int init();

  int run_foreground();

private:
  void on_close(const Peer&);

  EventResult on_handshake(const Peer&, const Message&);

  int resolve(ObcdcConfig&, std::string& errmsg);

  int auth(const ObcdcConfig&, std::string& errmsg);

  int check_quota();

  /*!
   * @brief
   * @param oblog_config
   * @param errmsg
   * @return Does the clog checkpoint meet the requirements?
   */
  int check_clog(const ObcdcConfig& oblog_config, std::string& errmsg);

  int create(ClientMeta&, ObcdcConfig&);

  void response_error(const Peer&, MessageVersion version, ErrorCode code, const std::string&);

  int close_client_force(const ClientMeta& client, const std::string& msg = "");

  void close_by_pid(int pid, const ClientMeta& client);

  void gc_pid_routine();

private:
  /**
   * <ClientId, sink_peer>
   */
  std::unordered_map<std::string, ClientMeta> _client_peers;

  std::string _localhost;
  std::string _localip;

  Comm _accepter;
};

}  // namespace logproxy
}  // namespace oceanbase
