--[[
luci configration for dns2sock.Made By shangjiyu
]]--

local fs = require "nixio.fs"

local running=(luci.sys.call("pidof dnscrypt-proxy > /dev/null") == 0)
if running then	
	m = Map("dnscrypt-proxy", translate("DNSCrypt-Proxy"), translate("DNSCrypt-Proxy is running"))
else
	m = Map("dnscrypt-proxy", translate("DNSCrypt-Proxy"), translate("DNSCrypt-Proxy is not running"))
end

s = m:section(TypedSection, "dnscrypt-proxy", "")
s.anonymous = true

upstream = s:option(Value, "local_address", translate("Listenning Address:Port"))
upstream.optional = false
upstream.default = "127.0.0.1:5300"

MaxActReq = s:option(Value, "max_active_requests", translate("Max Active Request"))
MaxActReq.optional = false
MaxActReq.datetype = "range(0,1000)"
MaxActReq.default = "64"

return m
