#include <stdio.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define IP_STR_LEN 15
#define MAC_SEP_CHR ':'
#define MAC_STR_LEN 17
#define MAC_ADDR_LEN 6
#define MAC_ADDR_NUM 16
#define MAGIC_PACK_LEN 102

void dump_mac_addr(const char *cmd,const unsigned char* mac_addr, size_t mac_addr_len) 
{
	int i;
	printf("%s ", cmd);
	for (i = 0; i < mac_addr_len; ++i) {
		if (i != 0) {
			printf("%c", MAC_SEP_CHR);
		}
		printf("%X", mac_addr[i]);
	}
	printf("\n");
}

bool is_hex(char chr)
{
	if (chr >= '0' && chr <= '9') {
		return true;
	} else if (chr >= 'a' && chr <= 'z') {
		return true;
	} else if (chr >= 'A' && chr <= 'Z') {
		return true;
	} else {
		return false;
	}
}

char hex_to_char(char chr_hex) {
	if (chr_hex <= 9) {
		return chr_hex + '0';
	} else {
		return chr_hex - 10 + 'A';
	}
}

unsigned char char_to_hex(char chr) {
	if (chr >= '0' && chr <= '9') {
		return chr - '0';
	} else if (chr >= 'a' && chr <= 'z') {
		return chr - 'a' + 10;
	} else if (chr >= 'A' && chr <= 'Z') {
		return chr - 'A' + 10;
	} else {
		return 0xF0;
	}
}

void get_mac_half_byte(const unsigned char mac_byte, bool is_low, char *half_byte)
{
	*half_byte = is_low ? (mac_byte & 0xF) : (mac_byte >> 4);
	*half_byte = hex_to_char(*half_byte);
}

void set_mac_half_byte(unsigned char *mac_byte, bool is_low, char half_byte) 
{
	if (!is_low) {
		*mac_byte = *mac_byte | (char_to_hex(half_byte) << 4);
	} else {
		*mac_byte = *mac_byte | char_to_hex(half_byte);
	}
}

int mac_addr_to_mac_str(const unsigned char* mac_addr, size_t mac_addr_len, char* mac_str, size_t mac_str_len)
{
	if (NULL == mac_addr || NULL == mac_str || mac_addr_len < MAC_ADDR_LEN || mac_str_len < MAC_STR_LEN) {
		return -1;
	}
	memset(mac_str, 0, mac_str_len);
	int i, v, j;
	for (i = 0, j = 0; i < MAC_STR_LEN; i++) {
		v = (i+1) % 3;
		if (v == 0) {
			mac_str[i] = MAC_SEP_CHR;
		} else {
			j = i / 3;
			if (v == 1) {
				get_mac_half_byte(mac_addr[j], false, &mac_str[i]);
			} else {
				get_mac_half_byte(mac_addr[j], true, &mac_str[i]);
			}
		}
	}
	return 0;
}

int mac_str_to_mac_addr(unsigned char* mac_addr, size_t mac_addr_len, const char* mac_str, size_t mac_str_len)
{
	if (NULL == mac_addr || NULL == mac_str || mac_addr_len < MAC_ADDR_LEN || mac_str_len < MAC_STR_LEN) {
		return -1;
	}
	memset(mac_addr, 0, mac_addr_len);
	int i, v, j;
	for (i = 0, j = 0; i < MAC_STR_LEN; i++) {
		v = (i+1) % 3;
		if (v == 0) {
			continue;
		}
		j = i / 3;
		if (v == 1) {
			set_mac_half_byte(&mac_addr[j], false, mac_str[i]);
		} else {
			set_mac_half_byte(&mac_addr[j], true, mac_str[i]);
		}
	}

	return 0;
}

// FF FF FF FF FF FF FF    +   MacAddr×16    +    4-6ByteSecret(null)
int gen_magic_packet(unsigned char* mac_addr, size_t mac_addr_len, unsigned char* magic_pack, size_t magic_pack_len)
{
	if (NULL == mac_addr || NULL == magic_pack || mac_addr_len < MAC_ADDR_LEN || magic_pack_len < MAGIC_PACK_LEN) {
		return -1;
	}
	int i, j;
	for(i=0; i < MAC_ADDR_LEN; i++) { 
		magic_pack[i] = 0xFF;    
	}
	for(i=1; i < MAC_ADDR_NUM + 1; i++) {
		for(j=0; j < MAC_ADDR_LEN; j++) {
			magic_pack[i * MAC_ADDR_LEN + j] = mac_addr[j];
		}
	} 
	return 0;
}

int create_socket()
{
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));
	return sockfd;
}

int init_sockaddr(struct sockaddr_in *addr, uint16_t port, const char *ip)
{
	memset((void*)addr, 0, sizeof(addr));
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = inet_addr(ip);
	return 0;
}

void usage(int argc, char* argv[])
{
	char ip_str[IP_STR_LEN + 1] = { "192.168.255.255" };
	char mac_str[MAC_STR_LEN + 1] = {"1A:2B:3C:4D:5E:6F"};
	unsigned char mac_addr[MAC_ADDR_LEN] = {0};
	mac_str_to_mac_addr(mac_addr, sizeof(mac_addr), mac_str, strlen(mac_str));
	mac_addr_to_mac_str(mac_addr, sizeof(mac_addr), mac_str, sizeof(mac_str));
	printf("Usage: %s <MacAddr> <BroadcastAddr>\n", argv[0]);
	printf("\teg: %s %s %s\n", argv[0], mac_str, ip_str);
}

int main(int argc, char* argv[])
{
	uint16_t port = 10000;
	char ip_str[IP_STR_LEN + 1] = {0};
	char mac_str[MAC_STR_LEN + 1] = {0};
	unsigned char mac_addr[MAC_ADDR_LEN] = {0};
	unsigned char magic_packet[MAGIC_PACK_LEN];

	if (argc != 3) {
		usage(argc, argv);
		return -1;
	} else {
		strncpy(mac_str, argv[1], sizeof(mac_str));
		strncpy(ip_str, argv[2], sizeof(ip_str));
		mac_str_to_mac_addr(mac_addr, sizeof(mac_addr), mac_str, strlen(mac_str));
	}
	
	int ret = gen_magic_packet(mac_addr, sizeof(mac_addr), magic_packet, sizeof(magic_packet));
	if (0 != ret) {
		return ret;
	}

	int sockfd = create_socket();
	if (sockfd < 0) {
		return -3;
	}

	struct sockaddr_in addr;
	init_sockaddr(&addr, port, ip_str);

	sendto(sockfd, magic_packet, sizeof(magic_packet), 0, (struct sockaddr *)&addr, sizeof(addr));
	close(sockfd);
	
	dump_mac_addr("\ttarget mac addr :", mac_addr, sizeof(mac_addr));
	printf("\ttarget ip addr : %s\n", ip_str);
	return 0;
}
