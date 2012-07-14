#ifndef HOSTIP
#  define HOSTIP "`hostname`"
#endif

#define LINUX_IPROUTE2_TUNCONF_SCRIPT(d, a, h) \
  "ip link set %s up \\\n" \
  "  && ip addr add %s dev %s \\\n" \
  "  && ip addr add %s dev %s \\\n" \
  "  && sysctl -w net.ipv6.conf.all.forwarding=1 \\\n" \
  "  && ip addr show dev %s\n" \
  , d, h, d, a, d, d

#define LINUX_IPROUTE2_TUNDOWN_SCRIPT(d) \
  "ip link set %s down \\\n" \
  "  && sysctl -w net.ipv6.conf.all.forwarding=0 \\\n" \
  , d

#define LINUX_LEGACY_TUNCONF_SCRIPT(d, a, h) \
  "ifconfig %s inet %s up \\\n" \
  "  && ifconfig %s add %s \\\n" \
  "  && sysctl -w net.ipv6.conf.all.forwarding=1 \\\n" \
  "  && ifconfig %s\n" \
  , d, h, d, a, d

#define LINUX_LEGACY_TUNDOWN_SCRIPT(d) \
  "ifconfig %s down \\\n" \
  "  && sysctl -w net.ipv6.conf.all.forwarding=0 \\\n" \
  , d

/* XXX: Will this work on other BSD derivatives? */
#define DARWIN_TUNCONF_SCRIPT(d, a, h) \
  "ifconfig %s inet6 %s up \\\n" \
  "  && sysctl -w net.inet6.ip6.forwarding=1 \\\n" \
  "  && ifconfig %s\n" \
  , d, a, d

#define DARWIN_TUNDOWN_SCRIPT(d) \
  "ifconfig %s down \\\n" \
  "  && sysctl -w net.inet6.ip6.forwarding=0 \\\n" \
  , d

/* TODO:
 * - add `busybox` support
 * - add option to disable `sysctl`
*/

/*---------------------------------------------------------------------------*/
inline int ssystem(const char *fmt, ...)
     __attribute__((__format__ (__printf__, 1, 2)));
inline int
ssystem(const char *fmt, ...) __attribute__((__format__ (__printf__, 1, 2)));

inline int
ssystem(const char *fmt, ...)
{
  char cmd[128];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(cmd, sizeof(cmd), fmt, ap);
  va_end(ap);
  printf("%s\n", cmd);
  fflush(stdout);
  return system(cmd);
}
/*---------------------------------------------------------------------------*/
inline void tunconf(void)
{
  ssystem(
#ifdef linux
#  if defined(LINUX_IPROUTE2) || LINUX_IPROUTE2
    LINUX_IPROUTE2_TUNCONF_SCRIPT(
#  else
    LINUX_LEGACY_TUNCONF_SCRIPT(
#  endif
#elif defined(__APPLE__)
    DARWIN_TUNCONF_SCRIPT(
#endif
      slip_config_tundev, slip_config_ipaddr, HOSTIP));
}
/*---------------------------------------------------------------------------*/
inline void tundown(void)
{
  ssystem(
#ifdef linux
#  if defined(LINUX_IPROUTE2) || LINUX_IPROUTE2
    LINUX_IPROUTE2_TUNDOWN_SCRIPT(
#  else
    LINUX_LEGACY_TUNDOWN_SCRIPT(
#  endif
#elif defined(__APPLE__)
    DARWIN_TUNDOWN_SCRIPT(
#endif
      slip_config_tundev));
}
/*---------------------------------------------------------------------------*/
