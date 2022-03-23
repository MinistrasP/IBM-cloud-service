#include <libubox/blobmsg_json.h>
#include <libubus.h>
#include <syslog.h>
#include "ubus_mem.h"

int rc = 0;

enum {
	MEMORY_TOTAL,
	MEMORY_FREE,
	MEMORY_CACHED,
	__MEMORY_MAX,
};

enum {
	MEMORY_DATA,
	__INFO_MAX,
};

static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
	[MEMORY_TOTAL] = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
	[MEMORY_FREE] = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
	[MEMORY_CACHED] = { .name = "cached", .type = BLOBMSG_TYPE_INT64 },
};

static const struct blobmsg_policy info_policy[__INFO_MAX] = {
	[MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};

static void board_cb(struct ubus_request *req, int type, struct blob_attr *msg) {
	struct memoryInfo *buff = (struct memoryInfo *)req->priv;
	struct blob_attr *tb[__INFO_MAX];
	struct blob_attr *memory[__MEMORY_MAX];

	blobmsg_parse(info_policy, __INFO_MAX, tb, blob_data(msg), blob_len(msg));
	if (!tb[MEMORY_DATA]) {
		syslog(LOG_ERR, "IBM Cloud service: No memory data recieved");
		rc=-1;
		return;
	}

	blobmsg_parse(memory_policy, __MEMORY_MAX, memory, blobmsg_data(tb[MEMORY_DATA]), 
	   		      blobmsg_data_len(tb[MEMORY_DATA]));

	buff->memory_total = blobmsg_get_u64(memory[MEMORY_TOTAL])/1000;
	buff->memory_free = blobmsg_get_u64(memory[MEMORY_FREE])/1000;
	buff->memory_cached = blobmsg_get_u64(memory[MEMORY_CACHED])/1000;
}

int get_mem(struct memoryInfo *buff)
{
	struct ubus_context *ctx;
	uint32_t id;

	ctx = ubus_connect(NULL);
	if (!ctx) {
		syslog(LOG_ERR, "IBM Cloud service: Failed to connect to ubus");
		return -1;
	}
	if (ubus_lookup_id(ctx, "system", &id) || 
	ubus_invoke(ctx, id, "info", NULL, board_cb, buff, 3000)) {
		syslog(LOG_ERR, "IBM Cloud service: Cannot request memory info from procd");
		rc = -1;
		return -1;
	}

	ubus_free(ctx);
	return 0;
}