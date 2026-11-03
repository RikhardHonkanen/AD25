#ifndef WIFI_H
#define WIFI_H

void wifi_connect(const char *ssid, const char *pass);

void wifi_ota_update_set_server(const char *ip, const char *port);

#endif