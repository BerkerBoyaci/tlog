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

    /**
     * @brief Enum class representing different types of formatting options.
     *
     * This enum class defines possible ways to format log messages, including LINE, MSG, TIME, and FUNC.
     */
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

    /**
     * @brief Template function to select and return a string literal based on the character type.
     *
     * This template function selects and returns a string literal based on the character type (`char` or `wchar_t`).
     * The function chooses the appropriate string literal based on the provided character type.
     *
     * @tparam SZ0 The size of the `char` array (s0).
     * @tparam SZ1 The size of the `wchar_t` array (s1).
     * @param c The character type (`char` or `wchar_t`) used to determine the return type.
     * @param s0 The `char` array representing the string literal.
     * @param s1 The `wchar_t` array representing the string literal.
     * @return The selected string literal as either `const char (&)[SZ0]` or `const wchar_t (&)[SZ1]`.
     */
	template <size_t SZ0, size_t SZ1>
	constexpr auto _stringlit(char c,
							  const char (&s0)[SZ0],
							  const wchar_t (&s1)[SZ1]) -> const char (&)[SZ0]
	{
		(void)c;
		(void)s1;
		return s0;
	}

    /**
     * @brief Template function to select and return a string literal based on the character type.
     *
     * This template function selects and returns a string literal based on the character type (`char` or `wchar_t`).
     * The function chooses the appropriate string literal based on the provided character type.
     *
     * @tparam SZ0 The size of the `char` array (s0).
     * @tparam SZ1 The size of the `wchar_t` array (s1).
     * @param c The character type (`char` or `wchar_t`) used to determine the return type.
     * @param s0 The `char` array representing the string literal.
     * @param s1 The `wchar_t` array representing the string literal.
     * @return The selected string literal as either `const char (&)[SZ0]` or `const wchar_t (&)[SZ1]`.
     */
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

        /**
         * @brief Sets the formatter string for log message formatting.
         *
         * This function sets the formatter string used for formatting log messages.
         *
         * @param t_fmt The formatter string of type `std::basic_string<T>` (where `T` can be `char` or `wchar_t`).
         */
		static void formatter_set(std::basic_string<T> t_fmt) noexcept
		{
			m_fmt = t_fmt;
		}

        /**
         * @brief Gets the formatter string used for log message formatting.
         *
         * This function retrieves the current formatter string used for formatting log messages.
         *
         * @return The formatter string of type `std::basic_string<T>` (where `T` can be `char` or `wchar_t`).
         */
		static std::basic_string<T> formatter_get()
		{
			return m_fmt;
		}

        /**
         * @brief Formats the log message string using defined parameters and returns as a basic_string<T>.
         *
         * This function formats a log message string using the specified format string and the provided variadic arguments.
         * The format string can contain placeholders such as "%m" for message content and "%t" for timestamp.
         *
         * @tparam Args Variadic template arguments representing the values to be formatted into the message.
         * @param args Variadic template arguments to be inserted into the format string.
         * @return The formatted log message string of type `std::basic_string<T>` (where `T` can be `char` or `wchar_t`).
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
         * @brief Finds a substring in the input string and replaces it with another substring.
         *
         * This function searches for the first occurrence of the substring `t_find` within the input string `t_format`.
         * If found, it replaces the substring with the provided replacement string `t_replace`.
         *
         * @param t_format The input string of type `std::basic_string<T>` (where `T` can be `char` or `wchar_t`).
         * @param t_find The substring to search for within `t_format`.
         * @param t_replace The replacement substring to substitute in place of `t_find`.
         * @return The modified string `t_format` after the replacement operation.
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

        /**
         * @brief Converts a `std::string` value to `std::wstring`.
         *
         * This function converts a given `std::string` value to `std::wstring`.
         * On Windows platforms (`_WIN32`), it uses `mbstowcs_s` for conversion.
         * On other platforms (such as Unix/Linux with `__GNUC__`), it uses `std::wstring_convert`.
         *
         * @param t_value The input `std::string` value to convert to `std::wstring`.
         * @return The converted `std::wstring` value.
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

        /**
         * @brief Converts a `std::chrono::system_clock::time_point` to a `std::basic_string<char>` timestamp.
         *
         * This function takes a `std::chrono::system_clock::time_point` as input and converts it to a formatted
         * timestamp string of type `std::basic_string<char>`.
         * The resulting timestamp string represents the time point in a human-readable format.
         *
         * @param t_time_point The input `std::chrono::system_clock::time_point` to convert.
         * @return The formatted timestamp string as `std::basic_string<char>`.
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

        /**
         * @brief Converts a `std::chrono::system_clock::time_point` to a `std::basic_string<wchar_t>` timestamp.
         *
         * This function takes a `std::chrono::system_clock::time_point` as input and converts it to a formatted
         * timestamp string of type `std::basic_string<wchar_t>`.
         * The resulting timestamp string represents the time point in a human-readable format.
         *
         * @param t_time_point The input `std::chrono::system_clock::time_point` to convert.
         * @return The formatted timestamp string as `std::basic_string<wchar_t>`.
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

        /**
         * @brief Holds formatter string used for log message formatting.
         *
         * This static member variable (`m_fmt`) holds the formatter string used by the logger class for formatting log messages.
         * The type of the formatter string is `std::basic_string<T>`, where `T` can be either `char` or `wchar_t`.
         */
		static std::basic_string<T> m_fmt;
	}; // end of class

	// Initialize static data members
	template <typename T>
	std::basic_string<T> Formatter<T>::m_fmt = stringlit(T, "%m %t");

} // end of Log namespace


#endif // FORMATTER_API_HPP

/* end_of_file */
