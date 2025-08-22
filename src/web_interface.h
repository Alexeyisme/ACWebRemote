#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <WebServer.h>

// Web interface functions
String generateConfigPage();
String generateStatusHTML();
String generateACControlForm();
String generateQuickActions();
String generateSystemControls();
String generateJavaScript();

#endif // WEB_INTERFACE_H
