--[[
LuCI - Lua Configuration Interface

Copyright 2011 Copyright 2011 flyzjhz <flyzjhz@gmail.com>


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

]]--

module("luci.controller.qosv4", package.seeall)

function index()

	if nixio.fs.access("/etc/config/qosv4") then
	local page 
	page = entry({"admin", "network", "qosv4"}, cbi("qosv4"), _("QOSv4"),55)
	page.i18n = "qosv4"
	page.dependent = true
	end

	entry({"admin", "network", "qosv4", "qosv4ip"}, cbi("qosv4ip"), nil).leaf = true

end
