#include "cpp_log.h"
extern "C" {
  #include "utils.h"
}

int logger_test_profiles(Logger *logger = NULL);
int logger_test_levels(Logger *logger = NULL);
int test_standard_logger(void);
int test_config_logger(CfgLog *cfg);

int main(int argc, const char *argv[]) {

  CfgLog *cfg = new CfgLog();
  strcpy(cfg->separator, " | ");
  strcpy(cfg->prefix, "> ");
  strcpy(cfg->postfix, " -\n");
  cfg->useColor = true;

  cfg->useUsrPattern = true;
  cfg->addUsrPattern(0, "??");
  cfg->addUsrPattern(1, "blülülü");
  cfg->addUsrPattern(2, "§*§");
  cfg->addUsrPattern(3, "The answer to life, the universe, and everything");

  strncpy(cfg->pattern, "&pre&lev&sep&us0&tim&us2&sep&msg&end", sizeof(cfg->pattern));
  cfg->profile = CfgLog::ELogProfileUser;

  Logger *logger = new Logger(cfg);
  logger->always("blubbedi");

  logger->setPattern("&pre&us0&lev&us1&sep&msg&end");
  logger->always("blü");
  logger->emergency("help!");

  delete logger;

  Logger *newLog = new Logger("test.log", CfgLog::ELogDebug, CfgLog::ELogProfileDefault);
  newLog->debug("blub");
  delete newLog;

  // test_standard_logger();
  // test_config_logger(cfg);
  // logger_test_profiles();
  // logger_test_levels();

  delete cfg;

  return 0;
}

/// Output all default profiles
int logger_test_profiles(Logger *logger) {
  Logger *log = NULL;

  if (logger == NULL) {
    log = new Logger();
  } else {
    log = logger;
  }

  // log->setLevel(CfgLog::ELogDebug);
  log->always("Testing profiles");

  for (int i = 0; i <=  CfgLog::ELogProfileUser; ) {
    log->always("--- Test %d start ---", i+1);

    // inc i here instead of at loop to start with test 1 instead of 0
    log->setProfile((CfgLog::profile_e)i++);

    log->always("an always msg");
    log->error("an error");
    log->warning("a warning");
    log->notice("a notice");
    log->debug("a debug msg");

    log->setProfile(CfgLog::ELogProfileNone);
    log->always("---  Test %d end  ---", i);
  }

  return 0;
}

int logger_test_levels(Logger *logger) {

  Logger *log = NULL;

  if (logger == NULL) {
    log = new Logger();
  } else {
    log = logger;
  }

  log->setProfile(CfgLog::ELogProfileDefault);
  log->always("Testing levels");

  for (int i = 0; i <= CfgLog::ELogDebug;) {

    log->setLevel((CfgLog::level_e)i++);

    log->always("\n--- Test log level %s start ---", CfgLog::CLogMsgLevel[i]);
    log->always("an always msg");
    log->emergency("an emergency");
    log->alert("an alert");
    log->critical("a critical msg");
    log->error("an error");
    log->warning("a warning");
    log->notice("a notice");
    log->info("an info");
    log->debug("a debug msg");
    log->always("--- Test log level %s end ---\n", CfgLog::CLogMsgLevel[i]);
  }

  return 0;
}

int test_standard_logger() {

  logger_test_profiles();
  logger_test_levels();

  return 0;
}

int test_config_logger(CfgLog *cfg) {

  if (cfg == NULL) {
    return 1;
  }

  Logger *log = NULL;
  log = new Logger(cfg);
  if (log == NULL) {
    return 1;
  }

  logger_test_levels(log);
  logger_test_profiles(log);
  return 0;
}
