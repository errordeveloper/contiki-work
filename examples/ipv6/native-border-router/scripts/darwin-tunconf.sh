# For details see `tunconf.sh`
ifconfig $SLIP_CONFIG_TUNDEV inet6 $SLIP_CONFIG_IPADDR up \
  && sysctl -w net.inet6.ip6.forwarding=1 \
  && ifconfig $SLIP_CONFIG_TUNDEV
