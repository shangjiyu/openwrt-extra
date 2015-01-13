--[[
wdas -- wifidog auth server
mail: xzm2@qq.com
QQ: 529698939
]]--
module("luci.controller.wifidog.wdas", package.seeall)

local session = require "luci.wifidogsession"
local http = luci.http
local translate = luci.i18n.translate
local wdcfg = require("luci.wifidogconfig")

function index()
	local page    = node("wdas")
	page.target   = alias("wdas", "login")
	page.order    = 90
	page.i18n = "wifidogauth"
	page.setuser  = "nobody"
	page.setgroup = "nogroup"

	entry({"wdas", "login"}, call("login"))
	entry({"wdas", "logincheck"}, call("logincheck"))
	entry({"wdas", "auth"}, call("auth"))
	entry({"wdas", "ping"}, call("ping"))
	entry({"wdas", "portal"}, call("portal"))
	entry({"wdas", "gw_message"}, call("gw_message"))
end

function login()
	--login/?gw_id=&gw_address=&gw_port=&mac=&url=
	luci.template.render("wifidog/wdas_login")
end

function logincheck()
	local username, password = wdcfg.auth.username, wdcfg.auth.password
	local user, pwd, id, addr, port, mac, url, token
	user = http.formvalue("user")
	pwd = http.formvalue("pwd")
	id = http.formvalue("gw_id")
	addr = http.formvalue("gw_address")
	port = http.formvalue("gw_port")
	mac = http.formvalue("mac")
	url = http.formvalue("url")
	token = http.getcookie("wdastok")
	http.prepare_content("application/json")
	if addr and port and mac and (user == username) and (pwd == password) then
		token = token and token:match("^[a-f0-9]*$") or luci.sys.uniqueid(16)
		local sdt = {id=id, addr=addr, port=port, mac=mac, url=url, timestamp=luci.sys.uptime()}
		local path = (http.getenv("SCRIPT_NAME") or "") .. "/wdas"
		session.write(token, sdt)
		http.header("Set-Cookie", "wdastok=" .. token .. "; path=" .. path)
		http.write('{url:"http://' .. addr .. ':' .. port ..
			'/wifidog/auth?token=' .. token .. '"}')
	else
		if addr and port and mac then
			http.write('{error: "' .. translate("Invalid username or password.") .. '"}')
		else
			http.write('{error: "' .. translate("Invalid parameter.") .. '"}')
		end
	end
end

function auth()
	--auth/?stage=&ip=&mac=&token=&incoming=&outgoing=
	local stage, ip, mac, token, incoming, outgoing
	stage = http.formvalue("stage")
	ip = http.formvalue("ip")
	mac = http.formvalue("mac")
	token = http.formvalue("token")
	incoming = http.formvalue("incoming")
	outgoing = http.formvalue("outgoing")
	token = token and token:match("^[a-f0-9]*$")
	local sdt = token and session.read(token)
	if token and sdt and (mac == sdt.mac) then
		http.write("Auth: 1")
	else
		http.write("Auth: 0")
	end
end

function ping()
	--ping/?gw_id=&sys_load=&sys_memfree=&sys_load=&wifidog_uptime=
	local id, sys_uptime, sys_memfree, sys_load, wifidog_uptime
	id = http.formvalue("gw_id")
	sys_uptime = http.formvalue("sys_uptime")
	sys_memfree = http.formvalue("sys_memfree")
	sys_load = http.formvalue("sys_load")
	wifidog_uptime = http.formvalue("wifidog_uptime")
	if id and sys_uptime and sys_memfree and sys_load and wifidog_uptime then
		http.write("Pong")
	else
		http.write("{error:2}")
	end
end

function portal()
	--portal/?gw_id=%s
	local token, sdt, url
	token = http.getcookie("wdastok")
	sdt = token and session.read(token)
	url = sdt and sdt.url or "http://www.baidu.com"
	http.redirect(url)
end

function gw_message()
	local msg = http.formvalue("message")
	http.write(msg)
end
