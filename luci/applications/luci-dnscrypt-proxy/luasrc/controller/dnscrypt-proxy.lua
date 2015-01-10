--[[
Luci configration for dns2sock.Made By shangjiyu
]]--

module("luci.controller.dnscrypt-proxy", package.seeall)

function index()
	
	if not nixio.fs.access("/etc/config/dnscrypt-proxy") then
		return
	end

	page = entry({"admin", "services", "dnscrypt-proxy"}, cbi("dnscrypt-proxy"), _("DNSCrypt-Proxy"), 55)
	page.i18n = "DNSCrypt-Proxy"
	page.dependent = true
end
