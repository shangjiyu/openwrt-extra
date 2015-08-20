
include $(TOPDIR)/rules.mk

PKG_NAME:=tengine
PKG_VERSION:=1.5.2
PKG_RELEASE:=1

PKG_SOURCE:=tengine-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://tengine.taobao.org/download/
PKG_MD5SUM:=800d7c8e9d197d77495219a542464776

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Web Servers/Proxies
  TITLE:=Tengine web server
  URL:=http://tengine.taobao.org/
  DEPENDS:=+libpcre +zlib +libpthread
  MENU:=1
endef

define Package/$(PKG_NAME)/description
 tengine is an HTTP and reverse proxy server, as well as a mail proxy server,
 written by Igor Sysoev.
endef

define Package/$(PKG_NAME)/config
  source "$(SOURCE)/Config.in"
endef

PKG_CONFIG_DEPENDS := \
	CONFIG_TENGINE_STUB_STATUS \
	CONFIG_TENGINE_FLV 

define Package/nginx/conffiles
/etc/nginx/
endef


ADDITIONAL_MODULES:=

ifeq ($(CONFIG_IPV6),y)
  ADDITIONAL_MODULES += --with-ipv6
endif
ifeq ($(CONFIG_TENGINE_STUB_STATUS),y)
  ADDITIONAL_MODULES += --with-http_stub_status_module
endif
ifeq ($(CONFIG_TENGINE_FLV),y)
  ADDITIONAL_MODULES += --with-http_flv_module=shared
endif
ifeq ($(CONFIG_TENGINE_SSL),y)
  ADDITIONAL_MODULES += --with-http_ssl_module
else
  ADDITIONAL_MODULES += --without-http-cache --without-http_ssl_module
endif
ifeq ($(CONFIG_TENGINE_DAV),y)
  ADDITIONAL_MODULES += --with-http_dav_module=shared
endif

ifeq ($(CONFIG_TENGINE_SUB_MODULE),y)
  ADDITIONAL_MODULES += --with-http_sub_module
endif

ifneq ($(CONFIG_TENGINE_HTTP_PROXY),y)
  ADDITIONAL_MODULES += --without-http_proxy_module
endif

define Build/Configure
	# TODO: fix --crossbuild
	$(TARGET_CC) --help
	
	(cd $(PKG_BUILD_DIR) ;\
		./configure \
			--crossbuild=Linux::$(ARCH) \
			--prefix=/usr \
			--conf-path=/etc/nginx/nginx.conf \
			--error-log-path=/var/log/nginx/error.log \
			--pid-path=/var/run/nginx.pid \
			--lock-path=/var/lock/nginx.lock \
			--http-log-path=/var/log/nginx/access.log \
			--http-client-body-temp-path=/var/lib/nginx/body \
			--http-proxy-temp-path=/var/lib/nginx/proxy \
			--http-fastcgi-temp-path=/var/lib/nginx/fastcgi \
			--with-cc="$(TARGET_CC)" \
			--with-cc-opt="$(TARGET_CPPFLAGS) $(TARGET_CFLAGS)" \
			--with-ld-opt="$(TARGET_LDFLAGS)" \
			--without-http_ssi_module  \
			--without-http_geo_module \
			--without-http_upstream_check_module\
			--without-http_upstream_least_conn_module \
			--without-http_upstream_session_sticky_module \
			--without-http_upstream_keepalive_module \
			--without-http_upstream_ip_hash_module \
			--without-http_upstream_consistent_hash_module \
			--without-http_stub_status_module \
			--without-syslog \
			--without-dso \
			--without-select_module \
			--with-poll_module \
			--without-http_empty_gif_module \
			--without-http_memcached_module \
			$(ADDITIONAL_MODULES) )
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/nginx $(1)/usr/sbin/
	
	$(INSTALL_DIR) $(1)/etc/nginx
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/etc/nginx/* $(1)/etc/nginx/
	
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/nginx.init $(1)/etc/init.d/nginx
	
	$(INSTALL_DIR) $(1)/etc/rc.d
	ln -s ../init.d/nginx $(1)/etc/rc.d/S50nginx
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
