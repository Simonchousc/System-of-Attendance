CC=gcc
CFLAGS += -I/usr/include/json-c
LDFLAGS = -ljson-c -lcjson -lmysqlclient

SRCS=fun_login.c login.c cJSON.c
OBJS=$(SRCS:.c=.o)
TARGET=login.cgi

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
