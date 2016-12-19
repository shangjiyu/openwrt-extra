require("nixio.fs")
module("luci.controller.vlmcsd", package.seeall)

function index()
    entry({"admin", "services", "vlmcsd"}, cbi("vlmcsd"), _("KMS Server"), 2)
end
