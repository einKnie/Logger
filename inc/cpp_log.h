#ifndef _CPP_LOGGER_H_
#define _CPP_LOGGER_H_

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

// #define DEBUG
#if defined DEBUG
  #define PRINT_DEBUG(...) printf(__VA_ARGS__)
#else
  #define PRINT_DEBUG(...)
#endif

class Logger {
public:

  /// Enums
  enum { EErr = 0, ENoErr };        ///< return values
  typedef enum { ELogDisable,       ///< no logging
                 ELogError,         ///< just errors
                 ELogWarn,          ///< errors and warnings
                 ELogVerbose,       ///< errors, warnings, and notices
                 ELogDebug          ///< errors, warnings, notices, and debug messages
               } level_e;           ///< loglevel

  typedef enum { ELogStyleNone,     ///< print just the plain message
                 ELogStyleMinimal,  ///< output minimalistic logging
                 ELogStyleDefault,  ///< time && level
                 ELogStyleVerbose,  ///< output verbose logging
                 ELogStyleUser      ///< user-defined style
               } style_e;           ///< logstyle

  /// Constants
  static const int   CMaxPathLen        = PATH_MAX;   ///< max path length
  static const int   CMaxMsgLen         = 500;        ///< max logger line length
  static const int   CMaxHeaderLen      = 100;
  static const int   CMaxPrefixLen      = 10;
  static const int   CMaxSepLen         = 10;
  static const int   CMaxPatternLen     = 80;
  static const int   CMaxPatternItems   = 10;
  static const int   CMaxPatternItemLen = 10;
  static const char *CLogMsgError;
  static const char *CLogMsgWarning;
  static const char *CLogMsgNotice;
  static const char *CLogMsgDebug;
  static const char *CLogMsgAlways;
  static const level_e CLogLevelDefault = ELogDebug;
  static const style_e CLogStyleDefault = ELogStyleNone;

  /// Logger configuration
  typedef struct cfgLog {

    enum {
      ELevelDefault = 0,  ///< default level string, e.g. Notice
      ELevelLower,        ///< lower case level string, e.g. notice
      ELevelUpper         ///< upper case level string, e.g. NOTICE
    };

    Logger::level_e logLevel;              ///< loglevel
    Logger::style_e logStyle;              ///< logstyle
    bool logToFile;                        ///< flag for file logging

    char logfile[Logger::CMaxPathLen];     ///< path to logfile
    char prefix[Logger::CMaxPrefixLen];    ///< set prefix
    char postfix[Logger::CMaxPrefixLen];   ///< postfix, must include newline if needed
    char separator[Logger::CMaxSepLen];    ///< set separator

    char pattern[Logger::CMaxPatternLen];  ///< log msg pattern
    char userDefinedPatternItem[Logger::CMaxPatternItemLen];  ///< user defined pattern item

    // advanced configuration
    int  logLevelCase;                     ///< print level in default, lower- or uppercase


    /// Constructor
    cfgLog() {

      logLevel = Logger::CLogLevelDefault;
      logStyle = Logger::CLogStyleDefault;

      logToFile = false;

      memset(prefix, '\0', sizeof(prefix));
      memset(postfix, '\0', sizeof(postfix));
      memset(logfile, '\0', sizeof(logfile));
      memset(separator, '\0', sizeof(separator));
      memset(pattern, '\0', sizeof(pattern));
      memset(userDefinedPatternItem, '\0', sizeof(userDefinedPatternItem));
      strcpy(separator, " | ");
      strcpy(postfix, "\n");

      logLevelCase  = ELevelDefault;
    }

  } CfgLog;

  /// Default constructor
  Logger();

  /// Constructor with CfgLog
  Logger(CfgLog *cfg);

  /// Constructor
  /// @param[in] logfile path to logfile, may be NULL for stdout
  /// @param[in] level desired loglevel [0...4]
  /// @param[in] style desired logstyle [0...2]
  Logger(const char *logfile, level_e level, style_e style);

  /// Destructor
  ~Logger();

  void error(const char *fmt, ...);
  void warning(const char *fmt, ...);
  void notice(const char *fmt, ...);
  void debug(const char *fmt, ...);
  void always(const char *fmt, ...);

  level_e getLevel(void);
  style_e getStyle(void);
  void setLevel(level_e level);
  void setStyle(style_e style);

  int setPattern(const char *pattern);
  char *getPattern(void);

private:

  typedef enum {
        EPatInvalid = 0,
        EPatSeparator,
        EPatPID,
        EPatTime,
        EPatLevel,
        EPatMsg,
        EPatUser,
        EPatPrefix,
        EPatEnd
      } pattern_e;

  CfgLog *m_cfg;             ///> logger config
  FILE *m_fd;                ///> file descriptor
  va_list m_args;            ///> current fmt arguments
  bool m_removeCfg;          ///> flag to remove cfgLog in case it was created at ctor

  pattern_e m_pattern[CMaxPatternItems];

  /// Initialize logger
  void init(void);

  /// Initialize configuration from pattern
  /// @param[in] pattern
  /// @return ENoErr on success, EErr on failure
  int initPattern(const char *pattern);

  /// Initialize a default logstyle
  /// sets all style setting to defaults depending on logstyle set
  void initStyle(void);

  void addUsr(char *msg);
  void addMsg(char *msg, const char *fmt);
  void addTime(char *msg);
  void addPID(char *msg);
  void addLevel(char *msg, const char *level);
  void addPostfix(char *msg);
  void addPrefix(char *msg);
  void addSeparator(char *msg);

  /// Print a log message
  /// @param [in] msg to print
  void log(const char *msg);

  /// Construct a log message
  /// @param[in,out] msg contains the contructed log msg after call
  /// @param[in] fmt the msg payload
  /// @param[in] level msg level
  void constructMsg(char *msg, const char *fmt, const char *level);

};

#endif //_CPP_LOGGER_H_
