
#include "log.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <string>

namespace log {

Log::Log(std::string &file_name)
    : file_stream{file_name, std::ios::trunc} {}

Log::Log(const char *file_name)
    : file_stream{file_name, std::ios::trunc} {}

Log::~Log() = default;

/**
 * NOTE: write(string) chama std::endl após escrever a mensagem.
 */
void Log::write(std::string &message) {
    this->file_stream << current_time() << " ";
    this->file_stream << message << std::endl;
}

/**
 * NOTE: write(char*) chama std::endl após escrever a mensagem.
 */
void Log::write(const char *message) {
    this->file_stream << current_time() << " ";
    this->file_stream << message << std::endl;
}

/**
 * NOTE: write(char) NÃO chama std::endl após escrever a mensagem.
 */
void Log::write(const char c) { this->file_stream << c; }

std::string Log::current_time() {
    // Get the current time point
    auto now = std::chrono::system_clock::now();
    // Convert to a time_t, which represents the time in seconds since epoch
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    // Convert to a tm structure for formatting
    std::tm *local_time = std::localtime(&now_time);
    // Format the time as a string
    std::ostringstream oss;
    oss << std::put_time(local_time, "[%Y-%m-%d %H:%M:%S]");
    return oss.str();
}

} // namespace log
