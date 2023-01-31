#include <filesystem>
#include <iostream>
#include <thread>
#include <functional>

#include "easylog.h"
#include "type_list.h"
#include "type_list_test.h"
#include "cache_callback.h"

#include "functional/function.h"
#include "proxy/proxy.h"

#include "cinatra.hpp"
#include "compile_parse.h"
#include "rust_macro_rule.h"

//namespace fs = std::filesystem;
using namespace cinatra;
using namespace std::string_view_literals;

struct DynamicInit {
  int x;
  int y;
};

class ListenerTest final {
 public:
  void Register(std23::function<void(DynamicInit)> callback){
    std::cout << "Register" << std::endl;
    callback({.x=1,.y=2});
  }
};

struct CallBack {
  void Notify(DynamicInit data){
    std::cout << "data:" << data.x << "\t" << data.y <<std::endl;
  }
};


struct at : pro::dispatch<std::string(int)> {
  template <class T>
  auto operator()(const T& self, int key) { return self.at(key); }
};

struct resource_dictionary : pro::facade<at> {};

void demo_print(pro::proxy<resource_dictionary> dictionary) {
  std::cout << dictionary.invoke<at>(1) << std::endl;
}

// Abstraction
struct Draw : pro::dispatch<void(std::ostream&)> {
  template <class T>
  void operator()(const T& self, std::ostream& out) { self.Draw(out); }
};
struct Area : pro::dispatch<double()> {
  template <class T>
  double operator()(const T& self) { return self.Area(); }
};
struct DrawableFacade : pro::facade<Draw, Area> {};

// Implementation
class Rectangle {
 public:
  void Draw(std::ostream& out) const
  { out << "{Rectangle: width = " << width_ << ", height = " << height_ << "}"; }
  void SetWidth(double width) { width_ = width; }
  void SetHeight(double height) { height_ = height; }
  double Area() const { return width_ * height_; }

 private:
  double width_;
  double height_;
};

// Client - Consumer
std::string PrintDrawableToString(pro::proxy<DrawableFacade> p) {
  std::stringstream result;
  result << "shape = ";
  p.invoke<Draw>(result);
  result << ", area = " << p.invoke<Area>();
  return std::move(result).str();
}

// Client - Producer
pro::proxy<DrawableFacade> CreateRectangleAsDrawable(int width, int height) {
  class Rectangle rect;
  rect.SetWidth(width);
  rect.SetHeight(height);
  return pro::make_proxy<DrawableFacade>(rect);
}


int main() {
  {
    std::any any;

  }
  {
    std::map<int, std::string> container1{{1, "hello"}};
    std::vector<std::string> container2{"hello", "world"};
    demo_print(&container1);  // print: hello\n
    demo_print(&container2);  // print: world\n
  }

  {
    http_server server(1);
    if (server.listen("0.0.0.0"sv, "18888"sv)) {
      server.set_http_handler<POST>(
          "/test"sv, [](cinatra::request& req, cinatra::response& res) {
            res.set_status_and_content(status_type::ok, "hello",
                                       req_content_type::json);
          });
    }
//    server.run();
//    server.stop();
  }
  {
    constexpr auto fn1 = $fn((a, b, c, d) -> d - ((a - b) + c));
    fn1(12, 97, 11, 121);
  }
  {
      ListenerTest ls;
      CallBack fun;

      ls.Register({std23::nontype<&CallBack::Notify>, fun});
  }


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
