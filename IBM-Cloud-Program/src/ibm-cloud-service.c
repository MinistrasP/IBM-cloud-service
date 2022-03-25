#include <stdio.h>
#include <syslog.h>
#include <iotp_device.h>
#include "ubus_mem.h"
#include "ibm-cloud-service.h"

extern volatile int interrupt;

void MQTTTraceCallback (int level, char * message)
{
    if ( level > 0 )
        syslog(LOG_INFO, "IBM Cloud service: Trace: %s\n", message? message:"NULL");
}


int config_watson(IoTPConfig **config, struct arguments *arg)
{
    int rc = 0;

    rc = IoTPConfig_create(config, NULL);
    if(rc != 0) {
        syslog(LOG_ERR, "IBM Cloud service: Failed to create device");
        return rc;
    }
    rc = IoTPConfig_setProperty(*config, "identity.orgId", arg->orgid);
    if(rc != 0) {
        syslog(LOG_ERR, "IBM Cloud service: Failed to set orgId");
        goto cleanup;
    }
    rc = IoTPConfig_setProperty(*config, "identity.typeId", arg->typeid);
    if(rc != 0) {
        syslog(LOG_ERR, "IBM Cloud service: Failed to create typeId");
        goto cleanup;
    }
    rc = IoTPConfig_setProperty(*config, "identity.deviceId", arg->deviceid);
    if(rc != 0) {
        syslog(LOG_ERR, "IBM Cloud service: Failed to set deviceId");
        goto cleanup;
    }
    rc = IoTPConfig_setProperty(*config, "auth.token", arg->token);
    if(rc != 0) {
        syslog(LOG_ERR, "IBM Cloud service: Failed to set token");
        goto cleanup;
    }
    rc = IoTPConfig_setProperty(*config, "options.mqtt.keepAlive", "30");
    if(rc != 0) {
        syslog(LOG_ERR, "IBM Cloud service: Failed to set token");
        goto cleanup;
    }

    return rc;

    cleanup:
    IoTPConfig_clear(*config);
    return rc;
}

int init_watson(IoTPConfig **config, IoTPDevice **device)
{
    int rc = 0;

    rc = IoTPDevice_create(device, *config);
    if (rc) {
        syslog(LOG_ERR, "IBM Cloud service: Unable to create IBM watson device");
        return rc;
    }
    rc = IoTPDevice_setMQTTLogHandler(*device, &MQTTTraceCallback);
    if ( rc != 0 ) {
        syslog(LOG_ERR, "IBM Cloud service: Failed to set MQTT Trace handler: rc=%d\n", rc);
        IoTPDevice_destroy(*device);
        return rc;
    }
    rc = IoTPDevice_connect(*device);
    if (rc) {
        syslog(LOG_ERR, "IBM Cloud service: Unable to connect to IBM watson cloud");
        IoTPDevice_destroy(*device);
        return rc;
    }
    return rc;
}

int sendData_watson(IoTPDevice **device)
{
    int rc = 0;
    struct memoryInfo *buffer;
    buffer = (struct memoryInfo *) malloc(sizeof(struct memoryInfo));

    while(interrupt == 0) {
        rc = get_mem(buffer);
        if(rc != 0) {
            syslog(LOG_ERR, "IBM Cloud service: Failed to get memory");
            free(buffer);
            return 1;
        }
        char *data[50];
        sprintf(data, "{\"Memory\" : {\"Total memory\": %d, \"Free memory\": %d , \"Buffered memory\": %d}}"
                                         ,buffer->memory_total, buffer->memory_free, buffer->memory_cached);
        rc = IoTPDevice_sendEvent(*device, "status", data, "json", QoS0, NULL);
        if(rc != 0) {
            syslog(LOG_ERR, "IBM Cloud service: Failed to send data");
            free(buffer);
            return 1;     
        }
        sleep(1);
    }
    free(buffer);
    return 1;
}

void cleanup_watson(IoTPConfig **config, IoTPDevice **device, int action)
{
    switch(action)
    {
        case 0:
        break;

        case 1:
            IoTPConfig_clear(*config);
        break;

        case 2:
            IoTPConfig_clear(*config);
        break;

        case 3:
            IoTPDevice_disconnect(*device);
            IoTPDevice_destroy(*device);
            IoTPConfig_clear(*config);
        break;
        default:
        break;
    }
}