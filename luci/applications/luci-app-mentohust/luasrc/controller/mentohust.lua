--[[

LuCI mentohust
Author:tsl0922
Email:tsl0922@sina.com

]]--

module("luci.controller.mentohust", package.seeall)

function index()

	if nixio.fs.access("/etc/config/mentohust") then
	local page 
	page = entry({"admin", "services", "mentohust"}, cbi("mentohust"), _("MentoHUST"), 10)
	page.i18n = "mentohust"
	page.dependent = true
	end
end
