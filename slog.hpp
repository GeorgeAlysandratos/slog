#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>

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

private:
	simple_logger_log_type t_;
	std::ostringstream stream_;

	// ----------------------------------------

	class LogManager {
	public:
		LogManager() {
			std::ios::sync_with_stdio(false);
			check_file();
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
						return "\033[0;37mINFO\033[0m";
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
						return "\033[0;37m";
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

			check_file();

			char time[9];
			std::time_t now = std::time(nullptr);
			std::tm now_tm = *std::localtime(&now);
			std::strftime(time, sizeof(time), "%T", &now_tm);
			std::timespec ts;
			std::timespec_get(&ts, TIME_UTC);
			std::size_t milliseconds = ts.tv_nsec / 10000;

			if(stdout_) {
				std::ios_base::fmtflags f( std::cout.flags() );

				std::cout 	<< time << '.'
							<< std::setw(5) << std::setfill('0') << milliseconds
							<< ' ' << get_color_output(log_entry.t_) << " - "
							<< get_color(log_entry.t_) << stream.str() << reset_color() << std::endl;

				std::cout.flags( f );
			}

			file_ 	<< time << '.'
					<< std::setw(5) << std::setfill('0') << milliseconds << std::setw(0)
					<< ' ' << get_output(log_entry.t_) << " - " << stream.str() << std::endl;
		}

		void set_stdout(bool val) {
			stdout_ = val;
		}

	private:
		static constexpr std::size_t FILENAME_LENGTH{13};

		void check_file() {
			char current[FILENAME_LENGTH];
			std::time_t now = std::time(nullptr);
			std::tm now_tm = *std::localtime(&now);
			std::strftime(current, sizeof(current), "%d-%m-%y.txt", &now_tm);
			if( strcmp(file_name_, current) != 0 ) {
				file_.close();
				memcpy(file_name_, current, FILENAME_LENGTH);
				file_.open(file_name_, std::ofstream::out | std::ofstream::app);
				file_ << '\n';
			}
		}

		std::mutex mutex_;
		char file_name_[FILENAME_LENGTH]{0};
		std::ofstream file_;

		std::tm file_time_;
		bool stdout_{true};
	};

	inline static LogManager manager_{LogManager()};
};
