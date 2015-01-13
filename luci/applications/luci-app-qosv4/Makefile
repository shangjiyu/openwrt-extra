#
# Copyright (C) 2008-2014 The LuCI Team <luci@lists.subsignal.org>
#
# This is free software, licensed under the Apache License, Version 2.0 .
#

include $(TOPDIR)/rules.mk

LUCI_TITLE:=LuCI Support for QoSv4
LUCI_DEPENDS:=+tc +kmod-sched +iptables +iptables-mod-filter +iptables-mod-ipopt +iptables-mod-conntrack-extra +iptables-mod-imq +iptables-mod-hashlimit +iptables-mod-connlimit

include ../../luci.mk

# call BuildPackage - OpenWrt buildroot signature
