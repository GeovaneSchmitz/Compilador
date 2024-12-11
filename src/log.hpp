#ifndef __XPP_LOG__
#define __XPP_LOG__

#include <fstream>
#include <string>
#include <mutex>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace logging {
    
    class Log {
        public:
            Log(std::string& file_name) : file_stream{file_name, std::ios::trunc} {}
            Log(const char* file_name) : file_stream{file_name, std::ios::trunc} {}
            ~Log() = default;

            /**
             * NOTE: write(string) chama std::endl após escrever a mensagem.
             */
            void write(std::string& message) {
                this->file_stream << current_time() << " ";
                this->file_stream << message << std::endl;
            }

            /**
             * NOTE: write(char*) chama std::endl após escrever a mensagem.
             */
            void write(const char* message) {
                this->file_stream << current_time() << " ";
                this->file_stream << message << std::endl;
            }

            /**
             * NOTE: write(char) NÃO chama std::endl após escrever a mensagem.
             */
            void write(const char c) {
                this->file_stream << c;
            }

        protected:
            std::ofstream file_stream;

            std::string current_time() {
                // Get the current time point
                auto now = std::chrono::system_clock::now();
                // Convert to a time_t, which represents the time in seconds since epoch
                std::time_t now_time = std::chrono::system_clock::to_time_t(now);
                // Convert to a tm structure for formatting
                std::tm* local_time = std::localtime(&now_time);
                // Format the time as a string
                std::ostringstream oss;
                oss << std::put_time(local_time, "[%Y-%m-%d %H:%M:%S]");
                return oss.str();
            }

    };

    class ThreadSafeLog : public Log {
        public:
            ThreadSafeLog(std::string& file_name) : Log(file_name) {}
            ThreadSafeLog(const char* file_name) : Log(file_name) {}
            ~ThreadSafeLog() = default;

            /**
             * NOTE: write(string) chama std::endl após escrever a mensagem.
             */
            void write(std::string& message) {
                this->log_mutex.lock();
                Log::write(message);
                this->log_mutex.unlock();
            }

            /**
             * NOTE: write(char*) chama std::endl após escrever a mensagem.
             */
            void write(const char* message) {
                this->log_mutex.lock();
                Log::write(message);
                this->log_mutex.unlock();
            }

            /**
             * NOTE: write(char) NÃO chama std::endl após escrever a mensagem.
             */
            void write(const char c) {
                this->log_mutex.lock();
                Log::write(c);
                this->log_mutex.unlock();
            }

        private:
            std::mutex log_mutex;
    };
}

#endif