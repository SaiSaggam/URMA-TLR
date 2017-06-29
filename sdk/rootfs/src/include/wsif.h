/* vi: set sw=4 ts=4: */

/* wsif.h
 *
 * Weblet Interface 1.11
 */

#include <stdint.h>
#include <stdio.h>

#define MAX_BASE_LENGTH         (8-1)
#define SCRATCH_PAD_SIZE        4096
#define MAX_QSTR_NUM            512

struct weblet_t;
struct page_blk_t;

typedef struct {
	char *key;
	char *val;
} qstr_t;

typedef struct {
	struct weblet_t *serv;
	struct {
		int http_ver;			//useless for weblet
		int keep_alive;			//useless for weblet
	} hsl;
	struct {
		int method_id;
        char *host;
		char *r_uri;			//reduced URI
        char *url_str;          //URL String i
		int lang_id;			//0=no lang or lang not in table
		int auth_id;			//0=not auth or auth fail
		uint32_t peer_ip;		//ip address of the client   (network order)
        uint8_t peer_ip6[16];   //ipv6 address of the client (network order)
		int cookie;		        //cookie                     (shareport)
        uint32_t aes_key[16];   
        char *range;            //range of request data      (shareport)
		char *user_agent;		//requested page's user agent(shareport)
        char *hnap_auth;        //authentication for HNAP
        char *hnap_method;      //request Method for HNAP               
        struct {
            char *method;
            
            char *username;
            char *nonce;
            char *uri;
            char *qop;
            char *nc;
            char *cnonce;
            
            char *response;
        } author;
	} req;
	struct {
		int count;
		qstr_t *item;
	} form;
	struct {
		int status_code;
		int content_type_id;
		long long content_length;	//-1=length not determined
		int lang_id;			    //0=no Content-Language:
		long max_age;			    //0=uncacheable
	} res;
	struct {
		int sess_id;
		char scratch_pad[SCRATCH_PAD_SIZE];
	} state;

    struct {
        struct weblet_t     *friend_serv; 
        struct page_blk_t   *friend_page;
    } friend_info;                  //for shareport extension

    FILE *fd;
} page_blk_t;

typedef struct {
	char base[MAX_BASE_LENGTH+1];
	int attr;
	int (*init)(char *base, int attr);
	int (*serv)(page_blk_t *page);
	int (*get_data)(page_blk_t *page, void *buf, int count);
	int (*put_data)(page_blk_t *page, const void *buf, int size);
	int (*abort)(page_blk_t *page);
	int (*term)(void);
} weblet_t;

/* Supported HTTP Version */

//these items are useless for weblets
//MUST keep the value for later version is greater than older version
#define HTTP_VER_11		0x101
#define HTTP_VER_10		0x100

/* Expected action after calling serv() */

#define ACTION_CONT		0		//request is not finished, continue next step
#define ACTION_STOP		-1		//request is completed, no more action

/* Weblet Attributes */
// low 8 bits:
//		Common attributes, define them here
// next 8 bits:
//		Local attributes, define them in weblet's local header files

#define WSF_AUTO_LANGUAGE	0x0010

/* Weblet Attributes -- Cache */

#define WSF_CACHE_NONE		0x0000
#define WSF_CACHE_PAGE		0x0001

/* Method ID */

#define METHOD_UNKNOWN		0
#define METHOD_GET			1
#define METHOD_POST_URL		2
#define METHOD_POST_FILE	3
#define METHOD_POST_XML		4
#define METHOD_SUBSCRIBE	5
#define METHOD_UNSUBSCRIBE	6
#define METHOD_SHARE_GET    7
#define METHOD_SHARE_POST   8
#define METHOD_HNAP_GET     9
#define METHOD_HNAP_POST    10



/* Status Code */

//NOTE! The code need not be same as HTTP status code
//We use the same value for convenient
//HTTP Session Layer MUST treat values other than the following as error
#define WS_STATUS_OK		200
#define WS_STATUS_SEE_OTHER	303
#define WS_STATUS_BAD_REQ	400
#define WS_STATUS_UNAUTH	401
#define WS_STATUS_FORBIDDEN	403
#define WS_STATUS_NOT_FOUND	404
#define WS_STATUS_ERROR		500

/* Basic Content Type */

#define CTYPE_NONE		0
#define CTYPE_HTML		1
#define CTYPE_XML       2
#define CTYPE_CSS       3
#define CTYPE_JS        4
#define CTYPE_PLAIN     5
#define CTYPE_JPEG      6
#define CTYPE_PNG       7
#define CTYPE_GIF       8
#define CTYPE_OCTET     9
#define CTYPE_JSON      10

/* Authentication Schemes */

#define AUTH_SCHEME_CGI     0
#define AUTH_SCHEME_BASIC   1
#define AUTH_SCHEME_DIGEST  2

/* Authentication Roles */

#define AUTH_ID_NOBODY      0	//user that is unauthenticated
#define AUTH_ID_USER        2	//normal user (not allow to change setting)
#define AUTH_ID_GUEST       3	//user only can see the public file 
#define AUTH_ID_ADMIN       7	//maximum rights for administration

//only who has auth_id greater or equal than this value can edit CSID
#define AUTH_ID_EDIT_CSID   4

