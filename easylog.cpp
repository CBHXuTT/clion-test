#include "easylog.h"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <filesystem>

bool has_init_ = false;
bool always_flush_ = false;

template <typename... Args>
inline void log(spdlog::level::level_enum level,
                source_location location,
                fmt::format_string<Args...> fmt,
                Args&&... args) {
  std::filesystem::path p{location.file_name()};

  switch (level) {
    case spdlog::level::trace:
      spdlog::trace("[{}] {}:{}: {}", location.module_name(),
                    p.filename().string(), location.line(),
                    fmt::format(fmt, std::forward<Args>(args)...));
      break;
    case spdlog::level::debug:
      spdlog::debug("[{}] {}:{}: {}", location.module_name(),
                    p.filename().string(), location.line(),
                    fmt::format(fmt, std::forward<Args>(args)...));
      break;
    case spdlog::level::info:
      spdlog::info("[{}] {}:{}: {}", location.module_name(),
                   p.filename().string(), location.line(),
                   fmt::format(fmt, std::forward<Args>(args)...));
      break;
    case spdlog::level::warn:
      spdlog::warn("[{}] {}:{}: {}", location.module_name(),
                   p.filename().string(), location.line(),
                   fmt::format(fmt, std::forward<Args>(args)...));
      break;
    case spdlog::level::err:
      spdlog::error("[{}] {}:{}: {}", location.module_name(),
                    p.filename().string(), location.line(),
                    fmt::format(fmt, std::forward<Args>(args)...));
      break;
    case spdlog::level::critical:
      spdlog::critical("[{}] {}:{}: {}", location.module_name(),
                       p.filename().string(), location.line(),
                       fmt::format(fmt, std::forward<Args>(args)...));
      break;
    case spdlog::level::off:
    case spdlog::level::n_levels:
      break;
  }

  if (always_flush_) {
    spdlog::default_logger()->flush();
  }

  if (level == spdlog::level::critical) {
    spdlog::default_logger()->flush();
    std::exit(EXIT_FAILURE);
  }
}

LogMessage::LogMessage(int level, source_location location)
    : level_(level), location_(location) {}

LogMessage::~LogMessage() {
  log((spdlog::level::level_enum)level_, location_, "{}", os_.str());
}

std::vector<spdlog::sink_ptr> get_sinks(const easylog_options& options) {
  std::vector<spdlog::sink_ptr> sinks;

  auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  console_sink->set_level((spdlog::level::level_enum)options.log_level);
  console_sink->set_pattern("[%m-%d %H:%M:%S.%e][%l] %v");
  sinks.push_back(console_sink);

  std::string filename = options.log_dir;
  std::string name = options.app_log_name;
  name.append(".log");
  if (filename.back() != '/' && filename.back() != '\\') {
    filename.append("/");
  }
  filename.append(name);
  auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      filename, options.max_size, options.max_files);
  file_sink->set_pattern("[%m-%d %H:%M:%S.%e][%l] %v");
  sinks.push_back(file_sink);

  return sinks;
}

void init_log(easylog_options options, bool over_write) {
  if (has_init_ && !over_write) {
    return;
  }

  auto sinks = get_sinks(options);

  auto logger = std::make_shared<spdlog::logger>(options.app_log_name,
                                                 sinks.begin(), sinks.end());

  logger->set_level((spdlog::level::level_enum)options.log_level);
  spdlog::set_level((spdlog::level::level_enum)options.log_level);
  spdlog::set_default_logger(logger);

  always_flush_ = options.always_flush;
  if (!options.always_flush && options.flush_interval > 0) {
    spdlog::flush_every(std::chrono::seconds(options.flush_interval));
  }

  has_init_ = true;
}

void enable_always_flush(bool always_flush) {
  always_flush_ = always_flush;
}