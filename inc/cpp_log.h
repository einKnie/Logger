#ifndef _CPP_LOGGER_H_
#define _CPP_LOGGER_H_

#include <stdio.h>
#include <string.h>
#include <linux/limits.h>

#define DEBUG
#if defined DEBUG
  #define PRINT_DEBUG(...) printf(__VA_ARGS__)
#else
  #define PRINT_DEBUG(...)
#endif

class Logger {
public:

  /// Enums
  enum { EErr = 0, ENoErr };            ///< return values
  typedef enum { ELogEmergency = 0,     ///< always && emergency msgs
                 ELogAlert,             ///< always, emergency && alert msgs
                 ELogCritical,          ///< always, emerg, alert && critical msgs
                 ELogError,             ///< always, emerg, alert, crit && errors
                 ELogWarn,              ///< always, emerg, alert, crit, errors && warnings
                 ELogNotice,            ///< always, emerg, alert, crit, errors, warnings && notices
                 ELogInfo,              ///< always, emerg, alert, crit, errors, warnings, notices && infos
                 ELogDebug              ///< always, emerg, alert, crit, errors, warnings, notices, infos && debug msgs
               } level_e;               ///< loglevel

  typedef enum { ELogProfileNone  = 0,  ///< print just the plain message
                 ELogProfileMinimal,    ///< output minimalistic logging
                 ELogProfileDefault,    ///< time && level
                 ELogProfileVerbose,    ///< output verbose logging
                 ELogProfileUser        ///< user-defined style
               } profile_e;             ///< logstyle

  /// Constants
  static const int   CMaxPathLen        = PATH_MAX;   ///< max path length
  static const int   CMaxMsgLen         = 500;        ///< max logger line length
  static const int   CMaxPrefixLen      = 10;         ///< maximum length of the msg prefix
  static const int   CMaxSepLen         = 10;         ///< maximum length of the separator
  static const int   CMaxPatternItems   = 10;         ///< maximum number of pattern items which may be set
  static const int   CMaxPatternItemLen = 10;         ///< maximum length of a pattern item
  static const int   CMaxPatternLen     = CMaxPatternItems * 4 + 1; ///< 10*4 characters + null termination

  static const char *CLogMsgEmergency;                ///< Emergency string
  static const char *CLogMsgAlert;                    ///< Alert string
  static const char *CLogMsgCritical;                 ///< Critical string
  static const char *CLogMsgError;                    ///< Error string
  static const char *CLogMsgWarning;                  ///< Warning string
  static const char *CLogMsgNotice;                   ///< Notice string
  static const char *CLogMsgInfo;                     ///< Info string
  static const char *CLogMsgDebug;                    ///< Debug string
  static const char *CLogMsgAlways;                   ///< Always string

  static const level_e CLogLevelDefault = ELogDebug;            ///< default loglevel value
  static const profile_e CLogProfileDefault = ELogProfileNone;  ///< default profile

  /// Logger configuration
  typedef struct cfgLog {
    /// TODO: allow user to set an arbitrary number of user defined pattern items

    typedef struct usrPattern {
      char pat[Logger::CMaxPatternItemLen];
      int nr;
      struct usrPattern *next;

      usrPattern(int nr = 0, const char *pattern = "") {
        this->nr = nr;
        memset(pat, 0, sizeof(pat));
        strncpy(pat, pattern, sizeof(pat));

        next = NULL;
      }
    } UsrPattern;

    enum {
      ELevelCaseDefault = 0,  ///< default level string, e.g. Notice
      ELevelCaseLower,        ///< lower case level string, e.g. notice
      ELevelCaseUpper         ///< upper case level string, e.g. NOTICE
    };

    typedef enum {
      EColorBlack = 30,
      EColorRed,
      EColorGreen,
      EColorYellow,
      EColorBlue,
      EColorMagenta,
      EColorCyan,
      EColorWhite
    } color_e;

    Logger::level_e logLevel;              ///< loglevel
    Logger::profile_e profile;             ///< logstyle
    bool logToFile;                        ///< flag for file logging
    bool useColor;                         ///< enable coloful logging
    color_e color;
    int  logLevelCase;                     ///< print level in default, lower- or uppercase

    char logfile[Logger::CMaxPathLen];     ///< path to logfile
    char prefix[Logger::CMaxPrefixLen];    ///< set prefix
    char postfix[Logger::CMaxPrefixLen];   ///< postfix, must include newline if needed
    char separator[Logger::CMaxSepLen];    ///< set separator

    char pattern[Logger::CMaxPatternLen];  ///< log msg pattern

    bool useUsrPattern;                    ///< use user defined patterns
    UsrPattern *usrPattern;                ///< liked list of user patterns

    /// Constructor
    cfgLog() {

      logLevel      = Logger::CLogLevelDefault;
      profile       = Logger::CLogProfileDefault;
      logToFile     = false;
      useColor      = false;
      color         = EColorWhite;
      logLevelCase  = ELevelCaseDefault;

      // user defined patterns
      useUsrPattern = false;
      usrPattern    = NULL;

      // init strings
      memset(prefix,      '\0', sizeof(prefix));
      memset(postfix,     '\0', sizeof(postfix));
      memset(logfile,     '\0', sizeof(logfile));
      memset(separator,   '\0', sizeof(separator));
      memset(pattern,     '\0', sizeof(pattern));

      // set string defaults
      strcpy(separator, " | ");
      strcpy(postfix, "\n");
    }

    ~cfgLog() {
      if (usrPattern != NULL) {
        UsrPattern *tmp = usrPattern;
        UsrPattern *ntmp = tmp;
        do {
          ntmp = tmp->next;
          delete tmp;
          tmp = ntmp;
        } while (tmp != NULL);
      }
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
  Logger(const char *logfile, level_e level, profile_e style);

  /// Destructor
  ~Logger();

  /// Initialize logger with CfgLog struct
  int init(CfgLog *cfg);

  void emergency(const char *fmt, ...);
  void alert(const char *fmt, ...);
  void critical(const char *fmt, ...);
  void error(const char *fmt, ...);
  void warning(const char *fmt, ...);
  void notice(const char *fmt, ...);
  void info(const char *fmt, ...);
  void debug(const char *fmt, ...);
  void always(const char *fmt, ...);

  level_e getLevel(void);
  profile_e getProfile(void);
  void setLevel(level_e level);
  void setProfile(profile_e style);

  int setPattern(const char *pattern);
  char *getPattern(void);

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
    int    m_pattern[CMaxPatternItems];   ///< currently set pattern array

  /// Initialize logger
  void init(void);

  /// Initialize configuration from pattern
  /// @param[in] pattern string
  /// @return ENoErr on success, EErr on failure
  int initPattern(const char *pattern);

  /// Initialize a default logstyle
  /// sets all style setting to defaults depending on logstyle set
  /// @param [in] profile a default profile
  void initProfile(profile_e profile);

  /// Add individual parts of the message
  int  addUsr(char *msg, int no);
  void addMsg(char *msg, const char *fmt);
  void addTime(char *msg);
  void addPID(char *msg);
  void addLevel(char *msg, const char *level);
  void addPostfix(char *msg);
  void addPrefix(char *msg);
  void addSeparator(char *msg);

  /// Construct a log message
  /// @param[in,out] msg contains the contructed log msg after call
  /// @param[in] fmt the msg payload
  /// @param[in] level msg level
  void constructMsg(char *msg, const char *fmt, const char *level);

};

#endif //_CPP_LOGGER_H_
