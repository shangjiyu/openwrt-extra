include $(TOPDIR)/rules.mk

PKG_NAME:=phddns
PKG_VERSION:=2.0.6
PKG_RELEASE:=32828


PKG_SOURCE:=$(PKG_NAME)-sdk-$(PKG_VERSION).$(PKG_RELEASE).tar.gz
PKG_SOURCE_URL:=http://download.oray.com/peanuthull/
#http://download.oray.com/peanuthull/phddns-sdk-2.0.6.32828.tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION).$(PKG_RELEASE)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk


define Package/phddns
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Oray PHDDNS client for OpenWRT
  URL:=http://www.oray.com/
endef

define Package/phddns/Description
  Oray PHDDNS Client. Author:bywayboy@qq.com
endef


#[安装(编译)时执行的脚本 记得加上#!/bin/sh 没有就空着]
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/phddns $(1)/usr/sbin/phddns

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/rc.d
	$(INSTALL_BIN) ./files/etc/init.d/phddns $(1)/etc/init.d/phddns
	ln -s ../init.d/phddns $(1)/etc/rc.d/S92phddns 

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) ./files/etc/config/phddns $(1)/etc/config/phddns
	
endef


#[删除后执行的脚本 记得加上#!/bin/sh 没有就空着] 
define Package/$(PKG_NAME)/postrm
#!/bin/sh
rm -rf /usr/sbin/phddns /etc/init.d/phddns /etc/rc.d/S92phddns


endef

$(eval $(call BuildPackage,phddns))
