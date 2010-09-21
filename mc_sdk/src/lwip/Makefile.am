libdir = @PS3SDK_LIBDIR@

CC = @PPU_CC@
AR = @PPU_AR@
RANLIB = @PPU_RANLIB@

LWIPINCDIR = $(srcdir)/src/include

INCLUDES = -I$(top_srcdir)/src/base -I$(top_srcdir)/src/kernel \
	-I$(LWIPINCDIR) -I$(LWIPINCDIR)/ipv4


LWIPDIR=$(srcdir)/src

# COREFILES, CORE4FILES: The minimum set of files needed for lwIP.
COREFILES=$(LWIPDIR)/core/mem.c $(LWIPDIR)/core/memp.c $(LWIPDIR)/core/netif.c \
	$(LWIPDIR)/core/pbuf.c $(LWIPDIR)/core/stats.c $(LWIPDIR)/core/sys.c \
        $(LWIPDIR)/core/tcp.c $(LWIPDIR)/core/tcp_in.c \
        $(LWIPDIR)/core/tcp_out.c $(LWIPDIR)/core/udp.c 
CORE4FILES=$(LWIPDIR)/core/ipv4/icmp.c $(LWIPDIR)/core/ipv4/ip.c \
	$(LWIPDIR)/core/inet.c $(LWIPDIR)/core/ipv4/ip_addr.c $(LWIPDIR)/core/ipv4/ip_frag.c

# APIFILES: The files which implement the sequential and socket APIs.
APIFILES=$(LWIPDIR)/api/api_lib.c $(LWIPDIR)/api/api_msg.c $(LWIPDIR)/api/tcpip.c \
	$(LWIPDIR)/api/err.c $(LWIPDIR)/api/sockets.c 

# NETIFFILES: Files implementing various generic network interface functions.'
NETIFFILES=$(LWIPDIR)/netif/loopif.c \
	$(LWIPDIR)/netif/etharp.c

# LWIPFILES: All the above.
LWIPFILES=$(COREFILES) $(CORE4FILES) $(APIFILES) $(NETIFFILES)


lib_LIBRARIES = liblwipv4.a

liblwipv4_a_SOURCES = $(LWIPFILES)

lwipoptsincludedir = @PS3SDK_INCLUDEDIR@
lwipoptsinclude_HEADERS = lwipopts.h

archincludedir = @PS3SDK_INCLUDEDIR@/arch
archinclude_HEADERS = $(LWIPINCDIR)/arch/cc.h $(LWIPINCDIR)/arch/perf.h

lwipincludedir = @PS3SDK_INCLUDEDIR@/lwip
lwipinclude_HEADERS = $(LWIPINCDIR)/lwip/api.h $(LWIPINCDIR)/lwip/api_msg.h \
	$(LWIPINCDIR)/lwip/arch.h $(LWIPINCDIR)/lwip/debug.h \
	$(LWIPINCDIR)/lwip/def.h $(LWIPINCDIR)/lwip/dhcp.h \
	$(LWIPINCDIR)/lwip/err.h $(LWIPINCDIR)/lwip/mem.h \
	$(LWIPINCDIR)/lwip/memp.h $(LWIPINCDIR)/lwip/netifapi.h \
	$(LWIPINCDIR)/lwip/netif.h $(LWIPINCDIR)/lwip/opt.h \
	$(LWIPINCDIR)/lwip/pbuf.h $(LWIPINCDIR)/lwip/raw.h \
	$(LWIPINCDIR)/lwip/sio.h $(LWIPINCDIR)/lwip/snmp_asn1.h \
	$(LWIPINCDIR)/lwip/snmp.h $(LWIPINCDIR)/lwip/snmp_msg.h \
	$(LWIPINCDIR)/lwip/snmp_structs.h $(LWIPINCDIR)/lwip/sockets.h \
	$(LWIPINCDIR)/lwip/stats.h $(LWIPINCDIR)/lwip/sys.h \
	$(LWIPINCDIR)/lwip/tcp.h $(LWIPINCDIR)/lwip/tcpip.h \
	$(LWIPINCDIR)/lwip/udp.h \
	$(LWIPINCDIR)/ipv4/lwip/autoip.h \
	$(LWIPINCDIR)/ipv4/lwip/icmp.h \
	$(LWIPINCDIR)/ipv4/lwip/igmp.h \
	$(LWIPINCDIR)/ipv4/lwip/inet.h \
	$(LWIPINCDIR)/ipv4/lwip/ip_addr.h \
	$(LWIPINCDIR)/ipv4/lwip/ip_frag.h \
	$(LWIPINCDIR)/ipv4/lwip/ip.h

netifincludedir = @PS3SDK_INCLUDEDIR@/netif
netifinclude_HEADERS = $(LWIPINCDIR)/netif/etharp.h \
	$(LWIPINCDIR)/netif/loopif.h $(LWIPINCDIR)/netif/slipif.h

noinst_HEADERS = $(LWIPINCDIR)/ipv6/lwip/icmp.h \
	$(LWIPINCDIR)/ipv6/lwip/inet.h \
	$(LWIPINCDIR)/ipv6/lwip/ip_addr.h \
	$(LWIPINCDIR)/ipv6/lwip/ip.h \
	$(LWIPDIR)/netif/ppp/auth.h $(LWIPDIR)/netif/ppp/md5.h \
	$(LWIPDIR)/netif/ppp/chap.h $(LWIPDIR)/netif/ppp/pap.h \
	$(LWIPDIR)/netif/ppp/chpms.h $(LWIPDIR)/netif/ppp/pppdebug.h \
	$(LWIPDIR)/netif/ppp/fsm.h $(LWIPDIR)/netif/ppp/ppp.h \
	$(LWIPDIR)/netif/ppp/ipcp.h $(LWIPDIR)/netif/ppp/randm.h \
	$(LWIPDIR)/netif/ppp/lcp.h $(LWIPDIR)/netif/ppp/vjbsdhdr.h \
	$(LWIPDIR)/netif/ppp/magic.h $(LWIPDIR)/netif/ppp/vj.h

EXTRA_DIST = CHANGELOG FILES doc/contrib.txt doc/FILES doc/rawapi.txt \
	doc/savannah.txt doc/snmp_agent.txt doc/sys_arch.txt \
	src/FILES src/netif/FILES \
	$(LWIPDIR)/netif/ethernetif.c $(LWIPDIR)/netif/slipif.c \
	$(LWIPDIR)/netif/ppp/auth.c $(LWIPDIR)/netif/ppp/magic.c \
	$(LWIPDIR)/netif/ppp/chap.c $(LWIPDIR)/netif/ppp/md5.c \
	$(LWIPDIR)/netif/ppp/chpms.c $(LWIPDIR)/netif/ppp/pap.c \
	$(LWIPDIR)/netif/ppp/fsm.c $(LWIPDIR)/netif/ppp/ppp.c \
	$(LWIPDIR)/netif/ppp/ipcp.c $(LWIPDIR)/netif/ppp/randm.c \
	$(LWIPDIR)/netif/ppp/lcp.c $(LWIPDIR)/netif/ppp/vj.c \
	$(LWIPDIR)/api/netifapi.c \
	$(LWIPDIR)/core/dhcp.c $(LWIPDIR)/core/inet6.c \
	$(LWIPDIR)/core/ipv4/autoip.c $(LWIPDIR)/core/ipv4/igmp.c \
	$(LWIPDIR)/core/ipv6/icmp6.c $(LWIPDIR)/core/ipv6/ip6_addr.c \
	$(LWIPDIR)/core/ipv6/ip6.c $(LWIPDIR)/core/ipv6/README \
	$(LWIPDIR)/core/raw.c \
	$(LWIPDIR)/core/snmp/asn1_dec.c $(LWIPDIR)/core/snmp/asn1_enc.c \
	$(LWIPDIR)/core/snmp/mib2.c $(LWIPDIR)/core/snmp/mib_structs.c \
	$(LWIPDIR)/core/snmp/msg_in.c $(LWIPDIR)/core/snmp/msg_out.c

