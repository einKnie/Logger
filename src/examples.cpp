//        __
//       / /   ___   __ _  __ _  ___ _ __
//      / /   / _ \ / _` |/ _` |/ _ \ '__|
//     / /___| (_) | (_| | (_| |  __/ |
//     \____/ \___/ \__, |\__, |\___|_|
//                  |___/ |___/        v0.1
//      <ramharter>

/// @file examples.cpp
/// @brief All example code used in the doxygen examples

#include "log.h"

void init_example() {
  //! [init default]
  // create a default logger
  Logger *log = new Logger();
  log->always("This is a default logger configuration\n");
  delete log;
  //! [init default]

  //! [init file]
  // create a file logger
  log = new Logger("test0.log", CfgLog::CLogLevelDefault, CfgLog::CLogProfileDefault);
  log->always("This logger writes to a file\n");
  delete log;
  //! [init file]

  //! [init cfglog]
  // Create a new CfgLog object
  CfgLog *cfg = new CfgLog();

  cfg->logToFile = true;                                              // enable logging to file
  strncpy(cfg->logfile, "test1.log", CfgLog::CMaxPathLen);            // set path to logfile
  cfg->profile = CfgLog::ELogProfileUser;                             // set profile to user defined
  strncpy(cfg->pattern, "&tim&sep&msg&end", CfgLog::CMaxPatternLen);  // set custom message pattern

  // Create a Logger object from the config
  log = new Logger(cfg);
  log->always("This is a cfgLog-configured logger\n");

  delete log;
  delete cfg;
  //! [init cfglog]

  //! [init reinit]
  // Create a new default Logger
  log = new Logger();
  log->always("This is a default logger");

  // Create a CfgLog object and set the profile to verbose
  cfg = new CfgLog();
  cfg->profile = CfgLog::ELogProfileVerbose;

  // Call the logger's init() method with the custom config
  log->init(cfg);
  log->always("This is the new configuration");

  delete log;
  delete cfg;
  //! [init reinit]
}

void pattern_example() {
  //! [pattern example]
  Logger *log = new Logger();
  log->always("This is the default logger configuration");

  // set a user pattern that shows the loglevel and the time
  // and add a vertical separator between them
  if (log->setPattern("&tim&sep&lev&sep&msg") == Logger::EErr) {
    log->error("Failed to set pattern\n");
  } else {
    log->always("This is our new pattern\n");
    log->always("Current pattern set: %s\n", log->getPattern());
  }
  delete log;
  //! [pattern example]
}

void userPatternShorthand_example() {
  //! [shorthand example]
  // Create a CfgLog object
  CfgLog *cfg = new CfgLog();

  cfg->useUsrPattern = true;                                          // enable user pattern shorthands
  cfg->addUsrPattern(0, " ~!~ ");                                     // add shorthand to position 0
  cfg->addUsrPattern(1, " --- ");                                     // add shorthand to position 1
  cfg->profile = CfgLog::ELogProfileUser;                             // set profile to user defined
  strncpy(cfg->pattern, "&us0&lev&us1&msg", CfgLog::CMaxPatternLen);  // set custom message pattern

  // Create a Logger object from the config
  Logger *log = new Logger(cfg);
  log->always("This is a message using user defined pattern shorthands\n");
  log->always("Current pattern set: %s\n", log->getPattern());
  log->always("The user patterns are \"%s\" and \"%s\"\n", cfg->getUsrPattern(0), cfg->getUsrPattern(1));

  delete log;
  delete cfg;
  //! [shorthand example]
}

void color_example() {
  //! [color example]
  // Create a CfgLog object
  CfgLog *cfg = new CfgLog();

  cfg->useColor = true;                       // enable colors
  cfg->color = CfgLog::EColorGreen;           // set color green
  cfg->profile = CfgLog::ELogProfileDefault;  // set default profile to include log level output

  // Create a Logger object from the config
  Logger *log = new Logger(cfg);
  log->always("This is a colorful logger");
  log->warning("This is a warning");
  log->error("This is an error");
  log->critical("This is critical");

  delete log;
  delete cfg;
  //! [color example]
}

void basic_usage_example() {
  //! [profile example]
  // Create a new Logger object
  Logger *log = new Logger();

  // The default configuration uses logging profile None, which prints only the message to stdout
  log->error("This is an error");
  log->info("This is an information");
  log->always("Current pattern set: %s\n", log->getPattern());

  // minimal profile
  log->setProfile(CfgLog::ELogProfileMinimal);
  log->error("This is an error in minimal profile");
  log->info("This is an information in minimal profile");
  log->always("Current pattern set: %s\n", log->getPattern());

  // default profile
  log->setProfile(CfgLog::ELogProfileDefault);
  log->error("This is an error in default profile");
  log->info("This is an information in default profile");
  log->always("Current pattern set: %s\n", log->getPattern());

  // verbose profile
  log->setProfile(CfgLog::ELogProfileVerbose);
  log->error("This is an error in verbose profile");
  log->info("This is an information in verbose profile");
  log->always("Current pattern set: %s\n", log->getPattern());

  delete log;
  //! [profile example]

  //! [level example]
  // Create a new Logger object
  log = new Logger();
  log->setProfile(CfgLog::ELogProfileDefault);

  for (int i = 0; i <= CfgLog::ELogDebug; i++) {

    log->setLevel((CfgLog::level_e)i);

    log->always("--- Test log level %s start ---", CfgLog::CLogMsgLevel[i]);
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

  delete log;
  //! [level example]
}

int main(void) {

  Logger *mainLog = new Logger();

  mainLog->always("Starting init example...");
  init_example();
  mainLog->always("\nStarting basic usage example...");
  basic_usage_example();
  mainLog->always("\nStarting pattern example...");
  pattern_example();
  mainLog->always("\nStarting user pattern shorthand example...");
  userPatternShorthand_example();
  mainLog->always("\nStarting color example...");
  color_example();

  delete mainLog;
  return 0;
}
