module("luci.controller.vlmcsd", package.seeall)

function index()
	if not nixio.fs.access("/etc/vlmcsd.ini") then
		return
	end
	local page
	page = entry({"admin", "network", "vlmcsd"}, cbi("vlmcsd"), _("VLMCSD"), 100)
	page.i18n = "vlmcsd"
	page.dependent = true
end
