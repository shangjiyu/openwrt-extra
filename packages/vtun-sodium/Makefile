#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=vtun-sodium
PKG_VERSION:=4.0.0
PKG_RELEASE:=1
PKG_REV:=91f50be9170e043857e402b26af53c32e665d8de

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/jedisct1/vtun
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.bz2
PKG_MAINTAINER:=Frank Denis <github@pureftpd.org>

PKG_INSTALL:=1
PKG_FIXUP:=autoreconf
PKG_BUILD_PARALLEL:=1
PKG_USE_MIPS16:=0

include $(INCLUDE_DIR)/package.mk

define Package/vtun-sodium
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=@BROKEN +kmod-tun +VTUN_LZO:liblzo +VTUN_LZO:zlib +libsodium
  TITLE:=VPN tunneling daemon
  URL:=https://github.com/jedisct1/vtun
  SUBMENU:=VPN
  MENU:=1
endef

define Package/vtun-sodium/config
menu "Configuration"
        depends on PACKAGE_vtun-sodium

config VTUN_config
	bool

	depends on PACKAGE_vtun-sodium
	config VTUN_LZO
		bool "enable vtun-sodium compression"
		help
			vtun-sodium tunnels will be compressed
		default y

	config VTUN_SODIUM
		bool "enable vtun-sodium sodium encryption"
		help
			vtun-sodium tunnels will be encrypted
		default y
endmenu
endef

define Package/vtun-sodium/conffiles
/etc/vtun.conf
endef

CONFIGURE_ARGS += \
	$(if $(CONFIG_VTUN_LZO),--with-lzo-lib="$(STAGING_DIR)/usr/lib/",--disable-zlib) \
	$(if $(CONFIG_VTUN_LZO),--with-lzo-headers="$(STAGING_DIR)/usr/include/lzo",--disable-lzo) \
	$(if $(CONFIG_VTUN_SODIUM),--with-sodium-headers="$(STAGING_DIR)/usr/include",--disable-sodium) \

CONFIGURE_VARS += \
	LIBS="$(TARGET_LDFLAGS)" \
	$(if $(CONFIG_VTUN_LZO),LZO_HDR_DIR="$(STAGING_DIR)/usr/include/lzo") \
	$(if $(CONFIG_VTUN_SODIUM),SODIUM_HDR_DIR="$(STAGING_DIR)/usr/include")

#TARGET_CFLAGS += \
#	$(if $(CONFIG_VTUN_SODIUM), -fstack-protector-all -fno-strict-overflow -fno-strict-aliasing,) \
#TARGET_CPPFLAGS += \
#	$(if $(CONFIG_VTUN_SODIUM), -D_FORTIFY_SOURCE=2,)
	

define Build/Compile
	$(call Build/Compile/Default,vtun)
endef

define Package/vtun-sodium/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/vtun.conf $(1)/etc/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/vtun $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,vtun-sodium))
