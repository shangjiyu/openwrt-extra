--Mr.Z<zenghuaguo@hotmail.com>
local NXFS = require "nixio.fs"
local SYS  = require "luci.sys"
local HTTP = require "luci.http"
local DISP = require "luci.dispatcher"

local m,s,o
local LUCI_VER,ADBYBY_VER,Status

LUCI_VER=SYS.exec("awk '/^Version/{print $2}' /usr/lib/opkg/info/luci-app-adbyby.control")

if SYS.call("[ -x /usr/share/adbyby/adbyby ]") == 0 then
	ADBYBY_VER = SYS.exec("/usr/share/adbyby/adbyby --version | awk '{print substr($3,1,3)}'")
else
	ADBYBY_VER = "???"
end

if SYS.call("pidof adbyby >/dev/null") == 0 then
	Status = "广告屏蔽大师 运行中"
else
	Status = "广告屏蔽大师 未运行"
end

m = Map("adbyby")
m.title	= translate(string.format([[</a><a href="javascript:alert('版本信息\n\nluci-app-adbyby\n\t版本：\t%s\n\nadbyby\n\t版本：\t%s')">广告屏蔽大师]],LUCI_VER,ADBYBY_VER))
m.description = translate(string.format("<strong><font color=\"0x008000\">广告屏蔽大师可以全面过滤各种横幅、弹窗、视频广告，同时阻止跟踪、隐私窃取及各种恶意网站。</font></strong><br />%s", Status))

s = m:section(TypedSection, "adbyby")
s.anonymous = true

--基本设置
s:tab("basic",  translate("基本设置"))

o = s:taboption("basic", Flag, "enable")
o.title = translate("启用")
o.default = 0
o.rmempty = false

o = s:taboption("basic", ListValue, "daemon")
o.title = translate("守护进程")
o:value("0", translate("禁用"))
o:value("1", translate("脚本监视"))
o:value("2", translate("内置监视"))
o.default = 2
o.rmempty = false

o = s:taboption("basic", Button, "proxy")
o.title = translate("透明代理")
if SYS.call("iptables-save | grep ADBYBY >/dev/null") == 0 then
	o.inputtitle = translate("点击关闭")
	o.inputstyle = "reset"
	o.write = function()
		SYS.call("/etc/init.d/adbyby del_rule")
		HTTP.redirect(DISP.build_url("admin", "services", "adbyby"))
	end
else
	o.inputtitle = translate("点击开启")
	o.inputstyle = "apply"
	o.write = function()
		SYS.call('[ -n "$(pgrep adbyby)" ] && /etc/init.d/adbyby add_rule')
		HTTP.redirect(DISP.build_url("admin", "services", "adbyby"))
	end
end

local DL = SYS.exec("head -1 /usr/share/adbyby/data/lazy.txt | awk -F' ' '{print $3,$4}'")
local DV = SYS.exec("head -1 /usr/share/adbyby/data/video.txt | awk -F' ' '{print $3,$4}'")
local NR = SYS.exec("grep -v '^!' /usr/share/adbyby/data/rules.txt | wc -l")
local NU = SYS.exec("cat /usr/share/adbyby/data/user.txt | wc -l")
local NW = SYS.exec("uci get adbyby.@adbyby[-1].domain 2>/dev/null | wc -l")

o = s:taboption("basic", Button, "restart")
o.title = translate("规则状态")
o.inputtitle = translate("重启更新")
o.description = translate(string.format("规则日期/条目：<br /><strong>Lazy规则：%s<br />Video规则：%s<br />第三方规则：%d条<br />自定义规则：%d条</strong>", DL, DV, math.abs(NR+NW-NU), NR))
o.inputstyle = "reload"
o.write = function()
	SYS.call("/etc/init.d/adbyby restart")
	HTTP.redirect(DISP.build_url("admin", "services", "adbyby"))
end

--高级设置
s:tab("advanced", translate("高级设置"))

o = s:taboption("advanced", Flag, "cron_mode")
o.title = translate("每天6点重启")
o.default = 0
o.rmempty = false

o = s:taboption("advanced", ListValue, "lan_mode")
o.title = translate("内网控制")
o:value("0", translate("禁用"))
o:value("1", translate("仅允许过滤列表内"))
o:value("2", translate("仅允许过滤列表外"))
o.default = 0
o.rmempty = false

o = s:taboption("advanced", DynamicList, "lan_ip")
o.title = translate("内网IP列表")
o.datatype = "ipaddr"
o.placeholder = "IP address | IP/Mask"
for i,v in ipairs(SYS.net.arptable()) do
	o:value(v["IP address"])
end
o:depends("lan_mode", 1)
o:depends("lan_mode", 2)

o = s:taboption("advanced", ListValue, "wan_mode")
o.title = translate("网站控制")
o:value("0", translate("禁用"))
o:value("1", translate("仅允许过滤列表内"))
o:value("2", translate("仅允许过滤列表外"))
o.default = 0
o.rmempty = false

o = s:taboption("advanced", DynamicList, "wan_ip")
o.title = translate("域名")
o.datatype = "host"
o.placeholder = "Hostname | IP address | IP/Mask"
o:depends("wan_mode", 1)
o:depends("wan_mode", 2)

o = s:taboption("advanced", DynamicList, "domain")
o.title = translate("域名白名单")
o.placeholder = "baidu.com|cn.bing.com|google.com"

o = s:taboption("advanced", DynamicList, "exrule")
o.title = translate("第三方规则")
o.placeholder = "[https|http|ftp]://[Hostname]/[File]"

local file = "/usr/share/adbyby/adhook.ini"
o = s:taboption("advanced", TextValue, "adhook")
o.title = translate("配置文件")
o.size = 30
o.rows = 15
o.wrap = "off"
o.cfgvalue = function(self, section)
	return NXFS.readfile(file) or ""
end
o.write = function(self, section, value)
	NXFS.writefile(file, value:gsub("\r\n", "\n"))
end

--自定义规则
s:tab("user", translate("自定义规则"))

local file = "/usr/share/adbyby/data/rules.txt"
o = s:taboption("user", TextValue, "rules")
o.description = translate("开头感叹号（!）的每一行被视为注释。")
o.rows = 20
o.wrap = "off"
o.cfgvalue = function(self, section)
	return NXFS.readfile(file) or ""
end
o.write = function(self, section, value)
	NXFS.writefile(file, value:gsub("\r\n", "\n"))
end

--技术支持
s:tab("help",  translate("技术支持"))

o = s:taboption("help", Button, "web")
o.title = translate("官方网站")
o.inputtitle = translate("访问")
o.inputstyle = "apply"
o.write = function()
	HTTP.redirect("http://www.adbyby.com")
end

return m
