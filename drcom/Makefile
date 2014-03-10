include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
PKG_NAME:=drcom
PKG_VERSION:=1.4.8.4
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz?raw=true
PKG_SOURCE_URL:=https://github.com/shangjiyu/drcom/blob/master/
PKG_MD5SUM:=0c8cbb4970facc60290f4e2d57d52637

PKG_INSTALL:=1
arch="$(ARCH)"
include $(INCLUDE_DIR)/package.mk

define Package/drcom
	SECTION:=net
	CATEGORY:=Network
	DEPENDS:=+libpthread
	TITLE:=An CERNET client daemon
	URL:=https://github.com/shangjiyu/drcom/
	SUBMENU:=CERNET
endef

define Package/drcom/description
An CERNET client daemon
Most usually used in China collages
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	$(SED) 's/modprobe/insmod/g' $(PKG_BUILD_DIR)/drcomd/drcomd.c
	$(SED) 's/r = system(s)/r = ""/g' $(PKG_BUILD_DIR)/drcomd/drcomd.c
	$(SED) 's/CC\ =\ gcc/CONFIG\ +=\ static\nCC\ =\ gcc/g' $(PKG_BUILD_DIR)/drcomd/drcomd.c
	$(SED) 's/\/etc\/drcom.conf/\/tmp\/drcom.conf/g' $(PKG_BUILD_DIR)/Makefile
	$(SED) 's,^KERNELDIR ?.*,KERNELDIR ?= $(LINUX_DIR),' $(PKG_BUILD_DIR)/kmod/Makefile
	$(SED) 's/.*\modules_install/#/g' $(PKG_BUILD_DIR)/kmod/Makefile
	$(SED) 's/.*\depmod/#/g' $(PKG_BUILD_DIR)/kmod/Makefile
endef

define Package/drcom/conffiles
/etc/drcom.conf
endef

define Package/drcom/install
	$(INSTALL_DIR) $(1)/usr/bin  
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/drcomc/drcomc $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/drcomd/drcomd $(1)/usr/bin
	
	$(INSTALL_DIR) $(1)/lib/modules/$(LINUX_VERSION)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/kmod/drcom.ko $(1)/lib/modules/$(LINUX_VERSION)/

	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/drcom.conf $(1)/etc/
endef

$(eval $(call BuildPackage,drcom))
