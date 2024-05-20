/*
	功能：针对于ISO/IEC14443 TypeB类卡的RFID操作
	读写器型号兼容：MFRC522,MFRC523,FM17522,FM17550等
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "rfid_lib.h"
#include "uart_init.h"
#include <arpa/inet.h>

void sendIc(unsigned char *arg)
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		perror("socket");
		return;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	inet_pton(AF_INET, "10.9.29.135", (void *)&(addr.sin_addr));

	sendto(sockfd, arg, 4, 0, (struct sockaddr *)&addr, sizeof(addr));

	close(sockfd);
	return;
}

int main(void)
{

	unsigned char type[2], ret, card_id[4];
	int i;
	rfid_reset();
	rfid_carda_init();
	while (1)
	{
		ret = rfid_carda_request(PICC_REQALL, type);
		if (ret == OK)
		{
			printf("type = 0x%02x%02x\n", type[0], type[1]);
			// WaitCardOff();
		}
		else
			continue;

		ret = rfid_anticoll(card_id);
		if (ret == OK)
		{
			printf("card ID:");
			for (i = 0; i < 4; i++)
				printf("0x%02x ", card_id[i]);

			puts("");
			// 发送ic
			sendIc(card_id);
			WaitCardOff();
		}
		else
			continue;
	}
	return 0;
}
