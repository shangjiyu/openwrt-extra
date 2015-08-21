#
# Copyright (C) 2007-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=subversion
PKG_VERSION:=1.8.10
PKG_RELEASE:=4

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://ftp.ps.pl/pub/apache/subversion
#PKG_SHA1SUM:=7705819a0037c14fb32eef36f2e57a803217c689

PKG_FIXUP:=autoreconf
PKG_MACRO_PATHS:=build/ac-macros

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk


define Package/subversion/Default
  SECTION:=net
  CATEGORY:=Network
  TITLE:=A compelling replacement for CVS
  DEPENDS:=+zlib  +libaprutil +libsasl2 +libmagic +libsqlite3 +libopenssl
  URL:=http://subversion.apache.org/
  SUBMENU:=Version Control Systems
endef

define Package/subversion/Default/description
	Subversion is a free/open-source version control system. That is,
	Subversion manages files and directories, and the changes made to them,
	over time. This allows you to recover older versions of your data, or
	examine the history of how your data changed. In this regard, many
	people think of a version control system as a sort of time machine.
endef

define Package/subversion-libs
  $(call Package/subversion/Default)
  TITLE:=subversion libs
endef

define Package/subversion-libs/description
  $(call Package/subversion/Default/description)
  This package contains the subversion libraries.
endef

define Package/subversion-client
  $(call Package/subversion/Default)
  DEPENDS+=subversion-libs
  TITLE:=subversion client tools
endef

define Package/subversion-client/description
  $(call Package/subversion/Default/description)
  This package contains the subversion client tools.
endef

define Package/subversion-server
  $(call Package/subversion/Default)
  DEPENDS+=subversion-libs
  TITLE:=subversion server
endef

define Package/subversion-server/description
  $(call Package/subversion/Default/description)
  This package contains the subversion server.
endef

define Package/subversion-server/conffiles
#/opt/etc/config/subversion
endef

#EXTRA_CPPFLAGS+=$(ICONV_CPPFLAGS)
TARGET_CFLAGS += $(FPIC)
TARGET_CPPFLAGS += -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE

CONFIGURE_ARGS += \
	--enable-nls \
	--disable-mod-activation \
	--with-apr="$(BUILD_DIR)/apr-1.5.1/apr-1-config" \
	--with-apr-util="$(BUILD_DIR)/apr-util-1.5.4/apu-1-config" \
	--with-sqlite="$(STAGING_DIR)/usr" \
	--with-sasl \
	--without-serf \
	--without-ruby-sitedir \
	--without-swig \
	--without-jikes \
	--without-junit \
	--without-berkeley-db \
	--without-openssl \
	--without-apxs \


CONFIGURE_VARS += \
	LDFLAGS="$(TARGET_LDFLAGS) -lcrypt -lm -lz -lpthread -lsasl2 -lsqlite3 -L$(TOOLCHAIN_DIR)/opt/lib -L$(TOOLCHAIN_DIR)/lib" \
	CPPFLAGS="$(TARGET_CPPFLAGS)" \

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) DESTDIR="$(PKG_INSTALL_DIR)" all local-install
endef

define Package/subversion-libs/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libsvn_*.so.* $(1)/usr/lib/
endef

define Package/subversion-client/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/{svn,svnversion,svnsync} $(1)/usr/bin/
endef

define Package/subversion-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/svn{look,admin,dumpfilter,serve} $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/config
	$(CP) ./files/etc/config/subversion $(1)/etc/config/subversion

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/etc/init.d/subversion $(1)/etc/init.d/subversion

	$(INSTALL_DIR) $(1)/etc/rc.d
	ln -s ../init.d/subversion $(1)/etc/rc.d/S92subversion
endef


$(eval $(call BuildPackage,subversion-libs))
$(eval $(call BuildPackage,subversion-client))
$(eval $(call BuildPackage,subversion-server))
