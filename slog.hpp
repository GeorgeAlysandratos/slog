#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <mutex>

#define slog(x) simple_logger(simple_logger_log_type::x)

enum class simple_logger_log_type {
    info,
    warn,
    error
};

class simple_logger
{
public:
    simple_logger(simple_logger_log_type t)
    : t_{t} {
    }

    template<typename T>
    simple_logger& operator<<(const T& val) {
        stream_ << val;
        return *this;
    }

    ~simple_logger() {
        manager_.add_log(*this);
    }

    static void set_stdout(bool val) {
        manager_.set_stdout(val);
    }

    static void set_basename(const char* filename) {
        manager_.set_basename(filename);
    }

private:
    simple_logger_log_type t_;
    std::ostringstream stream_;

    // ----------------------------------------

    class LogManager {
    public:
        LogManager() {
            std::ios::sync_with_stdio(false);
        }

        void add_log(simple_logger& log_entry) {
            auto get_output = [](simple_logger_log_type t) {
                switch(t) {
                    case simple_logger_log_type::info:
                        return "INFO";
                        break;
                    case simple_logger_log_type::warn:
                        return "WARN";
                        break;
                    case simple_logger_log_type::error:
                        return " ERR";
                        break;
                }
                return "";
            };

            auto get_color_output = [](simple_logger_log_type t) {
                switch(t) {
                    case simple_logger_log_type::info:
                        return "\033[0;97mINFO\033[0m";
                        break;
                    case simple_logger_log_type::warn:
                        return "\033[1;93mWARN\033[0m";
                        break;
                    case simple_logger_log_type::error:
                        return " \033[1;31mERR\033[0m";
                        break;
                }
                return "";
            };

            auto get_color = [](simple_logger_log_type t) {
                switch(t) {
                    case simple_logger_log_type::info:
                        return "\033[0;97m";
                        break;
                    case simple_logger_log_type::warn:
                        return "\033[0;93m";
                        break;
                    case simple_logger_log_type::error:
                        return "\033[1;31m";
                        break;
                }
                return "";
            };

            auto reset_color = []() {
                return "\033[0m";
            };

            std::ostringstream& stream = log_entry.stream_;

            std::lock_guard<std::mutex> lock(mutex_);

            char time[9];
            std::tm now_tm = check_file();

            std::strftime(time, sizeof(time), "%T", &now_tm);
            std::timespec ts;
            std::timespec_get(&ts, TIME_UTC);
            std::size_t milliseconds = ts.tv_nsec / 10000;

            if(stdout_) {
                std::ios_base::fmtflags f( std::cout.flags() );

                std::cout   << time << '.'
                            << std::setw(5) << std::setfill('0') << milliseconds
                            << ' ' << get_color_output(log_entry.t_) << " - "
                            << get_color(log_entry.t_) << stream.str() << reset_color()
                            << std::endl;

                std::cout.flags( f );
            }

            file_   << time << '.'
                    << std::setw(5) << std::setfill('0') << milliseconds << std::setw(0)
                    << ' ' << get_output(log_entry.t_) << " - " << stream.str()
                    << std::endl;
        }

        void set_stdout(bool val) {
            stdout_ = val;
        }

        void set_basename(const char* filename) {
            file_basename_ = filename;
        }

    private:
        std::tm check_file() {
            std::time_t now = std::time(nullptr);
            std::tm now_tm = *std::localtime(&now);
            if(file_tm.tm_mday != now_tm.tm_mday or file_tm.tm_mon != now_tm.tm_mon) {
                file_tm = now_tm;
                char current[32]{ 0 };
                std::strftime(current, sizeof(current), "%d-%m-%y.txt", &now_tm);
                std::string filename;
                if(file_basename_.length() > 0) {
                    filename.append(file_basename_);
                    filename.append(".");
                }
                filename.append(current);
                file_.close();
                file_.open(filename, std::ofstream::out | std::ofstream::app);
                file_ << std::endl;
            }
            return now_tm;
        }

        std::mutex mutex_;
        std::tm file_tm{0};
        std::string file_basename_;
        std::ofstream file_;
        bool stdout_{true};
    };

    inline static LogManager manager_{LogManager()};
};
