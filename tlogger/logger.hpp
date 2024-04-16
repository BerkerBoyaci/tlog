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
#if defined _WIN32
#include <direct.h>
#elif defined __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif
#endif

namespace Log
{

    /**
     * @brief User-defined literal to convert value to kilobytes (KB).
     *
     * This constexpr operator converts a given value to kilobytes (KB) by multiplying it with 1024.
     *
     * @param value The value to be converted.
     * @return The converted value in kilobytes.
     */
    constexpr auto operator "" _kb(unsigned long long value)
    {
        return value * 1024;
    }

    /**
     * @brief User-defined literal to convert value to megabytes (MB).
     *
     * This constexpr operator converts a given value to megabytes (MB) by multiplying it with 1024 * 1024.
     *
     * @param value The value to be converted.
     * @return The converted value in megabytes.
     */
    constexpr auto operator "" _mb(unsigned long long value)
    {
        return value * 1024 * 1024;
    }

    /**
     * @brief User-defined literal to convert value to gigabytes (GB).
     *
     * This constexpr operator converts a given value to gigabytes (GB) by multiplying it with 1024 * 1024 * 1024.
     *
     * @param value The value to be converted.
     * @return The converted value in gigabytes.
     */
    constexpr auto operator "" _gb(unsigned long long value)
    {
        return value * 1024 * 1024 * 1024;
    }

    /**
     * @brief Enum class representing different log priority levels.
     *
     * This enum class defines possible priority levels for logging, ranging from Quiet to Trace.
     */
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

    /**
     * @brief Enum class representing different ways to stream log messages.
     *
     * This enum class defines possible ways to stream log messages, either to the console or to a file.
     */
	enum class LogOutput
	{
		Console,
		File
	};

    /**
     * @brief Stream wrapper class for console output in character types.
     *
     * This template class provides a static member `tout` that represents the standard output stream (std::cout)
     * for the `char` type and std::wcout for the `wchar_t` type.
     *
     * @tparam T The character type (`char` or `wchar_t`) used for the stream.
     */
	template <typename T>
	class StreamWrapper
	{
	public:
		static std::basic_ostream<T> &tout;
	};

    /**
     * @brief Specialization of StreamWrapper class for `char` type.
     *
     * This specialization sets `StreamWrapper<char>::tout` to std::cout.
     */
	template <>
	std::ostream &StreamWrapper<char>::tout = std::cout;

    /**
     * @brief Specialization of StreamWrapper class for `wchar_t` type.
     *
     * This specialization sets `StreamWrapper<wchar_t>::tout` to std::wcout.
     */
	template <>
	std::wostream &StreamWrapper<wchar_t>::tout = std::wcout;

#if __cplusplus < 201703L

#if defined _WIN32
    /**
     * @brief Creates a directory using _mkdir function (Windows).
     *
     * This function creates a directory specified by the given path using the _mkdir function
     * provided by Microsoft Visual C++ compiler (Windows).
     *
     * @param t_path The path of the directory to create.
     */
	static void t_mkdir(std::string t_path)
	{
		_mkdir(t_path.c_str());
	}

#elif defined __GNUC__
    /**
     * @brief Creates a directory using mkdir function (Unix/Linux).
     *
     * This function creates a directory specified by the given path using the mkdir function
     * provided by GNU Compiler Collection (GCC) (Unix/Linux).
     *
     * @param t_path The path of the directory to create.
     */
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


        /**
         * @brief Destructor for the Logger class.
         *
         * Safely closes the log file associated with the logger instance.
         *
         */
		virtual ~Logger() noexcept
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_ofs.close();
		}

        /**
         * @brief Gets the singleton instance of the Logger or creates a new instance if none exists.
         *
         * @return A shared pointer to the Logger instance.
         *
         */
		static std::shared_ptr<Logger> get_instance()
		{
			if (m_logger_instance == nullptr)
            {
                m_logger_instance = std::shared_ptr<Logger<T>>(new Logger<T>{});
            }
			return m_logger_instance;
		}

        /**
         * @brief Returns the path of the current log output file.
         *
         * @return The path of the log output file as a string.
         */
		static std::string log_output_get()
		{
			return m_log_path;
		}

        /**
         * @brief Returns the current logging priority level.
         *
         * @return The logging priority level.
         */
		static LogPriority log_priority_get()
		{
			return m_log_priority;
		}

        /**
         * @brief Returns the log message formatter.
         *
         * @tparam T The character type used in the formatter.
         * @return The log message formatter as a basic_string.
         */
		static std::basic_string<T> formatter_get()
		{
			return m_format.formatter_get();
		}

        /**
         * @brief Returns the maximum file size limit for log files.
         *
         * @return The maximum file size limit in bytes.
         */
		static unsigned long long  file_limit_get()
		{
			return m_max_file_size;
		}

        /**
         * @brief Returns the current log output format.
         *
         * @return The current log output format.
         */
		static LogOutput log_format_get()
		{
			return m_log_output;
		}

        /**
         * @brief Sets the log output to stream to a file with the specified file path.
         *
         * This function configures the Logger to output log messages to a file specified by the given file path.
         *
         * @param t_file_path The file path for logging output (stream to file).
         */
		static void log_output_set(const std::string& t_file_path)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
			m_log_path = t_file_path;
		}

        /**
         * @brief Sets the log priority level for filtering log messages.
         *
         * This function sets the log priority level, which determines the minimum log level required for messages to be logged.
         *
         * @param t_log_priority The log priority level to be set (enum class LogPriority).
         */
		static void log_priority_set(LogPriority t_log_priority)
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_log_priority = t_log_priority;
		}

        /**
         * @brief Sets the log message format using a specified string.
         *
         * This function sets the log message format to the provided string using Formatter::formatter_set().
         * The default format is "%t %m" if not specified.
         *
         * @param t_fmt The format style as a string.
         */
		static void formatter_set(const std::basic_string<T> &t_fmt)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
            m_format.formatter_set(t_fmt);
		}

        /**
         * @brief Sets the maximum file size limit for log files.
         *
         * This function sets the maximum file size limit (in bytes) for log files.
         *
         * @param t_fileLimit The file size limit in bytes.
         */
		static void file_limit_set(unsigned long long t_fileLimit)
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_max_file_size = t_fileLimit;
		}

        /**
         * @brief Sets the log output format based on the configured log path.
         *
         * If the log path is empty, the output is directed to the console (LogOutput::Console).
         * If the log path is not empty, the output is directed to a file (LogOutput::File), and the file is opened.
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

        /**
         * @brief Logs a message with a specified log priority level (no-op function).
         *
         * This function does nothing and serves as a placeholder for logging messages with specific priority levels.
         *
         * @param messageLevel The log priority level.
         */
		static void log(LogPriority messageLevel) 
		{
			(void)messageLevel;
		}

        /**
         * @brief Enables or disables the file reset feature.
         *
         * This function allows enabling or disabling
         * the behavior of resetting log files.
         *
         * @param enable_file_reset If true, enables file reset;
         * if false, disables file reset.
         */
		static void enable_reset_file(bool enable_file_reset)
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_enable_file_reset = enable_file_reset;
		}


        /**
         * @brief Logs a message with a specified log priority level and additional arguments.
         *
         * This function logs a message with the specified log priority level and additional arguments.
         * The message is formatted based on the log priority and forwarded to the appropriate log_message function.
         *
         * @param messageLevel The log priority level.
         * @param args Additional arguments to be formatted into the log message.
         *
         * @tparam Args Variadic template arguments that represent the types of additional arguments.
         *
         * @note The log message is only processed if the specified messageLevel is less than or equal to the current log priority level (m_log_priority).
         *       - LogPriority::Quiet messages are ignored.
         *       - Messages are prefixed with the corresponding log level identifier (e.g., "FATAL:", "ERROR:", etc.).
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

    /*******************************************************************    
     * Private Members 
     ******************************************************************/

    private:
        
        /* data */

        
    /*******************************************************************    
     * Protected Members 
     ******************************************************************/
 
	protected:

        /**
         * @brief Constructs the Logger class as a singleton instance.
         *
         * This constructor initializes the Logger instance. If the log path string (m_log_path) is empty,
         * it selects the console stream (LogOutput::Console); otherwise, it selects the file stream (LogOutput::File).
         */
		Logger() noexcept
		{
            log_format_set();
		}

#if __cplusplus >= 201703L
		 /**
         * @brief Opens a file with UTF-8 encoding in ofstream write or append mode.
         *
         * For C++17 and later versions, this function opens a file specified by the given filesystem path (t_path)
         * in UTF-8 encoding using std::ofstream in write or append mode.
         *
         * If the parent directory of the file does not exist, it creates the directory.
         *
         * @param t_path The path to the file to be opened.
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

        /**
         * @brief Opens a file with UTF-8 encoding in ofstream write or append mode.
         *
         * For C++14 and earlier versions, this function opens a file specified by the given string path (t_path)
         * in UTF-8 encoding using std::ofstream in write or append mode.
         *
         * If the parent directory of the file does not exist, it creates the directory.
         *
         * @param t_path The path to the file to be opened.
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
#if defined _WIN32
			m_ofs.imbue(std::locale(std::locale::empty(), new std::codecvt<wchar_t, char, mbstate_t>("en_US.utf8")));
#else
			m_ofs.imbue(std::locale(std::locale(), new std::codecvt<wchar_t, char, mbstate_t>));
#endif
			m_ofs.open(t_path.c_str(), std::ofstream::out | std::ofstream::app);
			m_ofs.seekp(0, std::ios_base::end);
		}
#endif

        /**
         * @brief Formats and logs a message to the chosen output stream.
         *
         * This function formats the provided arguments using Formatter::format() and writes
         * the formatted message to the selected stream (file or console) based on the configured
         * log output (m_log_output).
         *
         * @param args Variadic template arguments to be formatted into the log message.
         *
         * @tparam Args Variadic template parameter pack representing the types of the arguments.
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

        /**
         * @brief Splits a string into substrings based on a specified delimiter and returns the result as a vector of strings.
         *
         * This function splits the input string (t_str_split) into multiple substrings using the provided delimiter (t_delim).
         * The resulting substrings are stored in a vector of strings.
         *
         * @param t_str_split The string to be split.
         * @param t_delim The delimiter character used for splitting.
         * @return std::vector<std::string> A vector containing the split substrings.
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

    /*******************************************************************    
    * Protected Members 
    ******************************************************************/
    
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
	
#define log_quiet()
#if defined _WIN32

#define log_set_format(formatter) Log::Logger<char>::formatter_set(formatter)

#define log_set_file_limit(fileLimit) Log::Logger<char>::file_limit_set(fileLimit)
#define log_enable_reset_file(enable_reset_file)	\
	Log::Logger<char>::enable_reset_file(enable_reset_file)
#define log_set_output(path)               \
	Log::Logger<char>::log_output_set(path); \
	Log::Logger<char>::log_format_set()
#define log_set_priority(severity) 		 \
	Log::Logger<char>::log_priority_set(static_cast<Log::LogPriority>(severity))

#define log_fatal(...) Log::Logger<char>::log(Log::LogPriority::Fatal, __VA_ARGS__)
#define log_error(...) Log::Logger<char>::log(Log::LogPriority::Error, __VA_ARGS__)
#define log_warn(...) Log::Logger<char>::log(Log::LogPriority::Warning, __VA_ARGS__)
#define log_info(...) Log::Logger<char>::log(Log::LogPriority::Info, __VA_ARGS__)
#define log_ver(...) Log::Logger<char>::log(Log::LogPriority::Verbose, __VA_ARGS__)
#define log_debug(...) Log::Logger<char>::log(Log::LogPriority::Debug, __VA_ARGS__)
#define log_trace(...) Log::Logger<char>::log(Log::LogPriority::Trace, __VA_ARGS__)
#endif

#if not defined _WIN32
#define log_set_format(formatter) Log::Logger<wchar_t>::formatter_set(formatter)
#define log_enable_reset_file(enable_reset_file)	\
	Log::Logger<wchar_t>::enable_reset_file(enable_reset_file)
#define log_set_output(path)                  \
	Log::Logger<wchar_t>::log_output_set(path); \
	Log::Logger<wchar_t>::log_format_set()
#define log_set_file_limit(fileLimit) 		\
	Log::Logger<wchar_t>::file_limit_set(fileLimit)

#define log_set_priority(severity)			\
	Log::Logger<wchar_t>::log_priority_set(static_cast<Log::LogPriority>(severity))

#define log_fatal(...) Log::Logger<wchar_t>::log(Log::LogPriority::Fatal, __VA_ARGS__)
#define log_error(...) Log::Logger<wchar_t>::log(Log::LogPriority::Error, __VA_ARGS__)
#define log_warn(...) Log::Logger<wchar_t>::log(Log::LogPriority::Warning, __VA_ARGS__)
#define log_info(...) Log::Logger<wchar_t>::log(Log::LogPriority::Info, __VA_ARGS__)
#define log_ver(...) Log::Logger<wchar_t>::log(Log::LogPriority::Verbose, __VA_ARGS__)
#define log_debug(...) Log::Logger<wchar_t>::log(Log::LogPriority::Debug, __VA_ARGS__)
#define log_trace(...) Log::Logger<wchar_t>::log(Log::LogPriority::Trace, __VA_ARGS__)
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
