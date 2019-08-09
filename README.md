# Logger

A versatile and fully customizable logger for c++ applications.

### Features

- optional file logging
- pre-defined log profiles for easy configuration
- fully customizable log string (logstamp, pid, loglevel, custom elements)
- nine different loglevels
- detailed documentation and examples

### Repo structure

The repo is structured as follows:

log<br>
 &nbsp;&nbsp;|_ bin/          &nbsp;&nbsp;&nbsp;...binary files<br>
 &nbsp;&nbsp;|_ doc/          &nbsp;&nbsp;...documentation<br>
 &nbsp;&nbsp;|_ inc/          &nbsp;&nbsp;&nbsp;&nbsp;...header files<br>
 &nbsp;&nbsp;|_ proj/         &nbsp;&nbsp;...build files<br>
 &nbsp;&nbsp;|_ src/          &nbsp;&nbsp;&nbsp;&nbsp;...source files<br>

### Compilation and Usage

To compile, move into <i>proj/</i> and call<br>
<b>make lib</b> to build just the library.<br>
<b>make examples</b> to build the examples<br>
<b>make doc</b> to build the documentation<br>
or <b>make all</b> to build everything.<br>

All compile units can subsequently be found in <i>bin/</i> while the documentation can be found in <i>doc/</i><br>
In order to use the CPPLogger in your application, simply compile with the library.<br>
Just include the <i>log.h</i> headerfile in your application and link to the library <i>libcpplogging.a</i> during compilation.
