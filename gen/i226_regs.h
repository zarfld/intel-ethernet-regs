#pragma once
/* Auto-generated from intel-ethernet-regs/devices/i226.yaml */

/* Base: MAC_CTRL */
#define I226_CTRL            0x00000
#define I226_STATUS          0x00008
#define I226_CTRL_EXT        0x00018

/* Base: MDIO */
#define I226_MDIC            0x00020

/* Base: INTERRUPTS */
#define I226_ICR             0x000C0
#define I226_ICS             0x000C8
#define I226_IMS             0x000D0
#define I226_IMC             0x000D8

/* Base: EXT_INTERRUPTS */
#define I226_EIMS            0x01524
#define I226_EIMC            0x01528
#define I226_EIAC            0x0152C
#define I226_EIAM            0x01530
#define I226_EICR            0x01580

/* Base: EITR */
#define I226_EITR0           0x01680
#define I226_EITR1           0x01684
#define I226_EITR2           0x01688
#define I226_EITR3           0x0168C

/* Base: MAC_ADDR */
#define I226_RAL0            0x05400
#define I226_RAH0            0x05404

/* Base: VLAN */
#define I226_VFTA0           0x05600

/* Base: LED */
#define I226_LEDCTL          0x00E00
