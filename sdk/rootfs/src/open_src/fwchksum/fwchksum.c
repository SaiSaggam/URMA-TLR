/*
 * Shell script helper to get FW checksum
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PROGRAM		"fwchksum"
#define VERSION		"v1.0"

#define KERNEL_CHECKSUM_OFFSET		28
#define ROOTFS_CHECKSUM_OFFSET		44

static void show_usage_and_exit(int exitval)
{
	fprintf(exitval ? stderr : stdout,
		"Usage: %s <fw_index>\n"			\
		"%s %s\n\n"					\
		"Utility to get FW checksums\n"			\
		"\n"						\
		"   <fw_index>           0: FW1; 1: FW2\n"	\
		"\n", PROGRAM, PROGRAM, VERSION);
	exit(exitval);
}

int ReadFwHdr(char partIndex, uint32_t *kernelChecksum,
	      uint32_t *rootfsChecksum)
{
	int fd = -1;
	int ret;
	char mtdPartition[] = "/dev/mtdr0";

	mtdPartition[sizeof(mtdPartition) - 2] = partIndex;

	/* Open partition */
	fd = open(mtdPartition, O_RDONLY);
	if ( fd < 0 ) {
		fprintf(stderr, "ERROR: couldn't open partition!\n");
		ret = EXIT_FAILURE;
		goto exit;
	}

	lseek(fd, KERNEL_CHECKSUM_OFFSET, SEEK_SET);
	ret = read(fd, kernelChecksum, sizeof(uint32_t));
	if ( ret != sizeof(uint32_t) ) {
		fprintf(stderr, "ERROR: kernel checksum read error!\n");
		ret = EXIT_FAILURE;
		goto exit;
	}

	lseek(fd, ROOTFS_CHECKSUM_OFFSET, SEEK_SET);
	ret = read(fd, rootfsChecksum, sizeof(uint32_t));
	if ( ret != sizeof(uint32_t) ) {
		fprintf(stderr, "ERROR: rootfs checksum read error!\n");
		ret = EXIT_FAILURE;
		goto exit;
	}

	ret = EXIT_SUCCESS;
exit:
	if ( fd >= 0 )
		close(fd);

	return ret;
}

int main(int argc, char **argv)
{
	int ret;
	char kernelPartition;
	uint32_t kernelChecksum;
	uint32_t rootfsChecksum;

	if ( argc != 2 )
		show_usage_and_exit(EXIT_FAILURE);

	switch ( *argv[1] ) {
		case '0':
			kernelPartition = '2';
			break;

		case '1':
			kernelPartition = '4';
			break;

		default:
			fprintf(stderr, "ERROR: invalid FW index!\n");
			show_usage_and_exit(EXIT_FAILURE);
			break;
	}

	/* Read firmware header */
	ret = ReadFwHdr(kernelPartition, &kernelChecksum, &rootfsChecksum);
	if ( ret )
		exit(EXIT_FAILURE);

	printf("Kernel checksum: 0x%08x\n", kernelChecksum);
	printf("Rootfs checksum: 0x%08x\n", rootfsChecksum);

	return EXIT_SUCCESS;
}
