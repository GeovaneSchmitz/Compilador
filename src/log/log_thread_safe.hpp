
#pragma once
#include "log.hpp"
#include <ctime>
#include <mutex>
#include <string>

namespace cmp_log {

class ThreadSafeLog : public Log {
    public:
    ThreadSafeLog(std::string &file_name);
    ThreadSafeLog(const char *file_name);
    ~ThreadSafeLog();

    /**
     * NOTE: write(string) chama std::endl após escrever a mensagem.
     */
    void write(std::string &message) override;

    /**
     * NOTE: write(char*) chama std::endl após escrever a mensagem.
     */
    void write(const char *message) override;

    /**
     * NOTE: write(char) NÃO chama std::endl após escrever a mensagem.
     */
    void write(const char c) override;

    private:
    std::mutex log_mutex;
};
} // namespace log
