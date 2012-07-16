# This example script tears-down the interface upon exit of the main process
# of `native-border-router` program. It can use the same environment varibles
# as `tunconf.sh` and it would be enough to run just the first command for a
# basic development set-up without routing outgoing packets. For details see
# `tunconf.sh`.

ip link set $SLIP_CONFIG_TUNDEV down \
  && sysctl net.ipv6.conf.all.forwarding=0
