# For details see `tunconf.sh`
ifconfig $SLIP_CONFIG_TUNDEV down \
  && sysctl -w net.inet6.ip6.forwarding=0
