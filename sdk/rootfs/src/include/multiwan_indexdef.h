#define BASIC_WAN_NUM                   1
#define MAX_WAN_NUM                     16

#if defined(BDW462AM_L)

#define START_INDEX_ETHER               0
#define MAX_ETHER_WAN_NUM               8
#define START_INDEX_3G                  8
#define MAX_3G_WAN_NUM                  4

#else

#define START_INDEX_ETHER               0
#define MAX_ETHER_WAN_NUM               5
#define START_INDEX_ADSL		5	
#define MAX_ADSL_WAN_NUM 		8
#define START_INDEX_3G                  13
#define MAX_3G_WAN_NUM                  3

#endif

#define BASIC_WAN_INDEX			99
#define DEFAULT_POLICY			100

// Keep those index the same with glue/etc.rt_tables.
#define IP_RULE_PRIO_LAN		20
#define IP_RULE_PRIO_POLICY_WAN		29
#define IP_RULE_PRIO_POLICY_MULTIWAN	30
#define IP_RULE_PRIO_WAN		199
#define IP_RULE_PRIO_MULTIWAN		200
#define IP_RULE_PRIO_WANDEF		222
