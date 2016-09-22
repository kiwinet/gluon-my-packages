#include <respondd.h>

#include <json-c/json.h>
#include <libgluonutil.h>
#include <libplatforminfo.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/utsname.h>
#include <sys/vfs.h>

static struct json_object * airtime2(void) {
        double *act2 = gluonutil_read_line("/tmp/act2");
        double *bus2 = gluonutil_read_line("/tmp/bus2");
        
        return json_object_new_double(bus2 / act2);
}

static struct json_object * airtime5(void) {
        double *act5 = gluonutil_read_line("/tmp/act5");
        double *bus5 = gluonutil_read_line("/tmp/bus5");
        
        return json_object_new_double(bus5 / act5);
}

static struct json_object * chan2(void) {
        char *chan2 = gluonutil_read_line("/tmp/channel2");
        
        return json_object_new_string(chan2);
}

static struct json_object * chan5(void) {
        char *chan5 = gluonutil_read_line("/tmp/channel5");
        
        return json_object_new_string(chan5);
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


const struct respondd_provider_info respondd_providers[] = {
	{"nodeinfo", respondd_provider_nodeinfo},
	{}
};
