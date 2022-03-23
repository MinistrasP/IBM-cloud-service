#include <stdio.h>  
#include <signal.h> 
#include <memory.h> 
#include <syslog.h>
#include <iotp_device.h>
#include <stdlib.h>
#include <argp.h>
#include "ubus_mem.h"
#include "ibm-cloud-service.h"

volatile int interrupt = 0;

const char *argp_program_version = "argp-ex2 1.0";
const char *argp_program_bug_address = "<kasparas.elzbutas@teltonika.lt>";
static char doc[] = "IBM-Cloud-Program usage:\n"
                     "Takes 4 arguments: \n"
                     "orgId - organization ID\n"
                     "typeId - device type ID\n"
                     "deviceId - device ID\n"
                     "token - secure token from IBM cloud\n";

static char args_doc[] = "ARG1(orgId), ARG2(typeId), ARG3(deviceId), ARG4(token)";

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;
  switch (key)
    {
    case ARGP_KEY_ARG:
      if (state->arg_num >= 4)
        argp_usage (state);
      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 4)
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

static struct argp argp = { NULL, parse_opt, args_doc, doc };

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
    struct arguments arguments;
    int rc = 0;
    int clean = 0;

    openlog("syslog_log", LOG_PID, LOG_USER);
    argp_parse (&argp, argc, argv, 0, 0, &arguments);
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    syslog(LOG_INFO, "IBM Cloud service: Started");

    rc = config_watson(&config, &arguments);
    if(rc != 0){
        clean = 0;
        goto cleanup;
    }
    rc = init_watson(&config, &device);
    if(rc != 0){
        clean = 1;
        goto cleanup;
    }
    rc = sendData_watson(&device);
    if(rc != 0){
        syslog(LOG_INFO, "IBM Cloud service: Finished sending data");
        clean = 2;
        goto cleanup;
    }

    cleanup:
        cleanup_watson(&config, &device, clean);
        closelog();
        return 1;
}