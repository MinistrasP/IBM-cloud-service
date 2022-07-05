#include <stdio.h>  
#include <signal.h> 
#include <memory.h> 
#include <syslog.h>
#include <iotp_device.h>
#include <stdlib.h>
#include <argp.h>
#include "ubus_mem.h"
#include "ibm_cloud_service.h"

volatile int interrupt = 0;

const char *argp_program_version = "argp-ex2 1.0";
const char *argp_program_bug_address = "<kasparas.elzbutas@teltonika.lt>";
static char doc[] = "IBM-Cloud-Program usage:\n"
                     "Takes 4 arguments: \n"
                     "-o <orgId> - organization ID\n"
                     "-t <typeId> - device type ID\n"
                     "-d <deviceId> - device ID\n"
                     "-k <token> - secure token from IBM cloud\n";

static char args_doc[] = "-o (OrgId), -t (TypeId), -d (DeviceId), -k (Token)";

static struct argp_option options[] = {
  {"orgId",    'o',"OrgId", 0,     "Organization ID" },
  {"typeId",   't',"TypeId", 0,    "Device type ID" },
  {"deviceId", 'd',"DeviceId", 0,  "Device ID"},
  {"token",    'k',"Token", 0,     "Authentication token" },
  { 0 }
};

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
    int *arg_count = state->input;
    switch (key){
        case 'o':
          arguments->orgid = arg;
        break;
        case 't':
          arguments->typeid = arg;
        break; 
        case 'd':
          arguments->deviceid = arg;
        break; 
        case 'k':
          arguments->token = arg;
        break;
        case ARGP_KEY_ARG:
          argp_usage (state);
        break;
    }
 return 0;
}  

static struct argp argp = { options, parse_opt, args_doc, doc };

static void sigHandler(int sig) 
{
    signal(SIGINT, NULL);
    syslog(LOG_ERR, "IBM Cloud service: Received signal: %d, terminating program\n", sig);
    interrupt = 1;
}

int main (int argc, char **argv)
{
    IoTPConfig *config = NULL;
    IoTPDevice *device = NULL;
    struct arguments args = {0};
    int rc = 0;
    int clean = 0;

    openlog("syslog_log", LOG_PID, LOG_USER);
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    syslog(LOG_INFO, "IBM Cloud service: Started");
    argp_parse (&argp, argc, argv, ARGP_NO_ARGS, 0, &args);
    
    if(args.orgid == NULL || args.typeid == NULL ||
     args.deviceid == NULL || args.token == NULL){
       syslog(LOG_ERR, "IBM Cloud service: Some fields are empty");
       goto cleanup;
    }
    rc = config_watson(&config, &args);
    if(rc != 0){
        clean = 0;
        goto cleanup;
    }
    rc = init_watson(&config, &device);
    if(rc != 0){
        cleanup_watson(&config, &device, 1);
        goto cleanup;
    }
    rc = sendData_watson(&device);
    if(rc != 0){
        syslog(LOG_INFO, "IBM Cloud service: Finished sending data");
        cleanup_watson(&config, &device, 2);
        goto cleanup;
    }

    cleanup:
        closelog();
        return 1;
}