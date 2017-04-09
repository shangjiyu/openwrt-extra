#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=brcmfmac-firmware-4366c0-pcie
PKG_RELEASE:=1

PKG_SOURCE_DATE:=2017-04-08

PKG_MAINTAINER:=shangjiyu <shangjiyu@gmail.com>


include $(INCLUDE_DIR)/package.mk

define Package/brcmfmac-firmware-4366c0-pcie 
  SECTION:=firmware
  CATEGORY:=Firmware
  URL:=http://git.kernel.org/cgit/linux/kernel/git/firmware/linux-firmware.git
  TITLE:=Broadcom 4366c0 FullMAC PCIe firmware
endef

define Build/Compile

endef

define Package/brcmfmac-firmware-4366c0-pcie/install
	$(INSTALL_DIR) $(1)/lib/firmware/brcm
	$(INSTALL_DATA) \
		./files/brcmfmac4366c-pcie.bin \
		$(1)/lib/firmware/brcm/
endef

$(eval $(call BuildPackage,brcmfmac-firmware-4366c0-pcie))
