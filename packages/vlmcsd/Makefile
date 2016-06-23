#
# Copyright (C) 2015 wongsyrone 
#
# This is free software, licensed under the GNU General Public License v3.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=vlmcsd
PKG_VERSION:=svn934
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/vlmcsd
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=KMS activation tool from MDL forum
	URL:=http://forums.mydigitallife.info/threads/50234-Emulated-KMS-Servers-on-non-Windows-platforms
	DEPENDS:=+libpthread
endef

define Package/vlmcsd/description
	KMS activation tool from MDL forum, which helps you testing MS products.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	$(call Build/Prepare/Default)
endef

# AUXV=1          Use /proc/self/auxv (**requires Linux with glibc >= 2.16 or musl**.)
MAKE_FLAGS+= \
	VERBOSE=3 \
	$(if $(CONFIG_USE_MUSL)$(CONFIG_GLIBC_VERSION_2_21),AUXV=1)

#-D_PEDANTIC      Report rare error/warning conditions instead of silently ignoring them.
TARGET_CFLAGS+= \
	-D_PEDANTIC

TARGET_CFLAGS += -ffunction-sections -fdata-sections
TARGET_LDFLAGS += -Wl,--gc-sections

# Use Link time optimization
TARGET_CFLAGS += -flto
TARGET_LDFLAGS += -Wl,-flto

define Package/vlmcsd/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/vlmcsd.init $(1)/etc/init.d/vlmcsd
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./files/vlmcsd.ini $(1)/etc/vlmcsd.ini
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/vlmcsd.config $(1)/etc/config/vlmcsd
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/vlmcsd $(1)/usr/sbin/vlmcsd
endef

$(eval $(call BuildPackage,vlmcsd)) 
