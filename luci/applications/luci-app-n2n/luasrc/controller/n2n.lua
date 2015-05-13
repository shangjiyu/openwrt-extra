--[[
Luci configration for n2n. Made By shangjiyu
]]--

module("luci.controller.n2n", package.seeall)

function index()
	
	if not nixio.fs.access("/etc/config/n2n") then
		return
	end

	page = entry({"admin", "services", "n2n"}, cbi("n2n"), _("N2N VPN"), 92)
	page.i18n = "N2N"
	page.dependent = true
end
