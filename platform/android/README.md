# Android OS as `native` target

This is an experimental platform that is aimed to allow building _"native"_
binaries which can run on Android targets, provided the user has root shell
access on the target. You will also need a custom kernel or at least a
checkout of the kernel tree which the device runs to cross-compile a module,
because nor `CONFIG_TUN` neither `CONFIG_USB_SERIAL_FTDI_SIO` is likelly to
be set in a typical Android kernel configuration.

## Examples

### Border Router
      cd examples/ipv6/native-border-router
      make TARGET=android \
        NDK=/opt/devel/tools/android/ndk_r7b/ \
        TOOLCHAIN=arm-linux-androideabi-4.4.3 \
        DROIDUPLE=android-14/arch-arm
      adb push ./border-router.android /system/bin/ctkbr
      adb shell /system/bin/ctkbr
