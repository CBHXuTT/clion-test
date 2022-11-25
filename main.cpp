#include <iostream>
#include <thread>
#include <filesystem>

#include "easylog.h"

namespace fs = std::filesystem;

int main() {
    fs::path log_path = fs::current_path().append("log");

    if (!fs::exists(log_path)) {
        fs::create_directories(log_path);
    }

    easylog_options options;
    options.log_dir = log_path.string();

    init_log(options);

    LOG(INFO) << "test111";
    LOG(WARN) << "warning";
    LOG(ERROR) << "error:" << "11111";


    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    return 0;
}