//        __
//       / /   ___   __ _  __ _  ___ _ __
//      / /   / _ \ / _` |/ _` |/ _ \ '__|
//     / /___| (_) | (_| | (_| |  __/ |
//     \____/ \___/ \__, |\__, |\___|_|
//                  |___/ |___/        v1.0
//      <ramharter>

/// @file cpp_log.h
/// @brief Header file of the Cpp Logger and CfgLog

#ifndef _CPP_LOGGER_H_
#define _CPP_LOGGER_H_

#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <stdint.h>

#define ASCII_LOWER_START 97
#define ASCII_LOWER_END   122
#define ASCII_UPPER_START 65
#define ASCII_UPPER_END   90

// #define DEBUG
#if defined DEBUG
  #define PRINT_DEBUG(...) printf(__VA_ARGS__)
#else
  #define PRINT_DEBUG(...)
#endif

/// Basic struct containing constants
typedef struct cfgLog {

  // Constants
  static const int   CMaxPathLen        = PATH_MAX;   ///< max path length
  static const int   CMaxLogMsgLen      = 200;        ///< max logger line length
  static const int   CMaxPrefixLen      = 10;         ///< maximum length of the msg prefix
  static const int   CMaxSepLen         = 5;          ///< maximum length of the separator
  static const int   CMaxPatternItems   = 10;         ///< maximum number of pattern items which may be set
  static const int   CMaxPatternItemLen = 10;         ///< maximum length of a pattern item
  static const int   CMaxPatternIdLen   = 5;          ///< max length of a pattern identifier
  static const int   CMaxPatternLen     = CMaxPatternItems * CMaxPatternIdLen + 1; ///< 10*4 characters + null termination
  static const int   CMaxLogLevelStrLen = 10;
  static const int   CLogColorLen       = 15;

  static const char  CLogMsgLevel[][CMaxLogLevelStrLen]; ///< List of loglevel strings

} cfgLog_t;

/// @brief CfgLog class
///
/// A configuration class for the CPP Logger
class CfgLog : public cfgLog_t {
public:

  // Enums
  /// @brief Enumeration of available log levels
  typedef enum {
    ELogEmergency = 0,     ///< print always && emergency msgs
    ELogAlert,             ///< print always &&emergency && alert msgs
    ELogCritical,          ///< print always &&emerg, alert && critical msgs
    ELogError,             ///< print always &&emerg, alert, crit && errors
    ELogWarn,              ///< print always &&emerg, alert, crit, errors && warnings
    ELogNotice,            ///< print always &&emerg, alert, crit, errors, warnings && notices
    ELogInfo,              ///< print always &&emerg, alert, crit, errors, warnings, notices && infos
    ELogDebug,             ///< print always &&emerg, alert, crit, errors, warnings, notices, infos && debug msgs
    ELogAlways
  } level_e;               ///< loglevel

  /// @brief Enumeration of available profiles.<br>
  /// See @ref PatternExample on how to create and use a user-defined style.
  typedef enum {
    ELogProfileNone  = 0,  ///< print just the plain message
    ELogProfileMinimal,    ///< print level and message
    ELogProfileDefault,    ///< print time, level, and message
    ELogProfileVerbose,    ///< print pid, time, level, and message
    ELogProfileUser        ///< user-defined style
  } profile_e;             ///< logstyle

  /// @brief Enumeration of level string casing
  enum {
    ELevelCaseDefault = 0,  ///< default level string, e.g. Notice
    ELevelCaseLower,        ///< lower case level string, e.g. notice
    ELevelCaseUpper         ///< upper case level string, e.g. NOTICE
  };

  /// @brief Enumeration of available colors
  typedef enum {
    EColorBlack = 30,
    EColorRed,
    EColorGreen,
    EColorYellow,
    EColorBlue,
    EColorMagenta,
    EColorCyan,
    EColorWhite
  } color_e;                ///< terminal color values

  static const level_e   CLogLevelDefault   = ELogDebug;        ///< default loglevel
  static const profile_e CLogProfileDefault = ELogProfileNone;  ///< default profile

  // Members
  level_e logLevel;               ///< loglevel
  profile_e profile;              ///< logstyle
  bool logToFile;                 ///< enable file logging
  bool useColor;                  ///< enable colorful logging
  bool useUsrPattern;             ///< ebable user defined patterns

  color_e color;                  ///< if colorful logging is enabled, use specified color
  int  logLevelCase;              ///< print loglevel in default, lower- or uppercase

  char logfile[CMaxPathLen];      ///< path to logfile
  char prefix[CMaxPrefixLen];     ///< prefix
  char postfix[CMaxPrefixLen];    ///< postfix
  char separator[CMaxSepLen];     ///< separator
  char pattern[CMaxPatternLen];   ///< logging pattern

  // Methods
  /// @brief Constructor
  CfgLog();

  /// @brief Destructor
  ~CfgLog();

  /// @brief Add a user defined pattern
  /// @param [in] nr the position
  /// @param [in] pat a char array containing the pattern
  void addUsrPattern(int nr, const char *pat);

  /// @brief Retrieve a user defined pattern
  /// @param [in] nr the position
  /// @return a pointer to the user pattern at position nr
  char *getUsrPattern(int nr);

private:

  typedef struct usrPattern {
    char pat[CMaxPatternItemLen];
    int nr;
    struct usrPattern *next;

    usrPattern(int nr = 0, const char *pattern = NULL) {
      this->nr = nr;
      memset(pat, 0, sizeof(pat));
      if (pattern) {
        strncpy(pat, pattern, sizeof(pat) - 1);
      }
      next = NULL;
    }
  } UsrPattern;

  UsrPattern *usrPattern;         ///< head of linked list of user patterns

};

/// @brief Logger class
///
/// The Logger provides simple API calls for configuration and logging
/// @todo Implement log rotation (file size based or time based)
/// @todo Implement choice between new file or append to file
class Logger {
public:

  /// Return values used by Logger
  enum { EErr = 0, ENoErr };

  /// Default constructor
  Logger();

  /// @brief Constructor with CfgLog
  /// @param [in] cfg pointer to a CfgLog object
  Logger(CfgLog *cfg);

  /// @brief Extended Constructor
  /// @param[in] logfile path to logfile, may be NULL for stdout
  /// @param[in] level desired loglevel @ref CfgLog::level_e
  /// @param[in] style desired logstyle @ref CfgLog::profile_e
  Logger(const char *logfile, CfgLog::level_e level, CfgLog::profile_e style);

  /// Destructor
  ~Logger();

  /// @brief Initialize logger with CfgLog struct
  /// @param [in] cfg pointer to a CfgLog object
  /// @return EErr on failure, ENoErr on success
  /// @note In case initialization has failed, the logger is still usable and returns to the default state.
  int init(CfgLog *cfg);

  /// @brief Print an emergency message
  void emergency(const char *fmt, ...);
  /// @brief Print an alert message
  void alert(const char *fmt, ...);
  /// @brief Print a critical message
  void critical(const char *fmt, ...);
  /// @brief Print an error message
  void error(const char *fmt, ...);
  /// @brief Print a warning message
  void warning(const char *fmt, ...);
  /// @brief Print a notice
  void notice(const char *fmt, ...);
  /// @brief Print an info
  void info(const char *fmt, ...);
  /// @brief Print a debug message
  void debug(const char *fmt, ...);
  /// @brief Print an always message
  void always(const char *fmt, ...);

  /// @brief Return the currently set log level
  /// @return the loglevel
  CfgLog::level_e getLevel(void);

  /// @brief Set the log level
  /// @param [in] level a loglevel
  void setLevel(CfgLog::level_e level);

  /// @brief Return the currently used profile
  /// @return the logging profile
  CfgLog::profile_e getProfile(void);

  /// @brief Set the logging profile
  /// @param [in] style a logging profile
  void setProfile(CfgLog::profile_e style);

  /// @brief Return a pointer to the currently set pattern
  /// @return pointer to logging pattern
  char *getPattern(void);

  /// @brief Set a custom message pattern
  /// @param [in] pattern a string containing a valid pattern
  /// @return EErr on failure, ENoErr on success
  /// @note In case initialization has failed, the logger is still usable and returns to the default state.
  int setPattern(const char *pattern);

private:

  #define LOG(...) \
    (void)vfprintf(m_fd, __VA_ARGS__); \
    (void)fflush(m_fd); \

  enum {
    EPatInvalid = 0,
    EPatSeparator,
    EPatPID,
    EPatTime,
    EPatLevel,
    EPatMsg,
    EPatPrefix,
    EPatEnd,
    EPatUsr
  };            ///< message pattern identifier

  CfgLog  *m_cfg;                         ///< logger config
  FILE    *m_fd;                          ///< file descriptor
  bool     m_removeCfg;                   ///< flag to remove cfgLog in case it was created at ctor
  int      m_pattern[CfgLog::CMaxPatternItems];   ///< currently set pattern array

  /// Initialize logger
  void init(void);

  /// Initialize configuration from pattern
  /// @param[in] pattern string
  /// @return ENoErr on success, EErr on failure
  int initPattern(const char *pattern);

  /// Initialize a plogging profile
  /// sets all style setting to defaults depending on profile set
  /// @param [in] profile a default profile
  int initProfile(CfgLog::profile_e profile);

  /// Initialize one of the standard profiles (i.e. not user)
  /// @param [in] profile a profile
  int initStandardProfile(CfgLog::profile_e profile);

  /// Add individual parts of the message
  int  addUsr(char *msg, int no);
  void addMsg(char *msg, const char *fmt);
  void addTime(char *msg);
  void addPID(char *msg);
  void addLevel(char *msg, CfgLog::level_e lev);
  void addPostfix(char *msg);
  void addPrefix(char *msg);
  void addSeparator(char *msg);

  /// @brief Construct a log message
  /// @param[in,out] msg contains the contructed log msg after call
  /// @param[in] fmt the msg payload
  /// @param[in] level msg level
  void constructMsg(char *msg, const char *fmt, CfgLog::level_e lev);

  /// @brief Convert a string \a buf to uppercase letters
  /// @param[in,out] buf the character array to be converted
  /// @param [in] len length of \a buf
  /// @return a pointer to the converted char array \a buf
  char *to_upper(char* bug, uint8_t len);

  /// @brief Convert a string \a buf to lowercase letters
  /// @param[in,out] buf the character array to be converted
  /// @param [in] len length of \a buf
  /// @return a pointer to the converted char array \a buf
  char *to_lower(char* buf, uint8_t len);

  /// @brief Find and return the next numeric value found in \a buf
  /// @param [in] buf buffer to find numerica value in
  /// @param [in,out] rembuf pointer to next character in \a buf after found numeric
  /// @return numeric value on success, -1 on failure
  int findNextNumeric(const char *buf, char **rembuf);

};

#endif //_CPP_LOGGER_H_
