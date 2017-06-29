
#ifndef __ASIF__
#define __ASIF__

typedef struct {
	int	(*lang_id)(char *accept_language);
	char	*(*lang_tag)(int id);
} asl_lang_t;

typedef struct {
	char *(*type_name)(int type_id);
	int	(*type_id)(char *ext_name);
} asl_mime_t;

typedef struct {
	int	(*user_id)(char *user_name);
	char	*(*user_pass)(int user_id);
	//char	*(*user_name)(int user_id);
} asl_user_t;

typedef struct {
	asl_lang_t	lang;
	asl_mime_t	mime;
	asl_user_t	user;
} aux_shim_t;

extern aux_shim_t	asl;

#define AUTH_STATE_START	0
#define AUTH_STATE_PROCESS	1
#define AUTH_STATE_FINISH	2	 

#define AUTH_TBL_NUM 10
typedef struct {
	int id;
	char user[32];
	char pass[32];
}authenticate_t;
authenticate_t authen_table[AUTH_TBL_NUM];


void init_asif(char *parm);

#endif
