#ifndef CACHE_CALLBACK_H_
#define CACHE_CALLBACK_H_

#include "easylog.h"

namespace cache {

struct Callback {
  virtual void on_process(float process) { LOG(INFO) << "process:" << process; }

  virtual void on_success() { LOG(INFO) << "success"; }

  virtual void on_failed() { LOG(INFO) << "failed"; }
};

class NativeCallback final : public Callback {
 public:
  void on_process(float process) override { Callback::on_process(process); }

  void on_success() override { Callback::on_success(); }

  void on_failed() override { Callback::on_failed(); }
};

}  // namespace cache
#endif  // CACHE_CALLBACK_H_