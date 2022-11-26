#include <filesystem>
#include <iostream>
#include <thread>

#include "easylog.h"
#include "type_list.h"
#include "type_list_test.h"

namespace fs = std::filesystem;

int main() {
  { TypelistTest(); }

//  {
//    fs::path log_path = fs::current_path().append("log");
//
//    if (!fs::exists(log_path)) {
//      fs::create_directories(log_path);
//    }
//
//    easylog_options options;
//    options.log_dir = log_path.string();
//
//    init_log(options);
//
//    std::thread t1([]() {
//      int64_t num = 0;
//      while (true) {
//        LOG(INFO) << num++;
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//      }
//    });
//
//    std::thread t2([&options]() {
//      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//      options.id += "1";
//      options.app_log_name += "1";
//      init_log(options);
//    });
//
//    t2.join();
//    t1.join();
//  }
  return 0;
}
