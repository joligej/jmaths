#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>

class function {
   private:
    const std::string name_;
    std::map<function *, unsigned long long> called_functions_;

    double average_time = 0;
    double total_time = 0;
    unsigned long long call_count = 0;

   public:
    function(std::string && name) : name_{name} {}
    function(const std::string & name) : name_{name} {}

    std::string_view name() const {
        return name_;
    }

    void new_time(double call_time) {
        total_time += call_time;
        ++call_count;
        average_time = total_time / call_count;
    }

    void new_call(function * f) {
        ++called_functions_[f];
    }

    double total_call_time() const {
        return total_time;
    }

    double average_call_time() const {
        return average_time;
    }

    unsigned long long calls_to() const {
        return call_count;
    }

    unsigned long long calls_from() const {
        unsigned long long total_call_count = 0;

        for (const auto & [function, call_count] : called_functions_) {
            total_call_count += call_count;
        }

        return total_call_count;
    }
};

int main(int argc, char * argv[]) {
    std::clog << "Processing log file...\n";

    std::ifstream potential_log_file;

    if (argc > 1) { potential_log_file.open(argv[1]); }

    std::istream & log_file = potential_log_file.is_open() ? potential_log_file : std::cin;

    std::map<std::string, function> function_by_name;

    std::stack<function *> function_stack;

    for (std::string line; std::getline(log_file, line);) {
        std::istringstream line_stream;
        line_stream.str(line);

        std::string first_word;
        line_stream >> first_word;
        line_stream >> std::ws;

        if (first_word == "log:call") {
            std::getline(line_stream, line);
            function_by_name.try_emplace(line, line);

            function * function_called = &function_by_name.at(line);

            if (!function_stack.empty()) { function_stack.top()->new_call(function_called); }

            function_stack.emplace(function_called);
        } else if (first_word == "log:time") {  // this means first_word == "time"
            double call_time;
            line_stream >> call_time;

#if 0
  {
  std::string temp;
  line_stream >> temp >> temp;
  // to get rid of "ms" and "from"
  }

  line_stream >> std::ws

  std::getline(line_stream, line);

  function_by_name[line].new_time(call_time);

#endif

#if 1
            std::getline(line_stream, line);

            function_stack.top()->new_time(call_time);
#endif

            function_stack.pop();
        } else {
            std::getline(line_stream, line);
            std::clog << "Skipping a line\n";
        }
    }

    if (!function_stack.empty()) std::clog << "Open stack frame:\n";
    while (!function_stack.empty()) {
        std::clog << function_stack.top()->name() << '\n';
        function_stack.pop();
    }

    std::cout << "name,ttltime,avgtime,to,from\n";

    for (const auto & [function_name, function] : function_by_name) {
        std::cout << "\"" << function_name << "\"" << ',';
        std::cout << function.total_call_time() << ',';
        std::cout << function.average_call_time() << ',';
        std::cout << function.calls_to() << ',';
        std::cout << function.calls_from() << '\n';
    }

    std::clog << "Log file processed succesfully\n";
}
