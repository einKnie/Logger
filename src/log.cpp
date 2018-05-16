#include "cpp_log.h"
#include <errno.h>
#include <time.h>
#include <unistd.h>

extern "C" {
  #include "utils.h"
}

// default logstyle patterns
char patterns[][Logger::CMaxPatternLen] =
                    {"&msg&end",                              // ELogStyleNone
                     "&pre&lev&sep&msg&end",                  // ELogStyleMinimal
                     "&pre&tim&sep&lev&sep&msg&end",          // ELogStyleDefault
                     "&pre&pid&sep&tim&sep&lev&sep&msg&end",  // ELogStyleVerbose
                    };

const char *Logger::CLogMsgError     = "Error";
const char *Logger::CLogMsgWarning   = "Warning";
const char *Logger::CLogMsgNotice    = "Notice";
const char *Logger::CLogMsgDebug     = "Debug";
const char *Logger::CLogMsgAlways    = "Always";

Logger::Logger() : Logger(NULL, CLogLevelDefault, CLogStyleDefault) {}
Logger::Logger(const char *logfile, Logger::level_e level, Logger::style_e style) {

  m_cfg = new CfgLog();
  m_removeCfg = true;

  m_cfg->logLevel = level;
  m_cfg->logStyle = style;

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

void Logger::init() {

  // initialize oattern directly if available
  if (m_cfg->pattern[0] == '\0') {
    initStyle();
  } else {
    m_cfg->logStyle = ELogStyleUser;
    initPattern(m_cfg->pattern);
  }

  // set log destination
  m_fd = NULL;
  if (m_cfg->logToFile) {
    m_fd = fopen(m_cfg->logfile, "w");
    if (m_fd == NULL) {
      m_cfg->logToFile = false;
      m_fd = stdout;
      error("Failed to reroute logging to file @ %s: %d", m_cfg->logfile, errno);
      notice("Routing all logging to stdout");
    }
  } else {
    m_fd = stdout;
  }

}

void Logger::error(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogDisable: return;
    case ELogError:   break;
    case ELogWarn   : break;
    case ELogVerbose: break;
    case ELogDebug:   break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgError);

  va_start(m_args, fmt);
  log(msg);
  va_end(m_args);
}

void Logger::warning(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogDisable: return;
    case ELogError:   return;
    case ELogWarn   : break;
    case ELogVerbose: break;
    case ELogDebug:   break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgWarning);

  va_start(m_args, fmt);
  log(msg);
  va_end(m_args);
}

void Logger::notice(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogDisable: return;
    case ELogError:   return;
    case ELogWarn   : return;
    case ELogVerbose: break;
    case ELogDebug:   break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgNotice);

  va_start(m_args, fmt);
  log(&msg[0]);
  va_end(m_args);
}

void Logger::debug(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};

  if (m_fd == NULL) return;

  switch(m_cfg->logLevel) {
    case ELogDisable: return;
    case ELogError:   return;
    case ELogWarn   : return;
    case ELogVerbose: return;
    case ELogDebug:   break;
    default: return;
  }

  constructMsg(msg, fmt, Logger::CLogMsgDebug);

  va_start(m_args, fmt);
  log(msg);
  va_end(m_args);
}

void Logger::always(const char *fmt, ...) {

  char msg[Logger::CMaxMsgLen]       = {0};

  if (m_fd == NULL) return;

  constructMsg(msg, fmt, Logger::CLogMsgAlways);

  va_start(m_args, fmt);
  log(msg);
  va_end(m_args);
}

void Logger::log(const char *msg) {

  (void)vfprintf(m_fd, msg, m_args);
  (void)fflush(m_fd);

}

Logger::level_e Logger::getLevel() {
  return m_cfg->logLevel;
}

void Logger::setLevel(Logger::level_e level) {
  m_cfg->logLevel = level;
}

Logger::style_e Logger::getStyle() {
  return m_cfg->logStyle;
}

void Logger::setStyle(Logger::style_e style) {
  m_cfg->logStyle = style;
  initStyle();
}

char *Logger::getPattern(void) {
  return m_cfg->pattern;
}

int Logger::setPattern(const char *pattern) {
  if (initPattern(pattern) == EErr) return EErr;

  strncpy(m_cfg->pattern, pattern, sizeof(m_cfg->pattern));
  return ENoErr;
}

/// Set style presets
void Logger::initStyle() {
  if (m_cfg->logStyle != ELogStyleUser) {
    PRINT_DEBUG("setting pattern: %s\n", patterns[(int)m_cfg->logStyle]);
    strncpy(m_cfg->pattern, patterns[(int)m_cfg->logStyle], sizeof(m_cfg->pattern));
  }
  initPattern(m_cfg->pattern);
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
        PRINT_DEBUG("Got time pattern\n");
        m_pattern[j] = EPatTime;
      } else if (strncmp(tmp, "lev", 3) == 0) {
        PRINT_DEBUG("Got level pattern\n");
        m_pattern[j] = EPatLevel;
      } else if (strncmp(tmp, "sep", 3) == 0) {
        PRINT_DEBUG("Got separator pattern\n");
        m_pattern[j] = EPatSeparator;
      } else if (strncmp(tmp, "pid", 3) == 0) {
        PRINT_DEBUG("Got pid pattern\n");
        m_pattern[j] = EPatPID;
      } else if (strncmp(tmp, "msg", 3) == 0) {
        PRINT_DEBUG("Got msg pattern\n");
        m_pattern[j] = EPatMsg;
      } else if (strncmp(tmp, "pre", 3) == 0) {
        PRINT_DEBUG("Got msg pattern\n");
        m_pattern[j] = EPatPrefix;
      } else if (strncmp(tmp, "end", 3) == 0) {
        PRINT_DEBUG("Got msg pattern\n");
        m_pattern[j] = EPatEnd;
      } else if (strncmp(tmp, "usr", 3) == 0) {
        PRINT_DEBUG("Got msg pattern\n");
        m_pattern[j] = EPatUser;
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
      case EPatUser:       addUsr(buf); break;
      case EPatTime:       addTime(buf); break;
      case EPatInvalid:
      default: if (i == 0) return; else break;
    }
  }

  if (strlen(buf) >= strlen(fmt)) {
    PRINT_DEBUG("Constructed message: %s\n", buf);
    strncpy(msg, buf, CMaxMsgLen);
  }
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
    case CfgLog::ELevelDefault: break;
    case CfgLog::ELevelLower: to_lower(lbuf, strlen(lbuf)); break;
    case CfgLog::ELevelUpper: to_upper(lbuf, strlen(lbuf)); break;
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

void Logger::addUsr(char *msg) {
  strcat(msg, m_cfg->userDefinedPatternItem);
}
