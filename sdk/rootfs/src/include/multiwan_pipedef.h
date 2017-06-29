/* Keep these define the same as commander.h  */

#define FIFO_PIPE				"/var/multiwan_pipe"

#define WAN						0x01
#define ROUTE					0x02
#define NAT						0x03

#define NOCMD					0x00
#define INIT					0x01
#define START					0x02
#define STOP					0x03
#define RESTART					0x04
#define RENEW					0x05
#define RELEASE					0x06
#define POLICY					0x07

#define PIPE_CMD_READY			0x00
#define PIPE_CMD_WAITTING		0x01

struct pipe_data {
	unsigned char index;
	unsigned char action;
	unsigned char command;
	unsigned char wantype;
};
