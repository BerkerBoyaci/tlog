#pragma once
#include <mutex>
#include <string>
#include <memory>
#include <fstream>
#include <locale>
#include <codecvt>
#include <iostream>
#include <vector>
#include "formatter.h"
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

	constexpr auto operator "" _kb(unsigned long long value) {
		return value * 1024;
	}


	constexpr auto operator "" _mb(unsigned long long value) {
		return value * 1024 * 1024;
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
	class MakeDirectory
	{
	public:
		virtual void mkdir(const std::string &fileName) = 0;
		virtual void mkdir(const std::wstring &fileName) = 0;
	};

	class MakeDirectoryWindows final : public MakeDirectory
	{
	public:
		static void mkdir(const std::string &fileName) override
		{
			_mkdir(t_path.c_str());
		}
		static void mkdir(const std::wstring &fileName) override
		{
			_wmkdir(t_path.c_str());
		}
	};

	class MakeDirectoryLinux final : public MakeDirectory
	{
	public:
		void mkdir(const std::string &fileName) override
		{
			mkdir(t_path.c_str(), 0777);
		}
		void mkdir(const std::wstring &fileName) override
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> strconverter;
			mkdir(strconverter.to_bytes(t_path).c_str(), 0777);
		}
	};

	std::unique_ptr<MakeDirectory> MakeDirectoryFactory()
	{
		std::unique_ptr<MakeDirectory> ptr;
#if defined _MSC_VER
		ptr = std::make_unique<MakeDirectoryWindows>();
#else
		ptr = std::make_unique<MakeDirectoryLinux>();
#endif
		return ptr;
	}

#endif


	// Class logger<>
	// record of variadic arguments to selected stream in formatted string
	// Example:
	// 	logger<wchar_t>::setLogOutput(L"log.txt");
	//	logger<wchar_t>::setLogPriority(LogPriority::Debug);
	//	auto log = logger<wchar_t>::getInstance();
	//	log->set_formatter(L"%f %l %m %t");
	//	log->log(LogPriority::Error, __LINE__, _T(__FILE__), L"tellus felis condimentum odio, : ", 1, );
	template <typename T = char>
	class logger
	{
	public:
		logger(const logger &) = delete;
		logger &operator=(const logger &) = delete;


		virtual ~logger() noexcept
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_ofs.close();
		}

		/*
		 * Get single instance or create new object if not created
		 * @return: single instance of logger.
		 */
		static std::shared_ptr<logger> get_instance()
		{
			if (m_logger_instance == nullptr)
				m_logger_instance = std::shared_ptr<logger<T>>(new logger<T>{});
			return m_logger_instance;
		}

		/*
		 * Not set or call for stream to console
		 * Set log path as std::basic_string for stream to file
		 * @param t_filePath : file path.
		 */
		static void set_log_output(const std::string& t_filePath)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
			m_log_path = t_filePath;
		}

		/*
		 * Set log priority level.
		 * @param t_logPriority: enum class LogPriority.
		 */
		static void set_log_priority(LogPriority t_logPriority)
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_log_priority = t_logPriority;
		}

		/*
		 * For Quiet log priority.
		 * @param messageLevel:  Log Level.
		 */
		static void log(LogPriority messageLevel) 
		{
			(void)messageLevel;
		} // 

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

		/*
		 * Get format type and pass to Formatter::getFormatter() function
		 * default as %t %m
		 * @param t_fmt :  format style as string.
		 */
		static void set_formatter(const std::basic_string<T> &t_fmt)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
			m_format.get_formatter(t_fmt);
		}

		/*
		 * Set file's limit as byte.
		 * @param t_fileLimit :  file size limit as byte.
		 */
		static void set_file_limit(unsigned long long t_fileLimit)
		{
			std::lock_guard<std::mutex> _lock(m_mutex);
			m_max_file_size = t_fileLimit;
		}

		/*
		 * Set output format
		 * if m_log_path is empty then stream to console
		 * if m_log_path is not empty then stream to file
		 */
		static void set_log_format()
		{

			if (m_log_path.size() == 0)
			{
				m_log_output = LogOutput::Console;
			}
			else
			{
				m_log_output = LogOutput::File;
				openFile(m_log_path);
			}
		}

	protected:
		/*
		 * Construct logger class as singleton.
		 * client can not access default constructor
		 * if m_log_path string is empty select the stream as console
		 * else select the stream as file
		 */
		logger() noexcept
		{
			set_log_format();
		}

#if __cplusplus >= 201703L
		/*
		 * For C++17 and C++20 versions
		 * Open file in UTF-8 standart in ofstream write or append mode
		 * if the parent path not exist then create directory
		 * @param t_path :  file path
		 */
		static void openFile(std::filesystem::path t_path)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);
			try
			{
				auto t_root = t_path.parent_path();

				if (!std::filesystem::exists(t_path) && !t_root.empty())
				{
					std::filesystem::create_directories(t_root.string());
				}
#if defined _MSC_VER
				m_ofs.imbue(std::locale(std::locale::empty(), new std::codecvt<wchar_t, char, mbstate_t>("en_US.utf8")));
#elif defined __GNUC__
				m_ofs.imbue(std::locale(std::locale(), new std::codecvt<wchar_t, char, mbstate_t>));
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
				std::cout << "Unknown error in logger::openFile()\n";
			}
		}
#else
		/*
		 * For C++14 and previous versions
		 * Open file in UTF-8 standart in ofstream write or append mode
		 * if the parent path not exist then create directory
		 * @param t_path : file path
		 */
		static void open_file(const std::string &t_path)
		{

			std::lock_guard<std::mutex> _lock(m_mutex);

			char delim = t_path.find('/') != std::string::npos ? '/' : '\\';
			auto ret = split(t_path, delim);
			std::string path;
			auto makeDir = MakeDirectoryFactory();
			for (int i = 0; i < ret.size() - 1; i++)
			{
				path += (i != 0) ? stringlit(T, "/") + ret[i] : ret[i];
				makeDir->mkdir(path);
			}
#if defined _MSC_VER
			m_ofs.imbue(std::locale(std::locale::empty(), new std::codecvt<wchar_t, char, mbstate_t>("en_US.utf8")));
#elif defined __GNUC__
			m_ofs.imbue(std::locale(std::locale(), new std::codecvt<wchar_t, char, mbstate_t>));
#endif
			m_ofs.open(t_path.c_str(), std::ofstream::out | std::ofstream::app);
			m_ofs.seekp(0, std::ios_base::end);
		}
#endif
		/*
		 * Pass argumants to set formatter::format() and take back as formatted string
		 * and write the string to choosen stream in constructor
		 * @param ...args: Variadic template arguments
		 */
		template <typename... Args>
		static void log_message(Args &&...args)
		{

			auto formattedStr = m_format.format(std::forward<Args>(args)...);

			formattedStr += '\n';
			if (m_log_output == LogOutput::File)
			{
				if ((static_cast<unsigned long long>(m_ofs.tellp()) + formattedStr.length()) >= m_max_file_size)
				{
					m_ofs.close();
					m_ofs.open(m_log_path, std::ios::out | std::ios::trunc);
				}
				m_ofs << formattedStr.c_str();
			}
			if (m_log_output == LogOutput::Console)
				StreamWrapper<T>::tout << formattedStr.c_str();
		}

		/*
		 * Divides a String into an ordered list of substrings, puts these substrings into
		 * an vector of string
		 * @param strSplit: basic_string<T>
		 * @param Delim: Template argument
		 * @return : result : vector<basic_string<T>>
		 */
		static std::vector<std::basic_string<T>> split(const std::basic_string<T> &strSplit, T delim)
		{
			std::vector<std::basic_string<T>> result;
			std::basic_stringstream<T> ss(strSplit);
			std::basic_string<T> item;

			while (getline(ss, item, delim))
			{
				result.push_back(item);
			}
			return result;
		}

	protected:

	    /**
         * @brief holds critical section.
         * 
         */
		static std::mutex m_mutex;

		/**
         * @brief Holds format type.
         * 
         */
		static formatter<T> m_format;
		
		
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
         * @brief 
         * 
         */
		static std::basic_ofstream<T> m_ofs;
		
		/**
         * @brief Holds singleton instance.
         * 
         */
		static std::shared_ptr<logger<T>> m_logger_instance;
		
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

	// Macro definitions for logger::log()
	
#define LOG_QUIET()
#define LOG_SET_FORMAT_C(formatter) Log::logger<char>::set_formatter(formatter)
#define LOG_SET_FILE_LIMIT_C(fileLimit) Log::logger<char>::set_file_limit(fileLimit)
#define LOG_SET_OUTPUT_C(path)              \
	Log::logger<char>::set_log_output(path); \
	Log::logger<char>::set_log_format()
#define LOG_SET_PRIORITY_C(severity) \
	Log::logger<char>::set_log_priority(static_cast<Log::LogPriority>(severity))

#define LOG_FATAL_C(...) Log::logger<char>::log(Log::LogPriority::Fatal, __VA_ARGS__)
#define LOG_ERROR_C(...) Log::logger<char>::log(Log::LogPriority::Error, __VA_ARGS__)
#define LOG_WARNING_C(...) Log::logger<char>::log(Log::LogPriority::Warning, __VA_ARGS__)
#define LOG_INFO_C(...) Log::logger<char>::log(Log::LogPriority::Info, __VA_ARGS__)
#define LOG_VERBOSE_C(...) Log::logger<char>::log(Log::LogPriority::Verbose, __VA_ARGS__)
#define LOG_DEBUG_C(...) Log::logger<char>::log(Log::LogPriority::Debug, __VA_ARGS__)
#define LOG_TRACE_C(...) Log::logger<char>::log(Log::LogPriority::Trace, __VA_ARGS__)

#define LOG_SET_FORMAT_W(formatter) Log::logger<wchar_t>::set_formatter(formatter)
#define LOG_SET_OUTPUT_W(path)                \
	Log::logger<wchar_t>::set_log_output(path); \
	Log::logger<wchar_t>::set_log_format()
#define LOG_SET_FILE_LIMIT_W(fileLimit) \
	Log::logger<wchar_t>::set_file_limit(fileLimit)

#define LOG_SET_PRIORITY_W(severity) \
	Log::logger<wchar_t>::set_log_priority(static_cast<Log::LogPriority>(severity))

#define LOG_FATAL_W(...) Log::logger<wchar_t>::log(Log::LogPriority::Fatal, __VA_ARGS__)
#define LOG_ERROR_W(...) Log::logger<wchar_t>::log(Log::LogPriority::Error, __VA_ARGS__)
#define LOG_WARNING_W(...) Log::logger<wchar_t>::log(Log::LogPriority::Warning, __VA_ARGS__)
#define LOG_INFO_W(...) Log::logger<wchar_t>::log(Log::LogPriority::Info, __VA_ARGS__)
#define LOG_VERBOSE_W(...) Log::logger<wchar_t>::log(Log::LogPriority::Verbose, __VA_ARGS__)
#define LOG_DEBUG_W(...) Log::logger<wchar_t>::log(Log::LogPriority::Debug, __VA_ARGS__)
#define LOG_TRACE_W(...) Log::logger<wchar_t>::log(Log::LogPriority::Trace, __VA_ARGS__)

	// Intialize static data members
	template <typename T>
	std::mutex logger<T>::m_mutex;
	template <typename T>
	std::basic_ofstream<T> logger<T>::m_ofs;
	template <typename T>
	unsigned long long logger<T>::m_max_file_size = 512 * 1024 *1024; // 512 MB
	template <typename T>
	std::string logger<T>::m_log_path{};
	template <typename T>
	LogPriority logger<T>::m_log_priority = LogPriority::Trace;
	template <typename T>
	LogOutput logger<T>::m_log_output = LogOutput::Console;
	template <typename T>
	std::shared_ptr<logger<T>> logger<T>::m_logger_instance = nullptr;
	template <typename T>
	formatter<T> logger<T>::m_format;

	// template alias for logger class
	using logger_w = logger<wchar_t>;
	using logger_c = logger<char>;

} // end of Log namespace
