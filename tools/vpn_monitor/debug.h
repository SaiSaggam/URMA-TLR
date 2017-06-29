#ifndef __DEBUG_H__
#define __DEBUG_H__

#if !defined(VPN_MONITOR_DEBUG)
#define VPN_MONITOR_DEBUG 1
#endif

#if VPN_MONITOR_DEBUG
#define DEBUG(x...)		do { printf("vpn_monitor: %s:%d %s: ", __FILE__, __LINE__, __FUNCTION__); printf(x); printf("\n"); } while (0)
#else
#define DEBUG(x...)		do { } while (0)
#endif

#endif /* __DEBUG_H__ */
