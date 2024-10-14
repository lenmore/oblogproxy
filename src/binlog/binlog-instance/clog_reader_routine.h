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

#include "thread.h"
#include "log.h"
#include "blocking_queue.hpp"
#include "obcdcaccess/obcdc/obcdc_entry.h"
#include "obcdc_config.h"

using namespace oceanbase::logproxy;
namespace oceanbase::binlog {

class BinlogConverter;

class ClogReaderRoutine : public Thread {
public:
  ClogReaderRoutine(BinlogConverter&, BlockingQueue<ILogRecord*>&);

  int init(const ObcdcConfig& config, IObCdcAccess* obcdc);

  void stop() override;

  void run() override;

private:
  BinlogConverter& _converter;
  IObCdcAccess* _obcdc;

  BlockingQueue<ILogRecord*>& _queue;
};

}  // namespace oceanbase::binlog
