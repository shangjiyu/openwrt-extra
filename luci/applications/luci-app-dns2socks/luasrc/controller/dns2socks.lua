--[[
Luci configration for dns2sock.Made By shangjiyu
]]--

module("luci.controller.dns2socks", package.seeall)

function index()
	
	if not nixio.fs.access("/etc/config/dns2socks") then
		return
	end

	page = entry({"admin", "services", "dns2socks"}, cbi("dns2socks"), _("DNS2SOCKS"), 55)
	page.i18n = "DNS2SOCKS"
	page.dependent = true
end
