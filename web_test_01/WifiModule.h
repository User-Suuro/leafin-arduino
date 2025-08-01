#pragma once

void connectToWiFi();
void sendCommand(const String& cmd, int delayMs = 2000, bool showResponse = true);
