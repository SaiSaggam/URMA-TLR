#ifndef __ETHREGS_H__
#define __ETHREGS_H__

int reg_read(int port, int phyreg, int *value);
int reg_write(int port, int phyreg, int value);
int is_linkup(int port);

#endif

