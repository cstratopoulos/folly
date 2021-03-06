/*
 * Copyright 2019-present Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include <folly/Optional.h>
#include <folly/experimental/coro/AsyncGenerator.h>
#include <folly/experimental/coro/Task.h>

namespace folly {
namespace python {

template <typename T>
class AsyncGeneratorWrapper {
 public:
  AsyncGeneratorWrapper() = default;
  explicit AsyncGeneratorWrapper(coro::AsyncGenerator<T>&& gen)
      : gen_(std::move(gen)) {}

  coro::Task<Optional<T>> getNext() {
    if (!iter_) {
      iter_ = co_await gen_.begin();
    } else {
      co_await(++*iter_);
    }
    if (iter_ == gen_.end()) {
      co_return none;
    }
    co_return(**iter_);
  }

 private:
  coro::AsyncGenerator<T> gen_;
  Optional<typename coro::AsyncGenerator<T>::async_iterator> iter_;
};

} // namespace python
} // namespace folly
