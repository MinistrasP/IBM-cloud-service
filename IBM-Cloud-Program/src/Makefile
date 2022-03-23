BIN = IBM-Cloud-Program

.PHONY: all clean

all: IBM-Cloud-Program

IBM-Cloud-Program: ubus_mem.c ibm-cloud-service.c main.c
	$(CC) -o $@ $^ -lblobmsg_json -lubox -lubus -libm-iot -largp

clean:
	rm -rf IBM-Cloud-Program