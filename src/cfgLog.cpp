//        __
//       / /   ___   __ _  __ _  ___ _ __
//      / /   / _ \ / _` |/ _` |/ _ \ '__|
//     / /___| (_) | (_| | (_| |  __/ |
//     \____/ \___/ \__, |\__, |\___|_|
//                  |___/ |___/        v1.0
//      <ramharter>

/// @file cfgLog.cpp
/// @brief Implementation of the CfgLog class

#include "log.h"

const char cfgLog::CLogMsgLevel[][cfgLog::CMaxLogLevelStrLen] = {
  "Emerg",
  "Alert",
  "Crit",
  "Error",
  "Warning",
  "Notice",
  "Info",
  "Debug",
  "Always"
};

CfgLog::CfgLog() {

  logLevel      = CLogLevelDefault;
  profile       = CLogProfileDefault;
  logToFile     = false;
  useColor      = false;
  useUsrPattern = false;

  color         = EColorWhite;
  logLevelCase  = ELevelCaseDefault;
  usrPattern    = NULL;

  // init strings
  memset(logfile,     '\0', sizeof(logfile));
  memset(prefix,      '\0', sizeof(prefix));
  memset(postfix,     '\0', sizeof(postfix));
  memset(separator,   '\0', sizeof(separator));
  memset(pattern,     '\0', sizeof(pattern));

  // set string defaults
  strcpy(separator, " | ");
  strcpy(postfix, "\n");
}

CfgLog::~CfgLog() {
  if (usrPattern) {
    UsrPattern *tmp = usrPattern;
    UsrPattern *ntmp = tmp;
    do {
      ntmp = tmp->next;
      delete tmp;
      tmp = ntmp;
    } while (tmp != NULL);
  }
}

void CfgLog::addUsrPattern(int nr, const char *pat) {
  if (!usrPattern) usrPattern = new UsrPattern(nr, pat);
  else {
    UsrPattern *tmp = usrPattern;
    while (tmp->next != NULL) {tmp = tmp->next;}
    tmp->next = new UsrPattern(nr, pat);
  }
}

char *CfgLog::getUsrPattern(int nr) {
  if (usrPattern) {
    UsrPattern *tmp = usrPattern;

    do {
      if (tmp->nr == nr) {
        return tmp->pat;
      } else {
        tmp = tmp->next;
      }
    } while (tmp != NULL);
  }
  return NULL;
}
