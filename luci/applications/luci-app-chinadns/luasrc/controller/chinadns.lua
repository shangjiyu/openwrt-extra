--[[
RA-MOD
]]--

module("luci.controller.chinadns", package.seeall)

function index()
	
	if not nixio.fs.access("/etc/config/chinadns") then
		return
	end

--[[
	local page
	page = node("admin", "RA-MOD")
	page.target = firstchild()
	page.title = _("RA-MOD")
	page.order  = 65
]]--

	page = entry({"admin", "services", "chinadns"}, cbi("chinadns"), _("ChinaDNS"), 55)
	page.i18n = "ChinaDNS"
	page.dependent = true
end
