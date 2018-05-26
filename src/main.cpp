#include "cpp_log.h"
extern "C" {
  #include "utils.h"
}

void test_logger(CfgLog *cfg = NULL);

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
  cfg->addUsrPattern(42, "The answer to life, the universe, and everything");

  strncpy(cfg->pattern, "&pre&lev&sep&us0&tim&us2&sep&msg&end", sizeof(cfg->pattern));
  cfg->profile = CfgLog::ELogProfileUser;

  Logger *logger = new Logger(cfg);
  logger->always("blubbedi");

  logger->setPattern("&pre&us0&lev&us1&sep&msg&end");
  logger->always("blü");
  logger->emergency("help!");

  delete logger;

  test_logger(cfg);

  delete cfg;

  // char tmp[] = "1cjfh2bh3nm4u5n6n7o8hh9do0zu";
  // char *buf, *buf2;
  // buf = &tmp[0];
  // buf2 = buf;
  // for (int i = 0; i < strlen(tmp); i++) {
  //   int j = findNextNumeric(buf, &buf2);
  //   if (j == -1) {
  //     printf("done!\n"); break;
  //   }
  //   printf("found %d - remaining string: %s\n", j, buf2);
  //   buf = buf2;
  // }


  return 0;
}

void test_logger(CfgLog *cfg) {
  Logger *log = NULL;
  // char pattern[80] = {0};

  if (cfg != NULL) {
    log = new Logger(cfg);
  } else {
    log = new Logger();
  }

  log->setLevel(CfgLog::ELogDebug);

  log->always("Testing styles");
  for (int i = 0; i <= CfgLog::ELogProfileVerbose;) {

    log->setProfile((CfgLog::profile_e)i++);

    log->always("test %d start...", i);
    log->always("an always msg");
    log->error("an error");
    log->warning("a warning");
    log->notice("a notice");
    log->debug("a debug msg");
    log->always("test %d end....", i);

  }

  log->setProfile(CfgLog::ELogProfileDefault);
  log->always("Testing levels");
  for (int i = 0; i <= CfgLog::ELogDebug;) {

    log->setLevel((CfgLog::level_e)i++);

    log->always("test %d start...", i);
    log->always("an always msg");
    log->emergency("an emergency");
    log->alert("an alert");
    log->critical("a critical msg");
    log->error("an error");
    log->warning("a warning");
    log->notice("a notice");
    log->info("an info");
    log->debug("a debug msg");
    log->always("test %d end....", i);

  }

  log->always("Finished logger testing");
  delete log;
}
