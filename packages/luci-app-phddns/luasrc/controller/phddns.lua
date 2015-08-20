--[[
LuCI - Lua Configuration Interface

Copyright 2014 bywayboy <bywayboy@qq.com>

]]--

module("luci.controller.phddns", package.seeall)

function index()
	
	local page
                   
	if nixio.fs.access("/etc/config/phddns") then
		page = entry({"admin", "services", "phddns"}, cbi("admin_services/phddns"), _("Oray DDNS"))
		page.dependent = true
		entry({"admin", "services", "phddns_status"}, call("action_phddns_status"))
	end

	if nixio.fs.access("/etc/config/subversion") then
		page = entry({"admin", "services", "subversion"},cbi("admin_services/subversion"), _("Subversion"))
		page.dependent = true
	end


	if nixio.fs.access("/etc/config/vlmcsd") then
		page = entry({"admin", "services", "vlmcsd"},cbi("admin_services/vlmcsd"), _("KMS Server"))
		page.dependent = true
	end
end


function action_phddns_status()
	local file="/tmp/phddns.stat"
	local _ = luci.i18n.translate
	local stat="Not Running"
	local domain = ""
	
	function string_split(s, delimiter)
		result = {};
		for match in (s..delimiter):gmatch("(.-)"..delimiter) do
			table.insert(result, match);
		end
		return result;
	end
	local m
	luci.http.prepare_content("application/json")
	if nixio.fs.access(file) then
		local str = nixio.fs.readfile(file)
		m = string_split(str,"|")
		if #m > 0 then stat= m[1] end
		if #m > 1 then domain= m[2] end
	end
	local arr = { ["stat"]=_(stat), ["domain"]=domain }
	luci.http.write_json(arr);	
end