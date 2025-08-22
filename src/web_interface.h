#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <WebServer.h>

// Web interface functions
String generateConfigPage(int currentModel = 0);
String generateStatusHTML(int currentModel = 0);
String generateACControlForm(int currentModel = 0);
String generateQuickActions();
String generateSystemControls();
String generateJavaScript();

#endif // WEB_INTERFACE_H
