#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /opt/ti/grace_3_10_00_82/packages;/opt/ti/msp430/MSPWare_2_20_00_19/driverlib/packages;/opt/ti/msp430/MSPWare_2_20_00_19/driverlib;/opt/ti/ccsv6/ccs_base;/home/flitjesdev/workspace_v6_1/LedClock/.config
override XDCROOT = /opt/ti/xdctools_3_31_00_24_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /opt/ti/grace_3_10_00_82/packages;/opt/ti/msp430/MSPWare_2_20_00_19/driverlib/packages;/opt/ti/msp430/MSPWare_2_20_00_19/driverlib;/opt/ti/ccsv6/ccs_base;/home/flitjesdev/workspace_v6_1/LedClock/.config;/opt/ti/xdctools_3_31_00_24_core/packages;..
HOSTOS = Linux
endif
