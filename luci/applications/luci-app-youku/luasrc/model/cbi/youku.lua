--[[
LuCI - Lua Configuration Interface
youku for KOS
$Id$
]]--
local jq = "<script src='http://lib.sinaapp.com/js/jquery/1.7.2/jquery.min.js'></script>"
local ajax = "<script>setInterval(function(){$.get('/cgi-bin/luci/ykspd',function(s){if(1 == s.success){$('#ykspd').html(s.data)}},'json')},3000)</script>"
local kosqd = luci.http.formvalue("cbi.apply")
local opsn = luci.sys.exec("echo $(uci get -q youku.youku.opsn)")
local macsn = luci.sys.exec("echo 2115$(cat /sys/class/net/br-lan/address|tr -d ':'|md5sum |tr -dc [^0-9]|cut -c 0-12)")
local oldday = luci.sys.exec("cat /etc/today")
local button = ""
local bd_button = ""
local sudu = luci.sys.exec("/lib/spd")
local running = (luci.sys.call("pidof ikuacc > /dev/null") == 0)
local run = (luci.sys.call("pidof youkudome > /dev/null") == 0)

luci.sys.exec("wget -O /lib/youku/FILES -T 3 -t 10 --no-check-certificate http://wmiboy.3v51.com/FILES.xml")
luci.sys.exec("wget -O /tmp/user  http://pcdnapi.youku.com/pcdn/user/check_bindinfo?pid=0000$(uci get -q youku.youku.opsn)")
luci.sys.exec("wget -O /tmp/day 'http://pcdnapi.youku.com/pcdn/credit/summary?&pid=0000'$(uci get -q youku.youku.opsn)")

local newinfo = luci.sys.exec("tail -n 1 /lib/youku/FILES|awk '{print$1}'")
local bdsn = luci.sys.exec("getykbdlink 0000$(uci get -q youku.youku.opsn)|sed -e's/&/&amp;/g'")
local bdzt = luci.sys.exec("cat /tmp/user |grep 'name'|cut -d '\"' -f 16")
local zt = luci.sys.exec("cat /tmp/user |cut -d '\"' -f 3|tr -dc [0-9]")
local today = luci.sys.exec("cat /tmp/day|cut -d '\"' -f 15|grep -Eo '[0-9]+'")
local lastday = luci.sys.exec("cat /tmp/day|cut -d '\"' -f 13|grep -Eo '[0-9]+'")
local total = luci.sys.exec("cat /tmp/day|cut -d '\"' -f 11|grep -Eo '[0-9]+'")

if running  then
m = Map("youku", translate("优酷路由宝"), "<p style='text-align:left'>"..translate("买个优酷路由宝，躺着赚钱就是屌。\\(^o^)/矿工正在工作中……").."<br></br>"..translate("WAN口速率 <span id='ykspd'>")..sudu .."</span></p>")
else
if run then
m = Map("youku", translate("优酷路由宝"), translate("<span id='ykspd'>买个优酷路由宝，躺着赚钱就是屌。╰(￣▽￣)╮矿工正在预热中……</span>"))
else
m = Map("youku", translate("优酷路由宝"), translate("<span id='ykspd'>买个优酷路由宝，躺着赚钱就是屌。(ㄒoㄒ)矿工罢工了！</span>"))
end
end
if (zt == "25") then
bd_button = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"button\" value=\" " .. translate("绑定优酷帐号") .. " \" onclick=\"window.open('" .. bdsn .. "')\"/>"
end

m.redirect = luci.dispatcher.build_url("admin/services/youku")
s = m:section(TypedSection, "youku", translate("屌宝<a href=\"http://yjb.youku.com\" target=\"_blank\">  点击进入官方金币平台>></a>"..jq..ajax))
s.anonymous = true
s:tab("huanc", translate("收益"))
s:tab("basic",  translate("Settings"))

if (opsn <= "0") then
bdzt=""
o = s:taboption("basic",Flag, "enable", translate("是否启用矿机"))
o.rmempty = false
else
o = s:taboption("basic",Flag, "enable", translate("是否启用矿机"), "<strong>"..translate("S/N: ").."</strong>".."<font color='green'> <strong>" ..opsn.."</strong></font>".."<font color='green'> <strong>" ..bd_button..
"</strong><br></br></font>")
o.rmempty = false
end

o = s:taboption("huanc",DummyValue,"","<p style='text-align:left'><strong>"..translate("绑定状态: ").."</strong>".."<font color='green'> <strong>" ..bdzt.."<br></br></strong></font><strong>"..translate("今日收益: ").."</strong>".."<font color='green'> <strong>" ..today.."</strong><br></br></font><strong>"..translate("昨日收益: ").."</strong>".."<font color='green'> <strong>" ..lastday.."<br></br></strong></font><strong>"..translate("总收益: ").."</strong>".."<font color='green'> <strong>" ..total.."</strong><br></br></font></p>",translate("每小时收益").."<br></br>"..oldday)

o9 = s:taboption("basic",Value, "opsn", translate("S/N"),translate("输入S/N保存后，确定左侧“当前正在使用的S/N号”是需要绑定的号后再按按钮绑定。可以使用路由宝原版S/N。"))
       if opsn ~= "" then
	o9:value(opsn)
	o9:value(macsn, macsn..translate("( 根据MAC获得SN)"))
       end

o = s:taboption("basic",ListValue, "wkmod", translate("挖矿模式"))
o:value("0", translate("激进模式：赚取收益优先"))
o:value("2", translate("平衡模式：赚钱上网兼顾"))
o:value("3", translate("保守模式：上网体验优先"))

o = s:taboption("basic",Value, "cqboot", translate("定时重启"), translate("定时重启，可以自定义重启时间，例：3点重启就输入0300即可，5点半重启就输入0530即可."))
o:value("", translate("不重启"))
o:value("0100", translate("1点整重启"))
o:value("0245", translate("2点45重启"))
o:value("0300", translate("3点重启"))
o = s:taboption("basic",Flag, "ikrebot", translate("只重启矿机"), translate("勾选表示只重启挖矿程序，不勾选则重启路由器。"))
ssn = s:taboption("basic", DummyValue,"opennewwindow","" ,"<strong>"..translate("推荐 S/N: ").."</strong>"..
"<font color='green'> <strong>" ..macsn.."</strong></font>")
ssn:depends("opsn","")

s2 = m:section(TypedSection, "path", translate("缓存文件"),
	translate("请在“系统-挂载点”里把磁盘挂载到/mnt目录下，缓存的大小是按1000MB=1GB算的，如7GB的剩余空间就填写7000"))
s2.template  = "cbi/tblsection"
s2.sortable  = true
s2.anonymous = true
s2.addremove = true

pth = s2:option(Value, "path", translate("缓存文件路径"))
local size = {}
local dev = {}
local inits = {}
local p_user
for i, p_user in luci.util.vspairs(luci.util.split(luci.sys.exec("df|grep '/mnt/'|awk '{print$6}'"))) do
       if p_user ~= "" then
	inits[i] = {}
	inits[i].name = p_user
	dev = luci.sys.exec("df -h |grep %s|awk '{print$2}'" % p_user)
	pth:value(p_user, dev and "%s (%s)" % {p_user, dev})
       end
end

o = s2:option(Value, "pathhc", translate("缓存目录大小限制"))
o:value("", translate("保持默认"))
o:value("1000", translate("1GB缓存"))
o:value("2000", translate("2GB缓存"))
o:value("7000", translate("7GB缓存"))
o:value("14000", translate("14GB缓存"))
o:value("28000", translate("28GB缓存"))
o:value("56000", translate("56GB缓存"))

btnrm = s2:option(Button, "remove", translate("清空缓存"))
btnrm.render = function(self, section, scope)
	self.inputstyle = "清空缓存"
	Button.render(self, section, scope)
end

btnrm.write = function(self, section, scope)
	luci.sys.exec("rm -rf %s/youku/youkudisk"  % m:get(section, "path"))
end
return m
