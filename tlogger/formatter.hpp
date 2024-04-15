/**
 * @file formatter.hpp
 * @author Berker BOYACI (berker.boyaci@smartalpha.ai)
 * @brief
 * @version 1.0.0
 * @date 2024-04-15
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef FORMATTER_API_HPP
#define FORMATTER_API_HPP


#include <sstream>
#include <chrono>
#include <type_traits>
#include <codecvt>

namespace Log
{

	// formatType enum class
	// possible ways to format type : LINE , MSG , TIME , FUNC
	enum class formatType
	{
		LINE,
		MSG,
		TIME,
		FUNC
	};

#ifndef _T
#define __T(x) L##x
#define _T(x) __T(x)
#endif

#define _CPP_CONCAT(x, y) x##y
#define CPP_CONCAT(x, y) _CPP_CONCAT(x, y)

	template <size_t SZ0, size_t SZ1>
	constexpr auto _stringlit(char c,
							  const char (&s0)[SZ0],
							  const wchar_t (&s1)[SZ1]) -> const char (&)[SZ0]
	{
		(void)c;
		(void)s1;
		return s0;
	}

	template <size_t SZ0, size_t SZ1>
	constexpr auto _stringlit(wchar_t c,
							  const char (&s0)[SZ0],
							  const wchar_t (&s1)[SZ1]) -> const wchar_t (&)[SZ1]
	{
		(void)c;
		(void)s0;
		return s1;
	}

#define stringlit(code_unit, lit) \
	_stringlit(code_unit(), lit, CPP_CONCAT(L, lit))

	// Class Formatter
	// Log message Formatter class
	// f :function name  / l : line / m : message / t : time
	// Example :
	// Formatter fmt;
	// fmt.getformatter("%t %l %m %t")
	// auto fmt = fmt.format(line,func,...args);
	template <typename T>
	class Formatter
	{
	public:
        /****************************************************************************
         * Special Members
         ***************************************************************************/

        Formatter() noexcept = default;
		Formatter(const Formatter &) noexcept = delete;
		Formatter &operator=(const Formatter &) = delete;

        /****************************************************************************
         * Public Members
         ***************************************************************************/

        /*
		* Get format type as basic_string<T>
		* @param fmtStr : basic_string<T>
		*/
		static void formatter_set(std::basic_string<T> t_fmt) noexcept
		{
			m_fmt = t_fmt;
		}

        /*
        *
        *
        */
		static std::basic_string<T> formatter_get()
		{
			return m_fmt;
		}

		/*
		* Format given arguments with defined parameters and return as basic_string<T>
		* @param ...args: Variadic template arguments
		*/
		template <typename... Args>
		static std::basic_string<T> format(Args &&...args)
		{
			std::basic_string<T> t_format = m_fmt;

			// Message
			size_t found_message = t_format.find(stringlit(T, "%m"));
			if (std::string::npos != found_message)
			{
				std::basic_ostringstream<T> oss;
				using unused = int[];

				(void)unused{
					0, (oss << args << " ", 0)...};
				t_format = find_and_replace(std::move(t_format),
                                            stringlit(T, "%m"),
                                            oss.str());
			}

			// Time ->
			size_t found_time = t_format.find(stringlit(T, "%t"));
			if (found_time != std::string::npos)
			{

				std::basic_ostringstream<T> oss;
#if __cplusplus >= 201703L
				if constexpr (true == std::is_same<T, char>::value)
				{
					oss << time_point_as_string(
                            std::chrono::system_clock::now()).c_str()
                        << " ";
				}
#else
				if (std::is_same<T, char>::value)
				{
					oss << time_point_as_string(
                            std::chrono::system_clock::now()).c_str()
                        << " ";
				}
#endif

#if __cplusplus >= 201703L
				if constexpr (true == std::is_same<T, wchar_t>::value)
				{
					oss << time_point_as_string(
                            std::chrono::system_clock::now()).c_str()
                        << " ";
				}
#else
				if (true == std::is_same<T, wchar_t>::value)
				{
					oss << time_point_as_wstring(
                            std::chrono::system_clock::now()).c_str()
                         << " ";
				}
#endif

				t_format = find_and_replace(std::move(t_format),
                                            stringlit(T, "%t"),
                                            oss.str());
			}

			return t_format;
		}

	protected:

		/*
        * find a substr in first argument and replace with last argument
        * param t_format: basic_string<T>
        * param t_find: basic_string<T>
        * param t_replace: basic_string<T>
        * return std::move(t_format): basic_string<T>
        */
		static std::basic_string<T> find_and_replace(std::basic_string<T> t_format,
                                                     const std::basic_string<T> &t_find,
                                                     const std::basic_string<T> &t_replace)
		{

			t_format.replace(t_format.find(t_find),
                             t_find.length(),
                             t_replace);
			return t_format;
		}

        /*
        *
        *
        *
        */
		static std::wstring wtos(std::string &t_value)
		{
#if defined _WIN32
			const size_t c_size = t_value.size() + 1;
			std::wstring wc;
			wc.resize(c_size);
			size_t c_size_1;
			mbstowcs_s(&c_size_1,
                       (wchar_t *)&wc[0],
                       c_size, t_value.c_str(),
                       c_size);
			wc.pop_back();
			return wc;
#elif defined __GNUC__
			std::wstring_convert<std::codecvt_utf8<wchar_t>,
			                     wchar_t> str_converter;
			return str_converter.from_bytes(t_value);
#endif
		}

		/*
        * take time point from argument, convert to basic_string<char> format and return
        * param : chrono::system_clock::timepoint
        * return : basic_string<char>
        */
		static std::basic_string<char> time_point_as_string(
                const std::chrono::system_clock::time_point &t_time_point)
		{
			std::time_t t = std::chrono::system_clock::to_time_t(t_time_point);
			char time_buffer[30];
#if defined _WIN32
			ctime_s(time_buffer, sizeof(time_buffer), &t);
#elif defined __GNUC__
			ctime_r(&t, time_buffer);
#endif
			std::basic_string<char> ts = time_buffer;
			ts.resize(ts.size() - 1);
			return ts;
		}

		/*
        * take time point from argument, convert to basic_string<wchar_t> format and return
        * param : chrono::system_clock::timepoint
        * return : basic_string<wchar_t>
        */
		static std::basic_string<wchar_t> time_point_as_wstring(
                const std::chrono::system_clock::time_point &t_time_point)
		{
			std::time_t t = std::chrono::system_clock::to_time_t(t_time_point);
			char time_buffer[30];
#if defined _WIN32
			ctime_s(time_buffer, sizeof(time_buffer), &t);
#elif defined __GNUC__
			ctime_r(&t, time_buffer);
#endif
			std::basic_string<char> ret = time_buffer;
			std::basic_string<wchar_t> ts = wtos(ret);
			ts.resize(ts.size() - 1);
			return ts;
		}

	protected:

        /*
        *
        *
        */
		static std::basic_string<T> m_fmt;
	}; // end of class

	// Initialize static data members
	template <typename T>
	std::basic_string<T> Formatter<T>::m_fmt = stringlit(T, "%m %t");

} // end of Log namespace


#endif // FORMATTER_API_HPP

/* end_of_file */
