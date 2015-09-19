import os
import urllib
import ConfigParser
import re
config=ConfigParser.ConfigParser()  
with open("yixun.cfg") as cfgfile:
    config.readfp(cfgfile)
acc=config.get('info','acc')
pwd=config.get('info','pwd')
acc=re.sub('\n','',acc)
if acc.find("@ca")<=0:
    acc+='@ca'
p=os.popen('yixun.exe '+acc)
x=p.read()
p.close()
print acc
acc=urllib.quote_plus(x)
site = "http://admin:admin@192.168.1.1/userRpm/PPPoECfgRpm.htm?wan=0&wantype=1&acc=%0D%0A"+acc+"&psw="+pwd+"&confirm=208841&specialDial=100&SecType=0&sta_ip=0.0.0.0&sta_mask=0.0.0.0&linktype=1&Save=%B1%A3+%B4%E6"
urllib.urlopen(site)
os.system('pause')
