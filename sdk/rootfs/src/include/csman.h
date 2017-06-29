/**
 * @file csman.h
 *
 * CSMan Basic API Header File.
 *
 * You should #include this file to link libcsman.so
 */

#ifndef _CSMAN_H

#define _CSMAN_H			1

#include <unistd.h>			//for size_t & ssize_t
#include <stdint.h>			//for uint16_t
#include <sys/types.h>		//for off_t
#include <sys/errno.h>		//for system errno

/* type of CSID */
typedef uint32_t			CSID_T;

/* CSMan Basic APIs */
int open_csman(const char *config, int flags);
ssize_t read_csman(int fd, CSID_T csid, void *buf, size_t count, int flags);
ssize_t write_csman(int fd, CSID_T csid, const void *buf, size_t count, int flags);
//int chkdirty_csman(int fd, CSID_T *pcsid, int flags);
ssize_t dump_csman(int fd, CSID_T *pcsid, void *buf, size_t count);
int close_csman(int fd);

/* Design Constrains */
#define MAX_ITEM_SIZE		1024

/* Macros to manipulate CSID */
#define CSID_GROUP(x)		((x)&0xFFFF0000)	//get group id from csid
#define CSID_MEMBER(x)		((x)&0x0000FFFF)	//get member id from csid
#define CSID_IS_CONFIG(x)	(!((x)&0x80000000))	//is it a config item?
#define CSID_IS_STATUS(x)	((x)&0x80000000)	//is it a status item?

// return codes for csman APIs
#define CSM_RC_SUCCESS		0
#define CSM_RC_NOID			-ENODATA	//-61
#define CSM_RC_BUFFER		-ENOBUFS	//-105
#define CSM_RC_OPENED		-EMFILE		//-24
#define CSM_RC_OPENFAIL		-EACCES		//-13
#define CSM_RC_BADFD		-EBADF		//-9
#define CSM_RC_EINVAL		-EINVAL		//-22
#define CSM_RC_OTHERS		-EIO		//-5

// flags for open_csman()
#define CSM_O_EXCL		0x0080		//i.e. O_EXCL
#define CSM_O_TRUNC		0x0280		//i.e. O_TRUNC|O_EXCL

#ifndef	__CSAL__

//The following definitions are for CSMan Agent only

//flags for read_csman()
#define CSM_R_ZERO		0x0001
#define CSM_R_0xFF		0x0002
#define CSM_R_ZSTR		0x0004
#define CSM_R_NONE		0x0008
#define CSM_R_OMITABLE	(CSM_R_ZERO|CSM_R_0xFF|CSM_R_ZSTR|CSM_R_NONE)

#define CSM_R_NOLOG		0x0080	//don't log any error

#define CSM_R_TRUNC		0x0100
#define CSM_R_TRUNCZ	0x0200
#define CSM_R_EXPAND	0x0400

#define CSM_R_SNUM		0x4000
#define CSM_R_UNUM		0x8000

//flags for write_csman()
#define CSM_W_ZERO		0x0001
#define CSM_W_0xFF		0x0002
#define CSM_W_ZSTR		0x0004
#define CSM_W_NONE		0x0008
#define CSM_W_OMITABLE	(CSM_W_ZERO|CSM_W_0xFF|CSM_W_ZSTR|CSM_W_NONE)

#define CSM_W_NOLOG		0x0080	//don't log any error

#define CSM_W_RMID		0x0100
#define CSM_W_EDIT		0x0200

#define CSM_W_SNUM		0x4000
#define CSM_W_UNUM		0x8000

#endif /* !__CSAL__ */

#endif /* !_CSMAN_H */
