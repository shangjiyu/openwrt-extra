--[[
Luci configration for dns2sock.Made By shangjiyu
]]--

module("luci.controller.adbyby", package.seeall)

function index()
	
	if not nixio.fs.access("/etc/config/adbyby") then
		return
	end

	local page
	page = entry({"admin", "services", "adbyby"}, cbi("adbyby"), _("ADBYBY"), 38)
	page.dependent = true
end
