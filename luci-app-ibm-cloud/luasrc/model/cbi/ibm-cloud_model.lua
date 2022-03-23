map = Map("ibmCloudProg")

section = map:section(NamedSection, "ibmcloud_conf", "configuration", "IBM Cloud Program")

flag = section:option(Flag, "enabled", "Enable", "Start service")

orgId = section:option(Value, "orgId", "Organization ID")
typeId = section:option(Value, "typeId", "Device Type")
deviceId = section:option(Value, "deviceId", "Device ID")
token = section:option(Value, "token", "Authentication token")

return map
