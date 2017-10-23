#pragma once

extern const unsigned int KIWI_INVALID_CHANNEL;
extern const unsigned int KIWI_INVALID_TXPOWER;
extern const double KIWI_INVALID_AIRTIME;

struct kiwi_wifi_info {
	unsigned char c24;
	unsigned char c50;
	unsigned char t24;
	unsigned char t50;
};

struct kiwi_airtime {
	double a24;
	double a50;
};

char *kiwi_get_nexthop(void);
struct kiwi_wifi_info *kiwi_get_wifi_info(void);
struct kiwi_airtime *kiwi_get_airtime(void);

#define DEBUG { printf("Reached %s:%d\n", __FILE__, __LINE__); fflush(NULL); }
