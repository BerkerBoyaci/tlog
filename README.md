## tlog
header only C++ logging library
## Usage
####  1-Set output stream : 
 for stream to file 
 <br/> `Logger<wchar_t>::set_log_output(L"log/log.txt");`
 <br/> for stream to console no need to call set_log_output function. Stream to console as default. <br/>
#### 2-Set priority level
`void set_log_priority(LogPriority t_logPriority); `
<br/> set_log_priority : logger severity upper limit. All log messages have its own severity and if it is higher than the limit those messages are dropped.
<br/> `Logger<wchar_t>::set_log_priority(LogPriority::Debug); `

 #### 3-Create instance
Create an singleton instance,, which ensures that only one object of its kind exists and provides a single point
<br/> ` auto log = Logger<char>::get_instance(); `

 #### Set format (Optional)
 Formatters specify the layout of log records in the final output.
<br/>`log->set_formatter("%m %t"); `
<br/> %m : message ,  %t : time 	<br/>
`
 
 #### Set limit size of file (Optional)
 The File size limit property of the File field enables you to set size limits on the file, in byte.
	<br/> ` log->set_file_limit(100); `

#### 4-Log
Logs a message with log priority level. The other arguments are interpreted as variadic arguments.
 <br/>` log->log(LogPriority::Error, "Lorem ipsum", "tellus felis condimentum odio, : " `
 
 ## Basic Sample 
 ` 	Logger<char>::set_log_output("log/log.txt"); `
<br/>  ` 	Logger<char>::set_log_priority(LogPriority::Debug); `
<br/>  ` 	auto log = Logger<char>::get_instance(); `
<br/>  ` 	log->set_file_limit(100); `
<br/>  ` 	log->set_formatter("%t %m"); `
<br/>  ` 	log->log(LogPriority::Error, __LINE__, __FILE__, "tellus felis condimentum odio, : " ); `
