--[[
luci configration for dns2sock.Made By shangjiyu
]]--

local fs = require "nixio.fs"

local running=(luci.sys.call("pidof dns2socks > /dev/null") == 0)
if running then	
	m = Map("dns2socks", translate("DNS2SOCKS"), translate("DNS2SOCKS is running"))
else
	m = Map("dns2socks", translate("DNS2SOCKS"), translate("DNS2SOCKS is not running"))
end

s = m:section(TypedSection, "dns2socks", "")
s.anonymous = true

switch = s:option(Flag, "enabled", translate("Enable"))
switch.rmempty = false

dns= s:option(Value, "dns_ip", translate("Upstream DNS Server"))
dns.optional = false
dns.default = "8.8.8.8"

dnsport = s:option(Value, "dns_port", translate("Upstream DNS Port"))
dnsport.datatype = "range(0,65535)"
dnsport.optional = false
dnsport.default ="53 "

socks5_ip = s:option(Value, "socks5_ip", translate("Socks5 Server"))
socks5_ip.optional = false
socks5_ip.default = "8.8.8.8"

socks5_port = s:option(Value, "socks5_port", translate("Socks5 Port"))
socks5_port.datatype = "range(0,65535)"
socks5_port.optional = false
socks5_port.default ="1081"

local_ip = s:option(Value, "local_ip", translate("Listenning Address"))
local_ip.optional = false
local_ip.default = "127.0.0.1"

local_port = s:option(Value, "local_port", translate("Listenning Port"))
local_port.datatype = "range(0,65535)"
local_port.optional = false
local_port.default ="5533"

return m
