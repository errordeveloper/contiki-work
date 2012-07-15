#define LINUX_IPROUTE2_TUNCONF_SCRIPT(d, a) \
  "ip link set %s up \\\n" \
  "  && ip addr add $CONTIKI_BORDER_ROUTER_TUNNEL_ADDRESS_IPV4 dev %s \\\n" \
  "  && ip addr add %s dev %s \\\n" \
  "  && ip addr show dev %s\n" \
  , d, d, a, d, d

#define LINUX_IPROUTE2_TUNDOWN_SCRIPT(d) \
  "ip link set %s down \\\n" \
  , d

#define LINUX_LEGACY_TUNCONF_SCRIPT(d, a) \
  "ifconfig %s inet $CONTIKI_BORDER_ROUTER_TUNNEL_ADDRESS_IPV4 up \\\n" \
  "  && ifconfig %s add %s \\\n" \
  "  && ifconfig %s\n" \
  , d, d, a, d

#define LINUX_LEGACY_TUNDOWN_SCRIPT(d) \
  "ifconfig %s down \\\n" \
  , d

/* XXX: Will this work on other BSD derivatives? */
#define DARWIN_TUNCONF_SCRIPT(d, a) \
  "ifconfig %s inet6 %s up \\\n" \
  "  && ifconfig %s\n" \
  , d, a, d

#define DARWIN_TUNDOWN_SCRIPT(d) \
  "ifconfig %s down \\\n" \
  , d

/*---------------------------------------------------------------------------*/
inline void sysctl_forwarding (int set) {
#if defined(linux)
  /*
//#define SYSCTL_NAME "net.ipv6.conf.all.forwarding"
//#define SYSCTL_NAME "/proc/sys/net/ipv6/conf/all/forwarding"
  int name[] = { NET_IPV6, NET_IPV6_CONF, NET_PROTO_CONF_ALL, NET_IPV6_FORWARDING }
  struct __sysctl_args args;
  memset(&args, 0, sizeof(struct __sysctl_args));
  args.name = name;
  args.nlen = sizeof(name)/sizeof(name[0]);
  */

#elif defined(__APPLE__)
#define SYSCTL_NAME "net.inet6.ip6.forwarding"
#include <sys/sysctl.h>
  int mib[4], val;
  size_t len = 4;

  if (!slip_config_sysctl) return;

  sysctlnametomib(SYSCTL_NAME, mib, &len);
  len = sizeof(val);
  if (sysctl(mib, 4, &val, &len, NULL, 0) == -1) {
    printf("Failed to get `%s`", SYSCTL_NAME);
  }
  else if (len > 0) {
    if(val == set) {
      printf("IPv6 packet forwarding already set!");
      printf(" [`%s` = (%d)]\n", SYSCTL_NAME, val);
    } else {
      printf("Setting IPv6 packet forwarding.");
      printf(" [`%s` = (%d -> %d)]\n", SYSCTL_NAME, val, set);
      if (sysctl(mib, 4, NULL, NULL, &set, len) == -1) {
        printf("Failed to set `%s`", SYSCTL_NAME);
      }
    }
  }
#else
#warning "sysctl_forwarding: no SYSCTL_NAME defined for the host platform"
#endif
}
/*---------------------------------------------------------------------------*/
inline int ssystem(const char *fmt, ...)
     __attribute__((__format__ (__printf__, 1, 2)));
inline int
ssystem(const char *fmt, ...) __attribute__((__format__ (__printf__, 1, 2)));

inline int
ssystem(const char *fmt, ...)
{
  char cmd[1024];
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
#ifndef CUSTOM_TUNDOWN_SCRIPT
#ifdef linux
#  if defined(LINUX_IPROUTE2) && LINUX_IPROUTE2
    LINUX_IPROUTE2_TUNCONF_SCRIPT(
#  else
    LINUX_LEGACY_TUNCONF_SCRIPT(
#  endif
#elif defined(__APPLE__)
    DARWIN_TUNCONF_SCRIPT(
#endif /* linux || __APPLE__ */
#elif defined(CUSTOM_TUNCONF_SCRIPT)
    CUSTOM_TUNCONF_SCRIPT(
#endif
      slip_config_tundev, slip_config_ipaddr));
  sysctl_forwarding(1);
}
/*---------------------------------------------------------------------------*/
inline void tundown(void)
{
  ssystem(
#ifndef CUSTOM_TUNDOWN_SCRIPT
#ifdef linux
#  if defined(LINUX_IPROUTE2) && LINUX_IPROUTE2
    LINUX_IPROUTE2_TUNDOWN_SCRIPT(
#  else
    LINUX_LEGACY_TUNDOWN_SCRIPT(
#  endif
#elif defined(__APPLE__)
    DARWIN_TUNDOWN_SCRIPT(
#endif  /* linux || __APPLE__ */
#elif defined(CUSTOM_TUNDOWN_SCRIPT)
    CUSTOM_TUNDOWN_SCRIPT(
#endif
      slip_config_tundev));
  sysctl_forwarding(0);
}
/*---------------------------------------------------------------------------*/
