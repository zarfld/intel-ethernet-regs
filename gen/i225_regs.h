#pragma once
/* Auto-generated from intel-ethernet-regs/devices/i225.yaml */

/* Base: MAC_CTRL */
#define I225_CTRL            0x00000
#define I225_STATUS          0x00008
#define I225_CTRL_EXT        0x00018

/* Base: MDIO */
#define I225_MDIC            0x00020

/* Base: INTERRUPTS */
#define I225_ICR             0x000C0
#define I225_ICS             0x000C8
#define I225_IMS             0x000D0
#define I225_IMC             0x000D8

/* Base: EXT_INTERRUPTS */
#define I225_EIMS            0x01524
#define I225_EIMC            0x01528
#define I225_EIAC            0x0152C
#define I225_EIAM            0x01530
#define I225_EICR            0x01580

/* Base: EITR */
#define I225_EITR0           0x01680
#define I225_EITR1           0x01684
#define I225_EITR2           0x01688
#define I225_EITR3           0x0168C

/* Base: MAC_ADDR */
#define I225_RAL0            0x05400
#define I225_RAH0            0x05404

/* Base: VLAN */
#define I225_VFTA0           0x05600

/* Base: LED */
#define I225_LEDCTL          0x00E00
