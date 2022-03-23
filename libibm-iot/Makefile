include $(TOPDIR)/rules.mk

PKG_NAME:=libibm-iot
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/ibm-watson-iot/iot-c.git
PKG_SOURCE_VERSION:=f921dce616af815da2236954c608a2f377c928c5

include $(INCLUDE_DIR)/package.mk

define Package/libibm-iot
	CATEGORY:=Libraries
	TITLE:=libibm-iot
	DEPENDS:=+libopenssl
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-device.so $(1)/usr/lib/libibm-iot.so
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/wiotp/sdk/*.h $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so.1 $(1)/usr/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/paho.mqtt.c/src/*.h $(1)/usr/include

endef

define Package/libibm-iot/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-device.so $(1)/usr/lib/libibm-iot.so
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so.1 $(1)/usr/lib
endef

$(eval $(call BuildPackage,libibm-iot,+libopenssl))