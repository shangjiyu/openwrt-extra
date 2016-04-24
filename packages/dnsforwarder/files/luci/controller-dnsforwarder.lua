--[[
LuCI - Lua Configuration Interface

Copyright (C) 2014-2015 MATTHEW728960

This is free software, licensed under the GNU General Public License v2.
See /LICENSE for more information.

$Id$
]]--

module("luci.controller.dnsforwarder", package.seeall)

function index()
	if not nixio.fs.access("/etc/config/dnsforwarder") then
		return
	end

	local page

	entry({"admin", "services", "dnsforwarder"}, cbi("dnsforwarder"), _("dnsforwarder"), 3).dependent = true

end
