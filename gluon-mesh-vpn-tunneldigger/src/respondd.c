/*
  Copyright (c) 2016, Matthias Schiffer <mschiffer@universe-factory.net>
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

#include <json-c/json.h>
#include <libgluonutil.h>
#include <uci.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>


static struct json_object * get_tunneldigger_version(void) {

	const char *version = '0.4.7';
	struct json_object *ret = gluonutil_wrap_string(version);
	return ret;
}

static struct json_object * get_tunneldigger(void) {
	bool enabled = false;

	struct uci_context *ctx = uci_alloc_context();
	ctx->flags &= ~UCI_FLAG_STRICT;

	struct uci_package *p;
	if (uci_load(ctx, "tunneldigger", &p))
		goto disabled;

	struct uci_section *s = uci_lookup_section(ctx, p, "broker");
	if (!s)
		goto disabled;

	const char *enabled_str = uci_lookup_option_string(ctx, s, "enabled");
	if (!enabled_str || !strcmp(enabled_str, "1"))
		enabled = true;

 disabled:

	uci_free_context(ctx);

	struct json_object *ret = json_object_new_object();
	json_object_object_add(ret, "version", get_tunneldigger_version());
	json_object_object_add(ret, "enabled", json_object_new_boolean(enabled));
	return ret;
}


static struct json_object * respondd_provider_nodeinfo(void) {
	struct json_object *ret = json_object_new_object();
	struct json_object *software = json_object_new_object();


	struct json_object *get_tunnel = get_tunneldigger();
	if (get_tunnel)
		json_object_object_add(software, "tunneldigger", get_tunnel);


	json_object_object_add(ret, "software", software);
	return ret;
}


const struct respondd_provider_info respondd_providers[] = {
	{"nodeinfo", respondd_provider_nodeinfo},
	{}
};
