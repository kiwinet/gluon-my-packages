include $(TOPDIR)/rules.mk

PKG_NAME:=kiwi-additional-wifi-json-info
PKG_VERSION:=3.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

CMAKE_OPTIONS:=-DINSTALL_TESTS=OFF

define Package/kiwi-additional-wifi-json-info
	TITLE:=$(PKG_NAME)
	CATEGORY:=Network
	MAINTAINER:=t-8ch
	DEPENDS:=+libnl-tiny +respondd +libuci
endef

define Package/kiwi-additional-wifi-json-info/description
	Some additional wifi device information for the kiwi JSON file.
	This package should work with Gluon v2016.1.x (respondd)  
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/kiwi-additional-wifi-json-info/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/lib/respondd
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/libkiwi.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/gluon/respondd/wireless.so $(1)/usr/lib/respondd/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/gluon/respondd/nexthop.so $(1)/usr/lib/respondd/
endef
 
$(eval $(call BuildPackage,kiwi-additional-wifi-json-info))
