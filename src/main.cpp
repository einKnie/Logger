#include "cpp_log.h"

void test_logger(Logger::CfgLog *cfg = NULL);

int main(int argc, const char *argv[]) {

  // Logger::CfgLog *cfg = new Logger::CfgLog();
  // cfg->printNewline = true;
  // cfg->printLevel = false;
  // cfg->printTime = false;
  // cfg->printPid = false;
  // strcpy(cfg->separator, " | ");
  // cfg->printPrefix = true;
  // strcpy(cfg->prefix, ">");
  // cfg->logToFile = true;
  // strcpy(cfg->logfile, "./file.log");

  Logger *logger = new Logger();
  logger->always("blubbedi\n\n");
  delete logger;

  test_logger();

  // delete cfg;


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
