include $(TOPDIR)/rules.mk

PKG_NAME:=deadwood
PKG_VERSION:=3.2.06
PKG_RELEASE:=$(shell date +%Y%m%d)

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_SUBDIR:=src
PKG_SOURCE_URL:=@SF/maradns/Deadwood/$(PKG_VERSION)
PKG_MD5SUM:=b3e0d0370cd49c5c7c2d3c14f2b82877

PKG_MAINTAINER:=Sam Trenholme <maradns@gmail.com>
PKG_LICENSE:=BSD-2-Clause
PKG_LICENSE_FILES:=Copying.txt

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_INSTALL:=1
PKG_USE_MIPS16:=0
PKG_BUILD_PARALLEL:=1

PKG_BUILD_DEPENDS:=argp-standalone

include $(INCLUDE_DIR)/package.mk

define Package/deadwood/Default
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=IP Addresses and Names
  TITLE:=A fully recursive caching DNS resolver
  URL:=http://maradns.samiam.org/deadwood/
  MAINTAINER:=Sam Trenholme <maradns@gmail.com>
  DEPENDS:=+librt
endef

define Package/deadwood
	$(call Package/deadwood/Default)
endef

define Package/deadwood/description
 Deadwood is a fully recursive caching DNS resolver.
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) \
	-C $(PKG_BUILD_DIR)/$(PKG_SOURCE_SUBDIR) \
	$(TARGET_CONFIGURE_OPTS) \
	CC="$(TARGET_CC)" \
	HOSTCC="$(HOSTCC)" \
	CFLAGS="$(TARGET_CFLAGS) $(FPIC) $(TARGET_CPPFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)"
	$(MAKE) \
	-C $(PKG_BUILD_DIR)/tools \
	$(TARGET_CONFIGURE_OPTS) \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS) $(FPIC) $(TARGET_CPPFLAGS)" \
	LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Build/Install
endef

define Package/deadwood/conffiles
/etc/deadwood/dwood3rc
/etc/deadwood/dwood3rc-all
endef

define Package/deadwood/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_SOURCE_SUBDIR)/Deadwood $(1)/usr/bin/deadwood
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/duende $(1)/usr/bin/duende
	$(INSTALL_DIR) $(1)/etc/deadwood
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/doc/dwood3rc $(1)/etc/deadwood/dwood3rc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/doc/dwood3rc-all $(1)/etc/deadwood/dwood3rc-all
endef

$(shell $(RM) $(DL_DIR)/$(PKG_SOURCE))

$(eval $(call BuildPackage,deadwood))
