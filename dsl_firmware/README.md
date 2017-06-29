DSL modem firmware upgrade procedure
====================================

1. Copy ras_PPPoA.dat onto the LR54.

2. Execute the 'update dsl' command:

digi.router> update dsl ras_PPPoA.dat

Updating DSL modem firmware using ras_PPPoA.dat; please wait...
DSL modem firmware update complete
digi.router>

3. Verify the firmware version:

digi.router> show dsl

 DSL Status and Statistics
 -------------------------
 Description      : 
 Admin Status     : Up
 Oper Status      : Down
 HW Version       : T14.F7_12.0
 FW Version       : 3.22.14.0_A60394
 System FW ID     : 3.6.20.0(Y09.ZZ.5)3.22.13.0 20151216_v035  [Dec 16 2015 16:59:11]

 Line Status      : Down

digi.router> 