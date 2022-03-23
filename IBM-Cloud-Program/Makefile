include $(TOPDIR)/rules.mk

PKG_NAME:=IBM-Cloud-Program
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/IBM-Cloud-Program
	DEPENDS:=+libubus +libubox +libblobmsg-json +libibm-iot
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=My ubus service program with IBM Watson
endef

define Package/IBM-Cloud-Program/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/IBM-Cloud-Program $(1)/usr/bin/IBM-Cloud-Program
	$(INSTALL_BIN) ./files/ibmCloudProg.init $(1)/etc/init.d/ibmCloudProg
	$(INSTALL_CONF) ./files/ibmCloudProg.config $(1)/etc/config/ibmCloudProg
endef

$(eval $(call BuildPackage,IBM-Cloud-Program))
