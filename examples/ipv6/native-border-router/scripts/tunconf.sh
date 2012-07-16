# This example use `iproute2` package, which is available in most moder Linux
# distributions and make older `net-tools` package somewhat obsolete [1].
#  [1]: http://www.linuxfoundation.org/collaborate/workgroups/networking/net-tools
#
# The main process of `native-border-router` program calls this shell script
# to set-up the tunnel interface with the following environment variables set:
#  * `SLIP_CONFIG_TUNDEV` - name of tunnel device (defualt: `tun0`)
#  * `SLIP_CONFIG_IPADDR` - IPv6 address prefix (example: `aaaa::1/64`)
#
# _NOTE_: You should expect this script being _sourced_ by `/bin/sh` and not
#         any other shell, such as `/bin/bash`. Support for alternative shells
#         or interpreters is currently not considered necessary, thought it
#         can be accomplished by calling it form here, e.g. `exec ./example.py`.
#
# This example script sets the interface (`$SLIP_CONFIG_TUNDEV`) _"up"_ and sets
# it's IPv6 (`$SLIP_CONFIG_IPADDR`) and IPv4 addresses (using `hostname`
# command in attempt to get the address of primary network interface of the
# host). It also sets IPv6 packet forwarding on all interfaces and outputs the
# resulting configuration for the user to observe.

ip link set $SLIP_CONFIG_TUNDEV up \
  && sysctl net.ipv6.conf.all.forwarding=1
  && ip addr add $SLIP_CONFIG_IPADDR dev $SLIP_CONFIG_TUNDEV \
  && ip addr add `hostname` dev $SLIP_CONFIG_TUNDEV \
  && ip addr show dev $SLIP_CONFIG_TUNDEV

# Below is a minimal version of this script which should still provide basic
# functionality for communicating with Contiki nodes.
#
# ip link set $SLIP_CONFIG_TUNDEV up \
#   && ip add $SLIP_CONFIG_IPADDR dev $SLIP_CONFIG_IPADDR
