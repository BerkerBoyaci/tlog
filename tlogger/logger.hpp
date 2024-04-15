/**
 * @file logger.hpp
 * @author Berker BOYACI (berker.boyaci@smartalpha.ai)
 * @brief
 * @version 1.0.0
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LOG_API_HPP
#define LOG_API_HPP


#include <mutex>
#include <string>
#include <memory>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iostream>
#include <vector>
#include "formatter.hpp"
#if __cplusplus >= 201703L
#include <string_view>
#include <filesystem>
#else
#if defined _MSC_VER
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif
#endif

namespace Log
{

	constexpr auto operator "" _kb(unsigned long long value)
    {
		return value * 1024;
	}

	constexpr auto operator "" _mb(unsigned long long value)
    {
		return value * 1024 * 1024;
	}

	constexpr auto operator "" _gb(unsigned long long value)
    {
		return value * 1024 * 1024 * 1024;
	}

	// LogPriority enum class
	// Possible priority levels :
	// Quiet , Fatal , Error , Warning , Info , Verbose , Debug , Trace
	enum class LogPriority : unsigned int
	{
		Quiet = 0,
		Fatal = 8,
		Error = 16,
		Warning = 24,
		Info = 32,
		Verbose = 40,
		Debug = 48,
		Trace = 56
	};

	// LogOutput enum class
	// Possible ways to stream
	enum class LogOutput
	{
		Console,
		File
	};


	// Stream wrapper class for console in char and wchar_t types
	template <typename T>
	class StreamWrapper
	{
	public:
		static std::basic_ostream<T> &tout;
	};

	template <>
	std::ostream &StreamWrapper<char>::tout = std::cout;

	template <>
	std::wostream &StreamWrapper<wchar_t>::tout = std::wcout;

#if __cplusplus < 201703L

#if defined _MSC_VER

	static void t_mkdir(std::string t_path)
	{
		_mkdir(t_path.c_str());
	}

#elif defined __GNUC__
	static void t_mkdir(std::string t_path)
	{
		mkdir(t_path.c_str(), 0777);
	}

#endif
#endif


	// Class Logger<>
	// record of variadic arguments to selected stream in formatted string
	// Example:
	// 	Logger<wchar_t>::setLogOutput(L"log.txt");
	//	Logger<wchar_t>::setLogPriority(LogPriority::Debug);
	//	auto log = Logger<wchar_t>::getInstance();
	//	log->formatter_set(L"%f %l %m %t");
	//	log->log(LogPriority::Error, __LINE__, _T(__FILE__), L"tellus felis condimentum odio, : ", 1, );
	template <typename T = char>
	class Logger
	{
	public:
        /****************************************************************************
         * Special Members
         ***************************************************************************/

        Logger(const Logger &) noexcept = delete;

        Logger &operator=(const Logger &) noexcept = delete;
        /****************************************************************************
         * Public Members
         ***************************************************************************/


        /*
         *
         *
         */
		virtual ~Logger() noexcept
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_ofs.close();
		}

		/*
		 * Get single instance or create new object if not created
		 * @return: single instance of Logger.
		 */
		static std::shared_ptr<Logger> get_instance()
		{
			if (m_logger_instance == nullptr)
            {
                m_logger_instance = std::shared_ptr<Logger<T>>(new Logger<T>{});
            }
			return m_logger_instance;
		}

        /*
         *
         *
         */
		static std::string log_output_get()
		{
			return m_log_path;
		}

        /*
         *
         *
         */
		static LogPriority log_priority_get()
		{
			return m_log_priority;
		}

        /*
         *
         *
         */
		static std::basic_string<T> formatter_get()
		{
			return m_format.formatter_get();
		}

        /*
         *
         *
         */
		static unsigned long long  file_limit_get()
		{
			return m_max_file_size;
		}

        /*
         *
         *
         */
		static LogOutput log_format_get()
		{
			return m_log_output;
		}

		/*
		 * Not set or call for stream to console
		 * Set log path as std::basic_string for stream to file
		 * @param t_file_path : file path.
		 */
		static void log_output_set(const std::string& t_file_path)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
			m_log_path = t_file_path;
		}

		/*
		 * Set log priority level.
		 * @param t_log_priority: enum class LogPriority.
		 */
		static void log_priority_set(LogPriority t_log_priority)
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_log_priority = t_log_priority;
		}

		/*
		 * Get format type and pass to Formatter::getFormatter() function
		 * default as %t %m
		 * @param t_fmt :  format style as string.
		 */
		static void formatter_set(const std::basic_string<T> &t_fmt)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
            m_format.formatter_set(t_fmt);
		}

		/*
		 * Set file's limit as byte.
		 * @param t_fileLimit :  file size limit as byte.
		 */
		static void file_limit_set(unsigned long long t_fileLimit)
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_max_file_size = t_fileLimit;
		}

		/*
		 * Set output format
		 * if m_log_path is empty then stream to console
		 * if m_log_path is not empty then stream to file
		 */
		static void log_format_set()
		{

			if (0 == m_log_path.size())
			{
				m_log_output = LogOutput::Console;
			}
			else
			{
				m_log_output = LogOutput::File;
                file_open(m_log_path);
			}
		}

		/*
		 * For Quiet log priority.
		 * @param messageLevel:  Log Level.
		 */
		static void log(LogPriority messageLevel) 
		{
			(void)messageLevel;
		}

        /*
         *
         *
         */
		static void enable_reset_file(bool enable_file_reset)
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_enable_file_reset = enable_file_reset;
		}

		/*
		 * Log given message with defined parameters and pass LogMessage() function
		 * @param messageLevel: Log Level
		 * @param ...args: Variadic template arguments
		 */
		template <typename... Args>
		static void log(LogPriority messageLevel, Args &&...args)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
			if (messageLevel <= m_log_priority)
			{
				switch (messageLevel)
				{
				case LogPriority::Quiet:
					break;
				case LogPriority::Fatal:
					log_message("FATAL:", std::forward<Args>(args)...);

					break;
				case LogPriority::Error:
					log_message("ERROR:", std::forward<Args>(args)...);

					break;
				case LogPriority::Warning:
					log_message("WARNING:", std::forward<Args>(args)...);

					break;
				case LogPriority::Info:
					log_message("INFO:", std::forward<Args>(args)...);

					break;
				case LogPriority::Verbose:
					log_message("VERBOSE:", std::forward<Args>(args)...);

					break;
				case LogPriority::Debug:
					log_message("DEBUG:", std::forward<Args>(args)...);

					break;
				case LogPriority::Trace:
					log_message("TRACE:", std::forward<Args>(args)...);

					break;
				}
			}
		}



	protected:
		/*
		 * Construct Logger class as singleton.
		 * client can not access default constructor
		 * if m_log_path string is empty select the stream as console
		 * else select the stream as file
		 */
		Logger() noexcept
		{
            log_format_set();
		}

#if __cplusplus >= 201703L
		/*
		 * For C++17 and C++20 versions
		 * Open file in UTF-8 standart in ofstream write or append mode
		 * if the parent path not exist then create directory
		 * @param t_path :  file path
		 */
		static void file_open(std::filesystem::path t_path)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
			try
			{
				auto t_root = t_path.parent_path();

				if (false == std::filesystem::exists(t_path) &&
                    false == t_root.empty())
				{
					std::filesystem::create_directories(t_root.string());
				}
#if defined _WIN32
                m_ofs.imbue(std::locale(std::locale(),
                                        new std::codecvt<wchar_t,
                                                char,
                                                mbstate_t>)
                );
#else
				m_ofs.imbue(std::locale(std::locale(),
                                        new std::codecvt<wchar_t,
                                                            char,
                                                            mbstate_t>)
                                            );
#endif
				m_ofs.open(t_path, std::ofstream::out | std::ofstream::app);
				m_ofs.seekp(0, std::ios_base::end);
			}
			catch (const std::filesystem::filesystem_error &ex)
			{
				std::cout << ex.what() << "\n";
			}
			catch (...)
			{
				std::cout << "Unknown error in Logger::openFile()\n";
			}
		}
#else
		/*
		 * For C++14 and previous versions
		 * Open file in UTF-8 standart in ofstream write or append mode
		 * if the parent path not exist then create directory
		 * @param t_path : file path
		 */
		static void file_open(const std::string &t_path)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
			char delim = t_path.find('/') != std::string::npos ? '/' : '\\';
			auto ret = split(t_path, delim);
			std::string path;

			for (int i = 0; i < ret.size() - 1; i++)
			{
				if(i != 0){
					std::string tmpstr;
					tmpstr.append(ret[i]);
					std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> strconverter;
					
					tmpstr += strconverter.to_bytes( stringlit(T, "\\")).c_str();
					path += tmpstr;
				}
				else
				{
					path.append(ret[i]);
				}
				t_mkdir(path);
			}
#if defined _MSC_VER
			m_ofs.imbue(std::locale(std::locale::empty(), new std::codecvt<wchar_t, char, mbstate_t>("en_US.utf8")));
#else
			m_ofs.imbue(std::locale(std::locale(), new std::codecvt<wchar_t, char, mbstate_t>));
#endif
			m_ofs.open(t_path.c_str(), std::ofstream::out | std::ofstream::app);
			m_ofs.seekp(0, std::ios_base::end);
		}
#endif

		/*
		 * Pass argumants to set Formatter::format() and take back as formatted string
		 * and write the string to choosen stream in constructor
		 * @param ...args: Variadic template arguments
		 */
		template <typename... Args>
		static void log_message(Args &&...args)
		{
			auto formatted_string = m_format.format(std::forward<Args>(args)...);
            formatted_string += '\n';

			if (LogOutput::File == m_log_output)
			{
				if ((static_cast<unsigned long long>( m_ofs.tellp()) +
                                                    formatted_string.length() )
                                                    >= m_max_file_size
					&& true == m_enable_file_reset )
				{
					m_ofs.close();
					m_ofs.open(m_log_path, std::ios::out | std::ios::trunc);
				}
				m_ofs << formatted_string.c_str();
			}

			if (LogOutput::Console == m_log_output)
			{
				StreamWrapper<T>::tout << formatted_string.c_str();
			}
		}

		/*
		 * Divides a String into an ordered list of substrings, puts these substrings into
		 * a vector of string
		 * @param t_str_split: basic_string<T>
		 * @param Delim: Template argument
		 * @return : result : vector<basic_string<T>>
		 */
		static std::vector<std::string> split(const std::string &t_str_split,
                                              char t_delim)
		{
			std::vector<std::string> result;
			std::stringstream ss(t_str_split);
			std::string item;

			while (getline(ss, item, t_delim))
			{
				result.push_back(item);
			}
			return result;
		}

	protected:
	    /**
         * @brief Holds file reset information.
         * 
         */
		static bool m_enable_file_reset;

	    /**
         * @brief holds critical section.
         * 
         */
		static std::mutex m_mutex;

		/**
         * @brief Holds format type.
         * 
         */
		static Formatter<T> m_format;
		
		/**
         * @brief Max file size of log outputs.
         * 
         */
		static unsigned long long m_max_file_size;
		
		/**
         * @brief Holds log file path.
         * 
         */
		static std::string m_log_path;
		
		/**
         * @brief Holds output file stream instance.
         * 
         */
		static std::basic_ofstream<T> m_ofs;
		
		/**
         * @brief Holds singleton instance.
         * 
         */
		static std::shared_ptr<Logger<T>> m_logger_instance;
		
		/**
         * @brief Holds LogPriority enum.
		 * Possible priority levels :
         * Quiet,
		 * Fatal,
		 * Error,
		 * Warning,
		 * Info,
		 * Verbose,
		 * Debug,
		 * Trace
		 * 
         */
		static LogPriority m_log_priority;
		
		/**
         * @brief Holds LogOutput enum.
		 * Possible ways to stream:
		 * Console,
		 * File
         * 
         */
		static LogOutput m_log_output;

	}; // end of class

// Macro definitions for Logger::log()
	
#define LOG_QUIET()

#if defined __GNUC__
#define LOG_SET_FORMAT(formatter) Log::logger<char>::set_formatter(Formatter)

#define LOG_SET_FILE_LIMIT(fileLimit) Log::logger<char>::set_file_limit(fileLimit)
#define LOG_ENABLE_RESET_FILE(enable_reset_file)	\
	Log::logger<char>::enable_reset_file(enable_reset_file)
#define LOG_SET_OUTPUT(path)               \
	Log::logger<char>::set_log_output(path); \
	Log::logger<char>::set_log_format()
#define LOG_SET_PRIORITY(severity) 		 \
	Log::logger<char>::set_log_priority(static_cast<Log::LogPriority>(severity))

#define LOG_FATAL(...) Log::logger<char>::log(Log::LogPriority::Fatal, __VA_ARGS__)
#define LOG_ERROR(...) Log::logger<char>::log(Log::LogPriority::Error, __VA_ARGS__)
#define LOG_WARNING(...) Log::logger<char>::log(Log::LogPriority::Warning, __VA_ARGS__)
#define LOG_INFO(...) Log::logger<char>::log(Log::LogPriority::Info, __VA_ARGS__)
#define LOG_VERBOSE(...) Log::logger<char>::log(Log::LogPriority::Verbose, __VA_ARGS__)
#define LOG_DEBUG(...) Log::logger<char>::log(Log::LogPriority::Debug, __VA_ARGS__)
#define LOG_TRACE(...) Log::logger<char>::log(Log::LogPriority::Trace, __VA_ARGS__)
#endif

#if defined _WIN32
#define LOG_SET_FORMAT(formatter) Log::logger<wchar_t>::set_formatter(Formatter)
#define LOG_ENABLE_RESET_FILE(enable_reset_file)	\
	Log::logger<wchar_t>::enable_reset_file(enable_reset_file)
#define LOG_SET_OUTPUT(path)                  \
	Log::logger<wchar_t>::set_log_output(path); \
	Log::logger<wchar_t>::set_log_format()
#define LOG_SET_FILE_LIMIT(fileLimit) 		\
	Log::logger<wchar_t>::set_file_limit(fileLimit)

#define LOG_SET_PRIORITY(severity)			\
	Log::logger<wchar_t>::set_log_priority(static_cast<Log::LogPriority>(severity))

#define LOG_FATAL(...) Log::logger<wchar_t>::log(Log::LogPriority::Fatal, __VA_ARGS__)
#define LOG_ERROR(...) Log::logger<wchar_t>::log(Log::LogPriority::Error, __VA_ARGS__)
#define LOG_WARNING(...) Log::logger<wchar_t>::log(Log::LogPriority::Warning, __VA_ARGS__)
#define LOG_INFO(...) Log::logger<wchar_t>::log(Log::LogPriority::Info, __VA_ARGS__)
#define LOG_VERBOSE(...) Log::logger<wchar_t>::log(Log::LogPriority::Verbose, __VA_ARGS__)
#define LOG_DEBUG(...) Log::logger<wchar_t>::log(Log::LogPriority::Debug, __VA_ARGS__)
#define LOG_TRACE(...) Log::logger<wchar_t>::log(Log::LogPriority::Trace, __VA_ARGS__)
#endif

// Intialize static data members
template <typename T>
std::mutex Logger<T>::m_mutex;
template <typename T>
std::basic_ofstream<T> Logger<T>::m_ofs;
template <typename T>
unsigned long long Logger<T>::m_max_file_size = 512 * 1024 * 1024; // 512 MB
template <typename T>
std::string Logger<T>::m_log_path{};
template <typename T>
bool Logger<T>::m_enable_file_reset{false};
template <typename T>
LogPriority Logger<T>::m_log_priority = LogPriority::Trace;
template <typename T>
LogOutput Logger<T>::m_log_output = LogOutput::Console;
template <typename T>
std::shared_ptr<Logger<T>> Logger<T>::m_logger_instance = nullptr;
template <typename T>
Formatter<T> Logger<T>::m_format;

// template alias for Logger class
using logger_w = Logger<wchar_t>;
using logger_c = Logger<char>;

} // end of Log namespace

#endif // LOG_API_HPP

/* end_of_file */
