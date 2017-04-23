module("luci.controller.dnspod", package.seeall)

function index()
        entry({"admin", "service", "dnspod"}, cbi("dnspod"), _("动态DNSPOD"), 57)
end
