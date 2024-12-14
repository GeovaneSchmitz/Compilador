
#include "log_thread_safe.hpp"
#include <ctime>
#include <mutex>
#include <string>

namespace log {

ThreadSafeLog::ThreadSafeLog(std::string &file_name)
    : Log(file_name) {}

ThreadSafeLog::ThreadSafeLog(const char *file_name)
    : Log(file_name) {}

ThreadSafeLog::~ThreadSafeLog() = default;

/**
 * NOTE: write(string) chama std::endl após escrever a mensagem.
 */
void ThreadSafeLog::write(std::string &message) {
    this->log_mutex.lock();
    Log::write(message);
    this->log_mutex.unlock();
}

/**
 * NOTE: write(char*) chama std::endl após escrever a mensagem.
 */
void ThreadSafeLog::write(const char *message) {
    this->log_mutex.lock();
    Log::write(message);
    this->log_mutex.unlock();
}

/**
 * NOTE: write(char) NÃO chama std::endl após escrever a mensagem.
 */
void ThreadSafeLog::write(const char c) {
    this->log_mutex.lock();
    Log::write(c);
    this->log_mutex.unlock();
}

} // namespace log
