
#ifndef EASYLOG_H_
#define EASYLOG_H_

#include <string>
#include <sstream>

constexpr int TRACE = 0;
constexpr int DEBUG = 1;
constexpr int INFO = 2;
constexpr int WARN = 3;
constexpr int ERROR = 4;
constexpr int CRITICAL = 5;
constexpr int OFF = 6;

struct easylog_options {
    int log_level = DEBUG;
    std::string app_log_name = "easylog";
    std::string log_dir;
    bool always_flush = false;
    int flush_interval = 3;
    int max_size = 5 * 1024 * 1024;
    int max_files = 5;
};


struct source_location {
    constexpr source_location(const char *module_name = MODULE_NAME,
                              const char *file_name = __builtin_FILE(),
                              const char *function_name = __builtin_FUNCTION(),
                              unsigned int line = __builtin_LINE()) noexcept
            : module_name_(module_name), file_name_(file_name), function_name_(function_name),
              line_(line) {}


    constexpr const char *module_name() noexcept { return module_name_; }

    constexpr const char *file_name() noexcept { return file_name_; }

    constexpr const char *function_name() noexcept { return function_name_; }

    constexpr unsigned int line() const noexcept { return line_; }


private:
    const char *module_name_;
    const char *file_name_;
    const char *function_name_;
    const unsigned int line_;
};


struct LogMessage {
    explicit LogMessage(int level, source_location location = {});

    ~LogMessage();

    std::ostream &stream() { return os_; }

private:
    int level_;
    source_location location_;
    std::ostringstream os_;
};

#define LOG(level) LogMessage{level}.stream()


void init_log(easylog_options options = {}, bool over_write = false);

void enable_always_flush(bool always_flush);

#endif // EASYLOG_H_
