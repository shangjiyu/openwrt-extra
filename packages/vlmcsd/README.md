vlmcsd
======

KMS Emulator in C for openwrt.

http://forums.mydigitallife.info/threads/50234-Emulated-KMS-Servers-on-non-Windows-platforms

##Windows 激活命令：

CD “%SystemRoot%\SYSTEM32″

CSCRIPT /NOLOGO SLMGR.VBS /SKMS 192.168.0.xxx

CSCRIPT /NOLOGO SLMGR.VBS /ATO

CSCRIPT /NOLOGO SLMGR.VBS /XPR

##Office/Project/Visio 2013(2010换下安装路径) 激活命令：

32位：CD “%ProgramFiles(x86)%\MICROSOFT OFFICE\OFFICE15″

64位：CD “%ProgramFiles%\MICROSOFT OFFICE\OFFICE15″

CSCRIPT OSPP.VBS /SETHST:192.168.0.xxx

CSCRIPT OSPP.VBS /ACT

CSCRIPT OSPP.VBS /DSTATUS