1.Contents:
	a) GPL: the source tree directory.
	b) buildroot-gcc342.tar.bz2 : the toolchains used to build the firmware
	c) GPL_readme.txt: instructions of building firmware.

2.How to make?
	a) The compile environment we used is X86 CPU, Fedora 5 with Squashfs LZMA.
	b) Untar buildroot-gcc342.tar.bz2 to /opt . 
	   You will get a directory "/opt/buildroot-gcc342".
	c) Untar xxx_GPL_xxx.tar.bz2 to /tmp.
	d) Login as root.
	e) cd /tmp/xxx_GPL_xxx/GPL, and type "make" to bulid kernel and filesystem.
	f) After "make", you will get firmware image "/tmp/xxx_GPL_xxx/GPL/image/firmware.bin".
	   **Please make sure the size of your kernel(image/kernel.bin) must 
	   **be less than 1MB and file system (image/rootfs.bin) must be less than 2.6MB. 

3.How to install?
	a) Upgrade firmware.bin via router's Web UI. (please refer your user manual)
	   
	   *NOTE*, I have to say, if you modified something and cause some errors,
	   the device may not be able to restart again.
	   
	   So, PLEASE MAKE SURE YOU REALLY KNOW WHAT YOU ARE DOING BEFORE YOU
	   UPGRADE ANY UNOFFICIAL FIRMWARE.
	   
4.Some hints
	The plateform we used on this project is mips, yeah, you might have
	known that after unpacking the cross compiler.
	It means, every change you want to make, could be verified in PC
	first. It's important, or you may need to restore the flash image
	much often.

