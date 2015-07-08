--[[
LuCI - Lua Configuration Interface - aria2 support

Copyright 2014 nanpuyue <nanpuyue@gmail.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0
]]--

module("luci.controller.ngrokc", package.seeall)

function index()
	if not nixio.fs.access("/etc/config/ngrokc") then
		return
	end

	local page = entry({"admin", "services", "ngrokc"}, cbi("ngrokc"), _("Ngrok Settings"))
	page.dependent = true
	
	entry({"admin", "services", "ngrokc", "status"}, call("ngrokc_status")).leaf = true
	
end

function ngrokc_status()
	ngrokcstat=luci.sys.exec("ngrokc_status")
	luci.http.prepare_content("application/json")
	luci.http.write(ngrokcstat)
end