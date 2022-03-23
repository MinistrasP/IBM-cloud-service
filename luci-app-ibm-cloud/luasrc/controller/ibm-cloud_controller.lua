module("luci.controller.ibm-cloud_controller", package.seeall)

function index()
	entry({"admin", "services", "ibm-cloud"}, cbi("ibm-cloud_model"), _("IBM Cloud Program"),105)
end
