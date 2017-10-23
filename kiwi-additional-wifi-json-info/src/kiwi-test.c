#include <stdio.h>

#include "kiwi.h"

int main() {
	struct kiwi_wifi_info *i = kiwi_get_wifi_info();
	printf("24: %u (%u), 50: %u (%u)\n", i->c24, i->t24, i->c50, i->t50);
	struct kiwi_airtime *a = kiwi_get_airtime();
	printf("24: %f 50: %f\n", a->a24, a->a50);
}
