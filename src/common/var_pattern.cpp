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

#include "var_pattern.h"
#include "common.h"

namespace oceanbase::logproxy {

/**
 * hard code directly
 * #(35), {(123), }(125), [(91), ](93)
 */
const char SqlPattern::token_pairs[128] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,  // 31
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,  // 63
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    ']',
    0,
    0,
    0,
    0,  // 95
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    '}',
    0,
    0,
    0,
    0,  // 127
};
const uint8_t SqlPattern::tokens[128] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,  // 31
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,  // 63
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    1,
    0,
    0,  // 95
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    1,
    0,
    0,  // 127
};
const uint8_t SqlPattern::end_tokens[128] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,  // 31
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,  // 63
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    0,  // 95
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    1,
    0,
    0,  // 127
};

bool SqlPattern::explode(const std::string& pattern, std::deque<Stub>& pieces, PatternErr& err)
{
  pieces.clear();

  std::string section;

  Stub stub;
  stub.set_type(LITERAL);

  char token = 0;

  int state = STATE_INIT;
  for (char c : pattern) {
    if (state == STATE_INIT) {
      if (tokens[(int)c]) {
        if (c == '#') {
          if (!section.empty()) {
            stub.set_value(section);
            pieces.emplace_back(stub);
            section.clear();
          }

          state = STATE_MATCH_TOKEN;
        } else {
          err = ILL_TOKEN;
          OMS_INFO("unexpected token : {}", c);
          return false;
        }
      } else {
        section += c;
      }
      continue;
    }

    if (state == STATE_MATCH_TOKEN) {
      if (!token_pairs[(int)c]) {
        err = ILL_BEGIN;
        OMS_INFO("unexpected begin token : {}", c);
        return false;
      }
      token = c;

      state = STATE_FETCH;
      stub.set_type(token == '[' ? LIST : FIELD);
      continue;
    }

    if (state == STATE_FETCH) {
      if (tokens[(int)c]) {
        if (end_tokens[(int)c]) {
          if (c != token_pairs[(int)token]) {
            err = UNMATCH;
            OMS_INFO("unmatch end token {} of {}", c, token);
            return false;
          }

          if (section.empty()) {
            err = NONE_FIELD;
            OMS_INFO("field empty");
            return false;
          }

          stub.set_value(section);
          pieces.emplace_back(stub);
          section.clear();

          state = STATE_INIT;
          stub.set_type(LITERAL);
        } else {
          err = ILL_END;
          OMS_INFO("unexped end token {} of {}", c, token);
          return false;
        }

      } else {
        section += c;
      }
      continue;
    }
  }

  bool ret;
  switch (state) {
    case STATE_INIT:
      if (!section.empty()) {
        stub.set_value(section);
        pieces.emplace_back(stub);
      }
      err = OK;
      ret = true;
      break;
    case STATE_MATCH_TOKEN:
      err = ILL_BEGIN;
      ret = false;
      break;
    case STATE_FETCH:
      err = ILL_END;
      ret = false;
      break;
    default:
      err = UNKNOWN;
      OMS_INFO("unknow state : {}", state);
      ret = false;
      break;
  }
  return ret;
}

bool UrlPattern::explode_path_vars(const std::string& path, std::map<uint32_t, std::string>& vars, PatternErr& err)
{
  vars.clear();

  bool start_flag = false;

  uint32_t idx = 0;
  std::string section;
  PatternState state = STATE_INIT;
  for (size_t i = 0; i < path.size();) {
    char c = path[i];

    if (state == STATE_INIT) {
      if (c == '/' || c == '{' || c == '}') {
        if (c == '/') {
          if (start_flag) {
            ++idx;
          }
          state = STATE_MATCH_TOKEN;
        } else {
          err = ILL_TOKEN;
          OMS_INFO("unexpected token : {}", c);
          vars.clear();
          return false;
        }
      }

      start_flag = true;
      ++i;
      continue;
    }

    if (state == STATE_MATCH_TOKEN) {
      if (c == '/' || c == '{' || c == '}') {
        if (c == '/') {
          ++i;
          continue;
        }
        if (c != '{') {
          err = ILL_BEGIN;
          OMS_INFO("unexpected start token : {}", c);
          vars.clear();
          return false;
        }

        state = STATE_FETCH;
      } else {
        state = STATE_NORMAL;
      }

      ++i;
      continue;
    }

    if (state == STATE_NORMAL) {
      if (c == '{' || c == '}') {
        err = ILL_TOKEN;
        OMS_INFO("unexpected token : {}", c);
        vars.clear();
        return false;
      }
      if (c == '/') {
        state = STATE_INIT;
        continue;
      }

      ++i;
      continue;
    }

    if (state == STATE_FETCH) {
      if (c == '/' || c == '{' || c == '}') {
        if (c == '}') {
          if (section.empty()) {
            err = NONE_FIELD;
            OMS_INFO("field empty");
            vars.clear();
            return false;
          }
          vars.emplace(idx, section);
          section.clear();

          state = STATE_END_FETCH;
        } else {
          err = ILL_END;
          OMS_INFO("unexpected end token {}", c);
          vars.clear();
          return false;
        }

      } else {
        section += c;
      }

      ++i;
      continue;
    }

    if (state == STATE_END_FETCH) {
      if (c != '/') {
        err = ILL_TOKEN;
        OMS_INFO("unexpected token : {} after end token", c);
        vars.clear();
        return false;
      }
      state = STATE_INIT;
      continue;
    }
  }

  bool ret;
  switch (state) {
    case STATE_INIT:
    case STATE_NORMAL:
    case STATE_END_FETCH:
      if (!section.empty()) {
        vars.emplace(idx, section);
      }
      err = OK;
      ret = true;
      break;
    case STATE_MATCH_TOKEN:
      err = OK;
      ret = true;
      break;
    case STATE_FETCH:
      err = ILL_END;
      vars.clear();
      ret = false;
      break;
    default:
      err = UNKNOWN;
      OMS_INFO("unknow state : %d", state);
      vars.clear();
      ret = false;
      break;
  }
  return ret;
}

bool UrlPattern::fetch_path_vars(
    const std::string& path, const std::map<uint32_t, std::string>& vars, std::map<std::string, std::string>& vals)
{
  vals.clear();

  bool start_flag = false;

  uint32_t idx = 0;
  std::string section;
  PatternState state = STATE_INIT;
  for (size_t i = 0; i < path.size();) {
    char c = path[i];

    if (state == STATE_INIT) {
      if (c == '/') {
        if (!section.empty()) {
          if (vars.count(idx) == 1) {
            vals.emplace(vars.at(idx), section);
          }
          section.clear();
          if (start_flag) {
            ++idx;
          }
        }
        ++i;

      } else {
        state = STATE_MATCH_TOKEN;
      }

      start_flag = true;
      continue;
    }

    if (state == STATE_MATCH_TOKEN) {
      if (c == '/') {
        state = STATE_INIT;
        continue;
      }
    }

    section += c;
    ++i;
  }

  switch (state) {
    case STATE_INIT:
    case STATE_MATCH_TOKEN:
      if (vars.count(idx) == 1 && !section.empty()) {
        vals.emplace(vars.at(idx), section);
      }
      break;
    default:
      break;
  }
  return true;
}

bool UrlPattern::fetch_path_vars_prefix(const std::string& path, std::string& prefix)
{
  auto pos = path.find("/{");
  if (pos == std::string::npos) {
    return false;
  }
  prefix = path.substr(0, pos);
  return true;
}

UrlTrie::~UrlTrie()
{
  clear(&_entrys);
}

void UrlTrie::clear(UrlTrie::UrlTrieNode* node)
{
  if (node != nullptr) {
    for (auto& entry : node->nexts) {
      clear(entry.second);
      delete entry.second;
      entry.second = nullptr;
    }
    node->nexts.clear();

    if (node->var.second != nullptr) {
      delete node->var.second;
    }
    node->var = {"", nullptr};
  }
}

bool UrlTrie::put(const std::string& path, const std::string& name)
{
  std::vector<std::string> sections;
  split(path, '/', sections);

  UrlTrieNode* node = &_entrys;
  for (auto& section : sections) {
    if (section.front() == '{' && section.back() == '}') {
      if (node->var.second == nullptr) {
        node->var = {section.substr(1, section.size() - 2), new UrlTrieNode()};
      }
      node = node->var.second;
      continue;
    }

    auto item = node->nexts.find(section);
    if (item != node->nexts.end()) {
      node = item->second;
    } else {
      auto new_node = new UrlTrieNode();
      node->nexts.emplace(section, new_node);
      node = new_node;
    }
  }
  if (node != nullptr) {
    node->end = true;
    node->name = name;
  }
  return !sections.empty();
}

/**
 * input: /aa/bb/cc
 * api:   /aa/{}/dd     not match
 *        /{}/bb/cc     match
 * TODO... backtrace
 */
bool UrlTrie::fetch_vars(const std::string& path, std::string& name, std::map<std::string, std::string>& vals)
{
  std::vector<std::string> sections;
  split(path, '/', sections);
  UrlTrieNode* node = &_entrys;
  for (auto& section : sections) {
    auto entry = node->nexts.find(section);
    if (entry == node->nexts.end()) {
      if (node->var.second != nullptr) {
        vals.emplace(node->var.first, section);
        node = node->var.second;
      } else {
        return false;
      }
    } else {
      node = entry->second;
    }
  }
  if (node->end) {
    name = node->name;
    return true;
  }
  return false;
}

}  // namespace oceanbase::logproxy