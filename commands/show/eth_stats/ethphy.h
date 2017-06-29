#ifndef __ETHPHY_H_
#define __ETHPHY_H_ 
 
#define MACBIT(x)			(1UL << (x))
#define MACBITS(lo,hi)		(((1UL << ((hi) - (lo) + 1UL)) - 1UL) << (lo))


#define PHY_CON    				0x0000		// Control register
#define PHY_STAT  				0x0001		// Status register
#define PHY_ID1					0x0002		// Identification register 1
#define PHY_ID2					0x0003		// Identification register 2
#define PHY_AUTOADVERT			0x0004		// Auto-negotiation advertisement
#define PHY_AUTOPART			0x0005		// Auto-negotiation partner ability
#define PHY_AUTOEXPANS			0x0006		// Auto-negotiation expansion cap.
#define PHY_1000BASET_CTRL		0x0009		// 1000BASE-T control register 
#define PHY_1000BASET_STAT		0x000a		// 1000BASE-T status register
#define PHY_EXTCON    			0x0010		// Extended control register
#define PHY_QUICKPOLL			0x0011		// Quick-poll status register
#define PHY_10BASETOP			0x0012		// 10BASE-T operations register
#define PHY_EXTCON2    			0x0013		// Extended control 2 register
#define PHY_STATUSOUTPUT      	0x0012      // Status output register
/*
 *		Bit definitions for PHY_CON
 */
#define PHY_CON_COLTEST			MACBIT(7)		// Enable collision test signal
#define PHY_CON_FULLDUPLEX		MACBIT(8)		// Enable full-duplex mode
#define PHY_CON_RESTARTNEG 	MACBIT(9)		// Restart auto-negotiation
#define PHY_CON_ISOLATE			MACBIT(10)		// Isolate PHY from MII bus
#define PHY_CON_POWERDOWN		MACBIT(11)		// Power-down t#define MACBIT(x)			(1UL << (x))
#define PHY_CON_AUTONEG			MACBIT(12)		// Auto-negotiation enabled
#define PHY_CON_100MB   		MACBIT(13)		// Enable 100 MB/s operation
#define PHY_CON_LOOPBACK		MACBIT(14)		// Enable loopback mode
#define PHY_CON_RESET			MACBIT(15)		// Reset the PHY

/*
 *		Bit definitions for PHY_STAT (these are all read-only)
 */
#define PHY_STAT_EXTCAP			MACBIT(0)		// Extended register set is present
#define PHY_STAT_JABBER			MACBIT(1)		// Set if jabber condition detected
#define PHY_STAT_LINKUP    	MACBIT(2)		// Set if valid net link detected
#define PHY_STAT_AUTONEG		MACBIT(3)		// Can support auto-negotiation
#define PHY_STAT_REMOTEFAULT	MACBIT(4)		// Set if remote fault detected
#define PHY_STAT_AUTONEGDONE	MACBIT(5)		// Set when auto-negotiation done
#define PHY_STAT_PRESUPP		MACBIT(6)		// Can support preable suppression
#define PHY_STAT_10HALFDUP		MACBIT(11)		// Half-duplex supported at 10 Mb
#define PHY_STAT_10FULLDUP		MACBIT(12)		// Full-duplex supported at 10 Mb
#define PHY_STAT_100HALFDUP	MACBIT(13)		// Half-duplex supported at 100 Mb
#define PHY_STAT_100FULLDUP	MACBIT(14)		// Full-duplex supported at 100 Mb
#define PHY_STAT_100BASET4		MACBIT(15)		// 100 Base-T4 supported

/*
 *		Bit definitions for PHY_AUTOADVERT - Auto-negotiation advertisment
 */
#define PHY_AUTOADV_SELECTOR_BP		0			// Technology supported
#define PHY_AUTOADV_SELECTOR_MASK	MACBITS(0,4)	//
#define PHY_AUTOADV_SELECT_802_3	(1)			// Indicates using IEEE 802.3

#define PHY_AUTOADV_10HALFDUP	MACBIT(5)		// We support 10 Mb half-duplex
#define PHY_AUTOADV_10FULLDUP	MACBIT(6)		// We support 10 Mb full-duplex
#define PHY_AUTOADV_100HALFDUP	MACBIT(7)		// We support 100 Mb half-duplex
#define PHY_AUTOADV_100FULLDUP	MACBIT(8)		// We support 100 Mb full-duplex
#define PHY_AUTOADV_100BASET4 	MACBIT(9)		// We support 100Base-T4
#define PHY_AUTOADV_PAUSE 	 	MACBIT(10)		// We support pause ability
#define PHY_AUTOADV_REMFAULT	MACBIT(13)		// Set to indicate a remote fault
#define PHY_AUTOADV_NEXTPAGE	MACBIT(15)		// Supports next page function

/*
 *		Bit definitions for PHY_AUTOPART - Auto-negotiated partner ability
 */
#define PHY_AUTOPART_SELECTOR_BP	0        	// Technology supp'd by partner
#define PHY_AUTOPART_SELECTOR_MASK	MACBITS(0,4)	//

#define PHY_AUTOPART_10HALFDUP	MACBIT(5)		// Partner supports 10 Mb half-dup
#define PHY_AUTOPART_10FULLDUP	MACBIT(6)		// Partner supports 10 Mb full-dup
#define PHY_AUTOPART_100HALFDUP	MACBIT(7)		// Partner supports 100 Mb half-dup
#define PHY_AUTOPART_100FULLDUP	MACBIT(8)		// Partner supports 100 Mb full-dup
#define PHY_AUTOPART_100BASET4 	MACBIT(9)		// Partner supports 100Base-T4
#define PHY_AUTOPART_PAUSE	 	MACBIT(10)		// Partner supports pause ability
#define PHY_AUTOPART_REMFAULT	MACBIT(13)		// Partner dected fault
#define PHY_AUTOPART_NEXTPAGE	MACBIT(15)		// Partner supports next page

/*      Bit definitions for PHY_1000BASET_CTRL      */

#define   PHY_CTRL_1000HALF          MACBIT(8)
#define   PHY_CTRL_1000FULL          MACBIT(9)


/*      Bit definitions for PHY_1000BASET_STAT     */
#define   PHY_STAT_1000HALF          MACBIT(10)
#define   PHY_STAT_1000FULL          MACBIT(11)


#endif
