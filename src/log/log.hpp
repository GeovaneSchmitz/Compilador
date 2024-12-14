
#pragma once

#include <ctime>
#include <fstream>
#include <string>

namespace log {

class Log {
    public:
    Log(std::string &file_name);
    Log(const char *file_name);
    ~Log();

    /**
     * NOTE: write(string) chama std::endl após escrever a mensagem.
     */
    virtual void write(std::string &message);

    /**
     * NOTE: write(char*) chama std::endl após escrever a mensagem.
     */
    virtual void write(const char *message);

    /**
     * NOTE: write(char) NÃO chama std::endl após escrever a mensagem.
     */
    virtual void write(const char c);

    protected:
    std::ofstream file_stream;

    std::string current_time();
};

} // namespace log
