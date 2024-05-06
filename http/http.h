#ifndef _LOG_H
#define _LOG_H
#include<string>
#include<stdint.h>
#include<stdarg.h>
#include<memory>
#include<list>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
namespace fxk
{
	//日志级别
	class LogLevel {
	public:
		enum Level {

			FATAL = 0,
			ALTER = 100,
			CRIT = 200,
			ERROR=300,
			WARN=400,
			NOTICE=500,
			INFO=600,
			DEBUG=700,
			NOTSET=800,

		};
		static const char* ToString(LogLevel::Level level);
		static LogLevel::Level FromString(const std::string& str);
	};



	//日志器
	class Logger {
		
	public:
		typedef std::shared_ptr<Logger> ptr;
		typedef Spinlock MutexType;
		Logger(const std::string& name = "root");
		void log(LogLevel::Level level, LogEvent::ptr event);
		void debug(LogEvent::ptr event);
		void info(LogEvent::ptr event);
		void warn(LogEvent:: ptr event);
		void error(LogEvent::ptr event);
		void fatal(LogEvent::ptr event);
		void addAppender(LogAppender::ptr appender);
		void delAppender(LogAppender::ptr appender);
		void clearAppenders();
		LogLevel::Level getLevel()const { return m_level; }
		void setLevel(LogLevel::Level val) { m_level = val; }
		const std::string getName()const { return m_name; }
		void setFormatter(LogFormatter::ptr val);
		void setFormatter(const std::string& val);
		LogFormatter(const std::string &val);
		LogFormatter::ptr getFormatter();
		std::string toYamlString();

	private:
		std::string m_name;						//日志名称
		LogLevel::Level m_level;		//日志级别
		MutexType m_mutex;
		std::list< LogAppender::ptr> m_appenders;//appenderds集合
		LogFormatter::ptr m_formatter;
		Logger::ptr m_root;
	};



	//日志输出地
	class LogAppender {
	public:
		typedef std::shared_ptr<LogAppender> ptr;
		typedef Spinlock MutexType;
		virtual ~LogAppender()
		{
		}
		virtual void log(std::shared_ptr<Logger>logger,LogLevel::Level level, LogEvent::ptr event) = 0;
		virtual std::string toYamlString() = 0;
		void setFormatter(LogFormatter::ptr val);
		LogFormatter::ptr getFormatter();
		LogLevel::Level getLevel()const { return m_level; }
		void setLevel(LogLevel::Level val) { m_level = val; }

	protected:
		LogLevel::Level m_level = LogLevel::DEBUG;
		bool m_hasFormatter = false;
		MutexType m_mutex;
		LogFormatter::ptr m_formatter;
	};


	//日志事件
	class LogEvent {

	public:
		typedef std::shared_ptr<LogEvent> ptr;
		LogEvent::LogEvent(std::shared_ptr<Logger>logger, LogLevel::Level level, const char* file, int32_t line, uint32_t elapse, uint32_t thread_id, uint32_t fiber_id, uint64_t time, const std::string& thread_name) :m_file(file)
			, m_line(line), m_elapse(elapse), m_threadId(thread_id), m_fiberId(fiber_id), m_time(time), m_threadName(thread_name), m_logger(logger), m_level(level);
		LogLevel::Level getLevel()const { return m_level; }
		std::string getContent()const { return m_ss.str(); }
		std::string getFile()const { return m_file; }
		int32_t getline()const { return m_line; }
		int64_t getElapse()const { return m_elapse; }
		uint32_t getThreadld()const { return m_threadld; }
		uint64_t getFiberld()const { return m_fiberld; }
		time_t getTime()const { return m_time; }
		const std::string& getThreadName()const { return m_threadName; }
		std::stringstream& getSS() { return m_ss; }
		std::shared_ptr<Logger>getLogger()const { return m_logger; }
		void format(const char* fmt, ...);
		void format(const char* fmt, va_list al);

	private:
		LogLevel::Level m_level;
		std::stringstream m_ss;
		const char* m_file = nullptr;
		int32_t m_line = 0;
		uint32_t m_elapse = 0;
		int32_t m_threadId = 0;
		uint32_t m_fiberId = 0;
		uint64_t m_time = 0;
		std::string m_threadName;
		std::shared_ptr<Logger> m_logger;

	};
	class LogEventWrap {
	public:
		LogEventWrap(LogEvent::ptr e);
		~LogEventWrap();
		LogEvent::ptr getEvent()const { return m_event; }
		std::stringstream& getSS();
	private:
		LogEvent::ptr m_event;
	};



	//日志输出格式
	class LogFormatter {

	public:
		typedef  std::shared_ptr<LogFormatter> ptr;
		LogFormatter(const std::string& pattern = "%d{%Y-%m-%d %H:%M;%S}[%rms]%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
		std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
	std::ostream& format(std::ostream& os, std::shared_ptr<Logger>logger,LogLevel::Level level,LogEvent::ptr event);

	public:
		class FormatItem{
	public:
		typedef std::shared_ptr<FormatItem> ptr;
		virtual ~FormatItem(){}
		virtual void format(std::ostream& os,std::shared_ptr<Logger>logger,LogLevel::Level level, LogEvent::ptr event) = 0;
	};
		void init();
		bool isError()const { return m_error; }
		const std::string getPattern()const { return m_pattern; }
	private:
		std::string m_pattern;
		std::vector<FormatItem::ptr> m_items;
		bool m_error = false;
	};
	//输出到控制台Appender
	class StdoutLogAppender:public LogAppender {
	public:
		typedef std::shared_ptr<StdoutLogAppender> ptr;
	 void log(Logger::ptr logger,LogLevel::Level level, LogEvent::ptr event)override;
	 std::string toYamlString()override;
	};
	//输出到文件Apppender
	class FileLogAppender:public LogAppender {
	public:
		typedef std::shared_ptr<FileLogAppender> ptr;
		FileLogAppender(const std::string& filename);
		 void log(Logger::ptr logger,LogLevel::Level, LogEvent::ptr event)override;
		 bool reopen();
	private:
		std::string m_filename;
		std::ofstream  m_filestream;
		uint64_t m_lastTime = 0;
	};
	class LoggerManager {

	public:
		typedef Spinlock MutexType;
		LoggerManager();
		Logger::ptr getLogger(const std::string& name);
		void init();
		Logger::ptr getRoot()const { return m_root; }
	std:: string toYamlString();
	private:
		MutexType m_mutex;
		std::map < std::string, Logger:: ptr > m_loggers;
		Logger::ptr m_root;

	};
	//单例模式
	typedef fxk::Singleton<LoggerManager>LogerMgr;
}

#endif

