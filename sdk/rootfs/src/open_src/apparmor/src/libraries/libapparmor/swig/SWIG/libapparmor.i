%module LibAppArmor

%{
#include "aalogparse.h"
#include "apparmor.h"

%}

%include "typemaps.i"
%include "aalogparse.h"

/* swig doesn't like the macro magic we do in apparmor.h so the fn prototypes
 * are manually inserted here
 */

extern int aa_is_enabled(void);
extern int aa_find_mountpoint(char **mnt);
extern int aa_change_hat(const char *subprofile, unsigned long magic_token);
extern int aa_change_profile(const char *profile);
extern int aa_change_onexec(const char *profile);
extern int aa_change_hatv(const char *subprofiles[], unsigned long token);
extern int aa_change_hat_vargs(unsigned long token, int count, ...);
extern int aa_getprocattr_raw(pid_t tid, const char *attr, char *buf, int len,
			      char **mode);
extern int aa_getprocattr(pid_t tid, const char *attr, char **buf, char **mode);
extern int aa_gettaskcon(pid_t target, char **con, char **mode);
extern int aa_getcon(char **con, char **mode);
extern int aa_getpeercon_raw(int fd, char *buffer, int *size);
extern int aa_getpeercon(int fd, char **con);
