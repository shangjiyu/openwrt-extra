module("luci.controller.cdns", package.seeall)
function index()
	if not nixio.fs.access("/etc/config/cdns") then
		return
	end
	entry({"admin", "services", "cdns"}, cbi("cdns"), _("CureDNS"), 72).dependent = true
end
