BIN = ibmCloud_Program

.PHONY: all clean

all: ibmCloud_Program

ibmCloud_Program: ubus_mem.c ibm-cloud-service.c main.c
	$(CC) -o $@ $^ -lblobmsg_json -lubox -lubus -libm-iot -largp

clean:
	rm -rf ibmCloud_Program