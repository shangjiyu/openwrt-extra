include $(TOPDIR)/rules.mk

PKG_NAME:=vlmcsd
PKG_VERSION:=svn796
PKG_REV:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

PKG_BUILD_DEPENDS:= +libpthread

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/vlmcsd
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Other Servers
  TITLE:=KMS server on OpenWRT
  URL:=https://github.com/bywayboy/openwrt-packages
  DEPENDS:= +libpthread 
endef

define Package/vlmcsd/description
 A KMS server for openwrt svn498
endef

#编译准备工作,复制源码到目标目录
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) -R ./src/* $(PKG_BUILD_DIR)/
endef

#[安装(编译)时执行的脚本 记得加上#!/bin/sh 没有就空着]
define Package/vlmcsd/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/vlmcsd $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/config
	$(CP) ./files/etc/vlmcsd.ini $(1)/etc/vlmcsd.ini
	$(CP) ./files/etc/config/vlmcsd $(1)/etc/config/vlmcsd
	
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/etc/init.d/vlmcsd $(1)/etc/init.d/vlmcsd
	
	$(INSTALL_DIR) $(1)/etc/rc.d
	ln -s ../init.d/vlmcsd $(1)/etc/rc.d/S93vlmcsd
endef

$(eval $(call BuildPackage,vlmcsd)) 
