/*
  Copyright (c) 2016, Andrej Sidorenko <avita@hostega.eu>
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <respondd.h>

#include <iwinfo.h>
#include <json-c/json.h>
#include <libgluonutil.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <uci.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/utsname.h>
#include <sys/vfs.h>

static struct json_object * airtime2(void) {
        char *line = NULL;
        size_t len = 0;
        double act2 = 0;
        double bus2 = 0;
        double rez = 0;

        FILE *f = popen("iw client0 survey dump |grep 'in use' -A5|grep busy|grep -o '[0-9]*'", "r");
        if (f) {
                ssize_t r = getline(&line, &len, f);

                if (r >= 0) {
                        len = strlen(line); /* The len given by getline is the buffer size, not the string length */

                        if (len && line[len-1] == '\n')
                                line[len-1] = 0;

                        bus2 = strtod(line, NULL);
                        free(line);
                        line = NULL;
                        len = 0;
                }
                else {
                        free(line);
                        line = NULL;
                }
        }
        pclose(f);

        FILE *f1 = popen("iw client0 survey dump |grep 'in use' -A5|grep active|grep -o '[0-9]*'", "r");
        if (f1) {
                ssize_t r1 = getline(&line, &len, f1);

                if (r1 >= 0) {
                        len = strlen(line); /* The len given by getline is the buffer size, not the string length */

                        if (len && line[len-1] == '\n')
                                line[len-1] = 0;

                        act2 = strtod(line, NULL);
                        free(line);
                        line = NULL;
                        len = 0;
                }
                else {
                        free(line);
                        line = NULL;
                }

        }
        pclose(f1);

        if  (act2 > 0) {
                rez= bus2 / act2;
        }

        return json_object_new_double(rez);
}

static struct json_object * airtime5(void) {
        char *line = NULL;
        size_t len = 0;
        double act5 = 0;
        double bus5 = 0;
        double rez = 0;

        FILE *f = popen("iw client1 survey dump |grep 'in use' -A5|grep busy|grep -o '[0-9]*'", "r");
        if (f) {
                ssize_t r = getline(&line, &len, f);

                if (r >= 0) {
                        len = strlen(line); /* The len given by getline is the buffer size, not the string length */

                        if (len && line[len-1] == '\n')
                                line[len-1] = 0;

                        bus5 = strtod(line, NULL);
                        free(line);
                        line = NULL;
                        len = 0;
                }
                else {
                        free(line);
                        line = NULL;
                }
        }
        pclose(f);

        FILE *f1 = popen("iw client1 survey dump |grep 'in use' -A5|grep active|grep -o '[0-9]*'", "r");
        if (f1) {
                ssize_t r1 = getline(&line, &len, f1);

                if (r1 >= 0) {
                        len = strlen(line); /* The len given by getline is the buffer size, not the string length */

                        if (len && line[len-1] == '\n')
                                line[len-1] = 0;

                        act5 = strtod(line, NULL);
                        free(line);
                        line = NULL;
                        len = 0;
                }
                else {
                        free(line);
                        line = NULL;
                }

        }
        pclose(f1);

        if  (act5 > 0) {
                rez= bus5 / act5;
        }

        return json_object_new_double(rez);
}

static struct json_object * chan2(void) {

        const char *ifname = 'client0';

        const struct iwinfo_ops *iw = iwinfo_backend(ifname);
        if (!iw)
                return;

        int channel;
        if (iw->channel(ifname, &channel) < 0)
                return;

        return json_object_new_int(channel);
}

static struct json_object * chan5(void) {

        const char *ifname = 'client1';

        const struct iwinfo_ops *iw = iwinfo_backend(ifname);
        if (!iw)
                return;

        int channel;
        if (iw->channel(ifname, &channel) < 0)
                return;

        return json_object_new_int(channel);
}

static struct json_object * txpower2(void) {

        const char *ifname = 'client0';

        const struct iwinfo_ops *iw = iwinfo_backend(ifname);
        if (!iw)
                return;

        int txpower;
        if (iw->txpower(ifname, &txpower) < 0)
                return;

        return json_object_new_int(txpower);
}

static struct json_object * txpower5(void) {

        const char *ifname = 'client1';

        const struct iwinfo_ops *iw = iwinfo_backend(ifname);
        if (!iw)
                return;

        int txpower;
        if (iw->txpower(ifname, &txpower) < 0)
                return;

        return json_object_new_double(txpower);
}

static struct json_object * width2(void) {
        char *line = NULL;
        size_t len = 0;
        char *width2 = 'N/A';

        FILE *f = popen("iw dev client0 info | awk '/width/ {printf $6;}'", "r");
        if (f) {
                ssize_t r = getline(&line, &len, f);

                if (r >= 0) {
                        len = strlen(line); /* The len given by getline is the buffer size, not the string length */

                        if (len && line[len-1] == '\n')
                                line[len-1] = 0;

                        width2 = line;
                        free(line);
                        line = NULL;
                        len = 0;
                }
                else {
                        free(line);
                        line = NULL;
                }
        }
        pclose(f);


        return json_object_new_string(width2);
}

static struct json_object * width5(void) {
        char *line = NULL;
        size_t len = 0;
        char *width5 = 'N/A';

        FILE *f = popen("iw dev client1 info | awk '/width/ {printf $6;}'", "r");
        if (f) {
                ssize_t r = getline(&line, &len, f);

                if (r >= 0) {
                        len = strlen(line); /* The len given by getline is the buffer size, not the string length */

                        if (len && line[len-1] == '\n')
                                line[len-1] = 0;

                        width5 = line;
                        free(line);
                        line = NULL;
                        len = 0;
                }
                else {
                        free(line);
                        line = NULL;
                }

        }
        pclose(f);

        return json_object_new_string(width5);
}

static struct json_object * cbrate2(void) {

        const char *ifname = 'client0';

        const struct iwinfo_ops *iw = iwinfo_backend(ifname);
        if (!iw)
                return;

        int bitrate;
        if (iw->bitrate(ifname, &bitrate) < 0)
                return;

        return json_object_new_int(bitrate);
}

static struct json_object * cbrate5(void) {

        const char *ifname = 'client1';

        const struct iwinfo_ops *iw = iwinfo_backend(ifname);
        if (!iw)
                return;

        int bitrate;
        if (iw->bitrate(ifname, &bitrate) < 0)
                return;

        return json_object_new_int(bitrate);
}

static struct json_object * mbrate2(void) {

        const char *ifname = 'ibss0';

        const struct iwinfo_ops *iw = iwinfo_backend(ifname);
        if (!iw)
                return;

        int bitrate;
        if (iw->bitrate(ifname, &bitrate) < 0)
                return;

        return json_object_new_int(bitrate);
}

static struct json_object * mbrate5(void) {

        const char *ifname = 'ibss1';

        const struct iwinfo_ops *iw = iwinfo_backend(ifname);
        if (!iw)
                return;

        int bitrate;
        if (iw->bitrate(ifname, &bitrate) < 0)
                return;

        return json_object_new_int(bitrate);
}

static struct json_object * respondd_provider_nodeinfo(void) {
        struct json_object *ret = json_object_new_object();

        struct json_object *wireless = json_object_new_object();
        json_object_object_add(wireless, "airtime2", airtime2());
        json_object_object_add(wireless, "airtime5", airtime5());
        json_object_object_add(wireless, "chan2", chan2());
        json_object_object_add(wireless, "chan5", chan5());

        json_object_object_add(ret, "wireless", wireless);

        return ret;
}

static struct json_object * respondd_provider_statistics(void) {
        struct json_object *ret = json_object_new_object();

        struct json_object *wireless = json_object_new_object();
        json_object_object_add(wireless, "airtime2", airtime2());
        json_object_object_add(wireless, "airtime5", airtime5());
        json_object_object_add(wireless, "txpower2", txpower2());
        json_object_object_add(wireless, "txpower5", txpower5());
        json_object_object_add(wireless, "width2", width2());
        json_object_object_add(wireless, "width5", width5());
        json_object_object_add(wireless, "cbrate2", cbrate2());
        json_object_object_add(wireless, "cbrate5", cbrate5());
        json_object_object_add(wireless, "mbrate2", mbrate2());
        json_object_object_add(wireless, "mbrate5", mbrate5());

        json_object_object_add(ret, "wireless", wireless);

        return ret;
}


const struct respondd_provider_info respondd_providers[] = {
        {"nodeinfo", respondd_provider_nodeinfo},
        {"statistics", respondd_provider_statistics},
        {}
};
