#include "cpp_log.h"
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

extern "C" {
  #include "utils.h"
}

/// TODO: use color mode!!!
/// if color mode enabled, print the more serious levels in red (e.g.)
// OR let  user specifiy which colors to use (?)

/// default logprofile patterns
char default_patterns[][Logger::CMaxPatternLen] =
                    {"&msg&end",                              // ELogProfileNone
                     "&pre&lev&sep&msg&end",                  // ELogProfileMinimal
                     "&pre&tim&sep&lev&sep&msg&end",          // ELogProfileDefault
                     "&pre&pid&sep&tim&sep&lev&sep&msg&end",  // ELogProfileVerbose
                    };
/// default level caase config for default profiles
int default_level_cases[] = { Logger::CfgLog::ELevelCaseLower,    // ELogProfileNone
                              Logger::CfgLog::ELevelCaseLower,    // ELogProfileMinimal
                              Logger::CfgLog::ELevelCaseDefault,  // ELogProfileDefault
                              Logger::CfgLog::ELevelCaseUpper     // ELogProfileVerbose
                            };

const char *Logger::CLogMsgEmergency = "Emerg";
const char *Logger::CLogMsgAlert     = "Alert";
const char *Logger::CLogMsgCritical  = "Crit";
const char *Logger::CLogMsgError     = "Error";
const char *Logger::CLogMsgWarning   = "Warning";
const char *Logger::CLogMsgNotice    = "Notice";
const char *Logger::CLogMsgInfo      = "Info";
const char *Logger::CLogMsgDebug     = "Debug";
const char *Logger::CLogMsgAlways    = "Always";

Logger::Logger() : Logger(NULL, CLogLevelDefault, CLogProfileDefault) {}
Logger::Logger(const char *logfile, Logger::level_e level, Logger::profile_e profile) {

  m_cfg = new CfgLog();
  m_removeCfg = true;

  m_cfg->logLevel = level;
  m_cfg->profile = profile;

  if (logfile != NULL) {
    strncpy(m_cfg->logfile, logfile, sizeof(m_cfg->logfile));
    m_cfg->logToFile = true;
  }

  init();
}

Logger::Logger(Logger::CfgLog *cfg) {
  if (cfg == NULL) {
    m_cfg = new CfgLog();
    m_removeCfg = true;
  } else {
    m_cfg = cfg;
    m_removeCfg = false;
  }
  init();
}

Logger::~Logger() {
  if ((m_removeCfg) && (m_cfg != NULL)) {
    delete m_cfg;
  }

  if (m_fd != NULL && m_fd != stdout) {
    fflush(m_fd);
    if (fclose(m_fd) != 0) {
      fprintf(stderr, "Failed to close logfile\n");
    } else {
      m_fd = NULL;
    }
  }
}

int Logger::init(CfgLog *cfg) {
  if (cfg != NULL) {
    if (m_removeCfg) delete m_cfg;
    m_cfg = cfg;
  } else return EErr;

  init();
  return ENoErr;
}

void Logger::init() {

  // set log destination
  m_fd = NULL;
  if (m_cfg->logToFile) {
    m_fd = fopen(m_cfg->logfile, "w");
    if (m_fd == NULL) {
      m_cfg->logToFile = false;
      m_fd = stdout;
      fprintf(stderr, "Failed to reroute logging to file @ %s: %d", m_cfg->logfile, errno);
      fprintf(stderr, "Routing all logging to stdout");
    }
  } else {
    m_fd = stdout;
  }

  // set pattern
  if (m_cfg->profile == ELogProfileUser) {
      if (initPattern(m_cfg->pattern) == ENoErr) {
        return;
      } else {
        fprintf(stderr, "Failed to apply pattern.\n");
        fprintf(stderr, "Reverting to default....\n");
        // revert to default
        m_cfg->profile = CLogProfileDefault;
      }
  }

  // initialize one of the standard profiles
  initProfile(m_cfg->profile);
}

void Logger::initProfile(profile_e profile) {

  if (!initPattern(default_patterns[(int)m_cfg->profile])) {
    fprintf(stderr, "Failed to initialize standard profile.\n");
  } else {
    PRINT_DEBUG("setting pattern: %s\n", default_patterns[(int)m_cfg->profile]);
    strncpy(m_cfg->pattern, default_patterns[(int)m_cfg->profile], sizeof(m_cfg->pattern));
    m_cfg->logLevelCase = default_level_cases[(int)m_cfg->profile];
  }
}

void Logger::emergency(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};
  va_list args;

  if (m_fd == NULL) return;
  // emergencies are always logged, regardless of log level

  constructMsg(msg, fmt, Logger::CLogMsgEmergency);

  if (m_cfg->useColor) {
    char buf[Logger::CMaxMsgLen];
    strcpy(buf, msg);
    sprintf(msg, "\033[%dm%s\033[0m", m_cfg->color, buf);
  }

  va_start(args, fmt);
  LOG(msg, args);
  va_end(args);
}

void Logger::alert(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};
  va_list args;

  if (m_fd == NULL) return;

  // TODO: eigentlich schwachsinning, ich kann genausogut abfragen ob loglevel >= Alert
  switch(m_cfg->logLevel) {
    case ELogEmergency: return;
    case ELogAlert:     break;
    case ELogCritical:  break;
    case ELogError:     break;
    case ELogWarn:      break;
    case ELogNotice:    break;
    case ELogInfo:      break;
    case ELogDebug:     break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgAlert);

  if (m_cfg->useColor) {
    char buf[Logger::CMaxMsgLen];
    strcpy(buf, msg);
    sprintf(msg, "\033[%dm%s\033[0m", m_cfg->color, buf);
  }

  va_start(args, fmt);
  LOG(msg, args);
  va_end(args);
}

void Logger::critical(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};
  va_list args;

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogEmergency: return;
    case ELogAlert:     return;
    case ELogCritical:  break;
    case ELogError:     break;
    case ELogWarn:      break;
    case ELogNotice:    break;
    case ELogInfo:      break;
    case ELogDebug:     break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgCritical);

  if (m_cfg->useColor) {
    char buf[Logger::CMaxMsgLen];
    strcpy(buf, msg);
    sprintf(msg, "\033[%dm%s\033[0m", m_cfg->color, buf);
  }

  va_start(args, fmt);
  LOG(msg, args);
  va_end(args);
}

void Logger::error(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};
  va_list args;

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogEmergency: return;
    case ELogAlert:     return;
    case ELogCritical:  return;
    case ELogError:     break;
    case ELogWarn:      break;
    case ELogNotice:    break;
    case ELogInfo:      break;
    case ELogDebug:     break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgError);
  va_start(args, fmt);
  LOG(msg, args);
  va_end(args);
}

void Logger::warning(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};
  va_list args;

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogEmergency: return;
    case ELogAlert:     return;
    case ELogCritical:  return;
    case ELogError:     return;
    case ELogWarn:      break;
    case ELogNotice:    break;
    case ELogInfo:      break;
    case ELogDebug:     break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgWarning);
  va_start(args, fmt);
  LOG(msg, args);
  va_end(args);
}

void Logger::notice(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};
  va_list args;

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogEmergency: return;
    case ELogAlert:     return;
    case ELogCritical:  return;
    case ELogError:     return;
    case ELogWarn:      return;
    case ELogNotice:    break;
    case ELogInfo:      break;
    case ELogDebug:     break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgNotice);
  va_start(args, fmt);
  LOG(msg, args);
  va_end(args);
}

void Logger::info(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};
  va_list args;

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogEmergency: return;
    case ELogAlert:     return;
    case ELogCritical:  return;
    case ELogError:     return;
    case ELogWarn:      return;
    case ELogNotice:    return;
    case ELogInfo:      break;
    case ELogDebug:     break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgInfo);
  va_start(args, fmt);
  LOG(msg, args);
  va_end(args);
}

void Logger::debug(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};
  va_list args;

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogEmergency: return;
    case ELogAlert:     return;
    case ELogCritical:  return;
    case ELogError:     return;
    case ELogWarn:      return;
    case ELogNotice:    return;
    case ELogInfo:      return;
    case ELogDebug:     break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgDebug);
  va_start(args, fmt);
  LOG(msg, args);
  va_end(args);
}

void Logger::always(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};
  va_list args;

  if (m_fd == NULL) return;

  constructMsg(msg, fmt, Logger::CLogMsgAlways);
  va_start(args, fmt);
  LOG(msg, args);
  va_end(args);
}

Logger::level_e Logger::getLevel() {
  return m_cfg->logLevel;
}

void Logger::setLevel(Logger::level_e level) {
  m_cfg->logLevel = level;
}

Logger::profile_e Logger::getProfile() {
  return m_cfg->profile;
}

void Logger::setProfile(Logger::profile_e profile) {
  m_cfg->profile = profile;
  initProfile(m_cfg->profile);
}

char *Logger::getPattern(void) {
  return m_cfg->pattern;
}

int Logger::setPattern(const char *pattern) {
  if (initPattern(pattern) == ENoErr) {
  	strncpy(m_cfg->pattern, pattern, sizeof(m_cfg->pattern));
    return ENoErr;
  } else {
    initProfile(CLogProfileDefault);
    return EErr;
  }
}

int Logger::initPattern(const char *pattern) {

  char tmp[5] = {0};
  int  patLen = strlen(pattern);
  char pat[Logger::CMaxPatternLen] = {0};

  // reset pattern array
  for (int i = 0; i < CMaxPatternItems; i++) {
    m_pattern[i] = EPatInvalid;
  }

  strncpy(pat, pattern, Logger::CMaxPatternLen);
  PRINT_DEBUG("Using pattern: %s\n", pat);

  if (pattern == NULL) {
    PRINT_DEBUG("Invalid pattern string\n");
    return EErr;
  } else if (strstr(pat, "&") == NULL) {
    PRINT_DEBUG("Invalid pattern string\n");
    return EErr;
  }

  for (int i = 0, j = 0; i < patLen; i++, j++) {

    if (pat[i] == '&' && (i < (patLen - 2))) {

      sprintf(tmp, "%3.3s", &pat[++i]);
      PRINT_DEBUG("got pattern identifier: %s\n", tmp);

      if (strncmp(tmp, "tim", 3) == 0) {
        m_pattern[j] = EPatTime;
      } else if (strncmp(tmp, "lev", 3) == 0) {
        m_pattern[j] = EPatLevel;
      } else if (strncmp(tmp, "sep", 3) == 0) {
        m_pattern[j] = EPatSeparator;
      } else if (strncmp(tmp, "pid", 3) == 0) {
        m_pattern[j] = EPatPID;
      } else if (strncmp(tmp, "msg", 3) == 0) {
        m_pattern[j] = EPatMsg;
      } else if (strncmp(tmp, "pre", 3) == 0) {
        m_pattern[j] = EPatPrefix;
      } else if (strncmp(tmp, "end", 3) == 0) {
        m_pattern[j] = EPatEnd;
      // } else if (strncmp(tmp, "us1", 3) == 0) {
      //   m_pattern[j] = EPatUser1;
      // } else if (strncmp(tmp, "us2", 3) == 0) {
      //   m_pattern[j] = EPatUser2;
      } else if (strncmp(tmp, "us", 2) == 0) {
        // get number from string, add user pattern of number
        int no = findNextNumeric(tmp, NULL);
        PRINT_DEBUG("Got user pattern %d\n", no);
        // TODO:
        // maybe set EUsrBase + userPatternNo && calc back to no @ construct ?
        // m_pattern must be non-typed for this to work
        m_pattern[j] = EPatUsr + no;
      } else {
        PRINT_DEBUG("got nothing...\n");
        return EErr;
      }

      if ((i + 2) > patLen) {
        break;
      } else {
        i += 2;
      }
    } else {
      PRINT_DEBUG("Invalid pattern string\n");
      return EErr;
    }
  }
  return ENoErr;
}

void Logger::constructMsg(char *msg, const char *fmt, const char *level) {
  char buf[CMaxMsgLen] = {0};

  for (int i = 0; i < CMaxPatternItems; i++) {
    PRINT_DEBUG("item %d: %d\n", i, m_pattern[i]);
    switch (m_pattern[i]) {
      case EPatSeparator:  addSeparator(buf); break;
      case EPatPrefix:     addPrefix(buf); break;
      case EPatEnd:        addPostfix(buf); break;
      case EPatPID:        addPID(buf); break;
      case EPatLevel:      addLevel(buf, level); break;
      case EPatMsg:        addMsg(buf, fmt); break;
      case EPatTime:       addTime(buf); break;
      case EPatInvalid:    if (i == 0) return; else break;
      default: int no =    m_pattern[i] - EPatUsr; addUsr(buf, no); // user defined pattern
    }
  }

  if (strlen(buf) >= strlen(fmt)) {
    PRINT_DEBUG("Constructed message: %s\n", buf);
    strncpy(msg, buf, CMaxMsgLen);
  }
}

int Logger::addUsr(char *msg, int no) {
  CfgLog::UsrPattern *tmp = m_cfg->usrPattern;
  if (m_cfg->usrPattern != NULL) {
    do {
      if (no == tmp->nr) {
        strcat(msg, tmp->pat);
        return ENoErr;
      } else {tmp = tmp->next;}
    } while (tmp != NULL);
  }
  return EErr;
}

void Logger::addSeparator(char *msg) {
  strcat(msg, m_cfg->separator);
}

void Logger::addPrefix(char *msg) {
  strcat(msg, m_cfg->prefix);
}

void Logger::addPostfix(char *msg) {
  strcat(msg, m_cfg->postfix);
}

void Logger::addLevel(char *msg, const char *level) {
  char levelbuf[20] = {0};
  char lbuf[10] = {0};

  strcpy(lbuf, level);
  switch(m_cfg->logLevelCase) {
    case CfgLog::ELevelCaseLower: to_lower(lbuf, strlen(lbuf)); break;
    case CfgLog::ELevelCaseUpper: to_upper(lbuf, strlen(lbuf)); break;
    case CfgLog::ELevelCaseDefault: // move to default
    default: break;
  }

  sprintf(levelbuf, "%-7s", lbuf);
  strcat(msg, levelbuf);
}

void Logger::addPID(char *msg) {
  char buf[10] = {0};
  sprintf(buf, "%d", getpid());
  strcat(msg, buf);
}

void Logger::addTime(char *msg) {
  char timebuf[20] = {0};
  time_t t;
  struct tm *tm;
  time(&t);
  tm = localtime(&t);

  sprintf(timebuf, "%02d:%02d:%02d",
              tm->tm_hour, tm->tm_min, tm->tm_sec);
  strcat(msg, timebuf);
}

void Logger::addMsg(char *msg, const char *fmt) {
  strcat(msg, fmt);
}
