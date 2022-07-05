struct arguments
{
  char *args[4];
  char *orgid;
  char *typeid;
  char *deviceid;
  char *token;
};

int config_watson(IoTPConfig **config, struct arguments *arg);
int init_watson(IoTPConfig **config, IoTPDevice **device);
int sendData_watson(IoTPDevice **device);
void cleanup_watson(IoTPConfig **config, IoTPDevice **device, int action);