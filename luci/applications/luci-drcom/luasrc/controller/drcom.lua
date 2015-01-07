--[[
ZJUT Drcom Luci configuration page. Made By Shangjiyu
]]--

module("luci.controller.drcom", package.seeall)

function index()
	
	if not nixio.fs.access("/etc/config/drcom") then
		return
	end

	local page
	page = entry({"admin", "services", "drcom"}, cbi("drcom"), _("Dr.COM"), 45)
	page.i18n = "Dr.COM"
	page.dependent = true
end
