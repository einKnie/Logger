#include "cpp_log.h"

void test_logger(Logger::CfgLog *cfg = NULL);

int main(int argc, const char *argv[]) {

  Logger::CfgLog *cfg = new Logger::CfgLog();
  strcpy(cfg->separator, " | ");
  strcpy(cfg->prefix, "> ");
  strcpy(cfg->postfix, " -\n");
  strcpy(cfg->userDefinedPatternItem, " ^^ ");
  // cfg->logToFile = true;
  // strcpy(cfg->logfile, "./file.log");
  strncpy(cfg->pattern, "&pre&lev&sep&usr&tim&usr&sep&msg&end", sizeof(cfg->pattern));
  cfg->profile = Logger::ELogProfileUser;

  Logger *logger = new Logger(cfg);
  logger->always("blubbedi");

  logger->setPattern("&pr&bube&sep&pid&sep&lev&sep&msg&end");
  logger->always("blü");

  delete logger;

  test_logger(cfg);

  delete cfg;


  return 0;
}

void test_logger(Logger::CfgLog *cfg) {
  Logger *log = NULL;
  // char pattern[80] = {0};

  if (cfg != NULL) {
    log = new Logger(cfg);
  } else {
    log = new Logger();
  }

  log->setLevel(Logger::ELogDebug);

  log->always("Testing styles");
  for (int i = 0; i <= Logger::ELogProfileVerbose;) {

    log->setProfile((Logger::profile_e)i++);

    log->always("test %d start...", i);
    log->always("an always msg");
    log->error("an error");
    log->warning("a warning");
    log->notice("a notice");
    log->debug("a debug msg");
    log->always("test %d end....", i);

  }

  log->setProfile(Logger::ELogProfileDefault);
  log->always("Testing levels");
  for (int i = 0; i <= Logger::ELogDebug;) {

    log->setLevel((Logger::level_e)i++);

    log->always("test %d start...", i);
    log->always("an always msg");
    log->error("an error");
    log->warning("a warning");
    log->notice("a notice");
    log->debug("a debug msg");
    log->always("test %d end....", i);

  }

  log->always("Finished logger testing");
  delete log;
}

void test_logger2(Logger::CfgLog *cfg) {
  Logger *log = NULL;
  Logger::CfgLog *myCfg = NULL;
  // char pattern[80] = {0};

  if (cfg != NULL) {
    myCfg = cfg;
    log = new Logger(myCfg);
  } else {
    myCfg = new Logger::CfgLog();
    log = new Logger(myCfg);
  }

  log->setLevel(Logger::ELogDebug);

  log->always("Testing profiles");
  for (int i = 0; i <= Logger::ELogProfileVerbose;) {

    myCfg->profile = (Logger::profile_e)i++;
    log->init(myCfg);

    log->always("test %d start...", i);
    log->always("an always msg");
    log->error("an error");
    log->warning("a warning");
    log->notice("a notice");
    log->debug("a debug msg");
    log->always("test %d end....", i);

  }

  myCfg->profile = Logger::ELogProfileDefault;
  log->init(myCfg);

  log->always("Testing levels");
  for (int i = 0; i <= Logger::ELogDebug;) {

    log->setLevel((Logger::level_e)i++);

    log->always("test %d start...", i);
    log->always("an always msg");
    log->error("an error");
    log->warning("a warning");
    log->notice("a notice");
    log->debug("a debug msg");
    log->always("test %d end....", i);

  }

  log->always("Finished logger testing");
  delete log;
}
