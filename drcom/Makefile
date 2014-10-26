include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
PKG_NAME:=drcom
PKG_VERSION:=zjut
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz?raw=true
PKG_SOURCE_URL:=https://github.com/shangjiyu/drcom/blob/master/
PKG_MD5SUM:=bfa9a38b32122056c6cbf7bf7b447bd6

PKG_INSTALL:=1
arch="$(ARCH)"
include $(INCLUDE_DIR)/package.mk

define Package/drcom
	SECTION:=net
	CATEGORY:=Network
#	DEPENDS:=+libpthread
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
	$(SED) 's/\/etc\/drcom.conf/\/tmp\/drcom.conf/g' $(PKG_BUILD_DIR)/Makefile
endef

define Package/drcom/conffiles
/etc/drcom.conf
endef

define Package/drcom/install
	$(INSTALL_DIR) $(1)/usr/bin  
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/drcom/drcom $(1)/usr/bin
	
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/drcom.conf $(1)/etc/
endef

$(eval $(call BuildPackage,drcom))
