#include "cpp_log.h"

void test_logger(Logger::CfgLog *cfg = NULL);

int main(int argc, const char *argv[]) {

  Logger::CfgLog *cfg = new Logger::CfgLog();
  strcpy(cfg->separator, " | ");
  strcpy(cfg->prefix, "> ");
  // cfg->logToFile = true;
  // strcpy(cfg->logfile, "./file.log");
  strncpy(cfg->pattern, "&pre&lev&sep&tim&sep&msg&end", sizeof(cfg->pattern));

  Logger *logger = new Logger(cfg);
  logger->always("blubbedi");

  logger->setPattern("&pre&sep&pid&sep&lev&sep&msg&end");
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
  for (int i = 0; i <= Logger::ELogStyleVerbose;) {

    log->setStyle((Logger::style_e)i++);

    log->always("test %d start...", i);
    log->always("an always msg");
    log->error("an error");
    log->warning("a warning");
    log->notice("a notice");
    log->debug("a debug msg");
    log->always("test %d end....", i);

  }

  log->setStyle(Logger::ELogStyleDefault);
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
