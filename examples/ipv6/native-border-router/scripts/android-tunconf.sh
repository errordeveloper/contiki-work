# Android OS does not provide the following commands:
# * `hostname` (which is actually not associate with an IP)
# * `sysctl` (though it could be accomplished see below)
#
# To toggle `sysctl` one can use `getprop/setprop` commands
# and define property triggers in `/init.rc` file, however
# it is porbably most like that you set these parameters
# yourself in `/init.rc` once and for all.

ip link set $SLIP_CONFIG_TUNDEV up \
  && ip addr add $SLIP_CONFIG_IPADDR dev $SLIP_CONFIG_TUNDEV \
  && ip addr show dev $SLIP_CONFIG_TUNDEV

# Another solution would be to replace this script with
# a service definition (in `/init.rc`) and just call
# `start border_router_tun0` from here (TODO: example).
