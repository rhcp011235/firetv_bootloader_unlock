firetv_bootloader_unlock
========================

Opensource Amazon FireTv Bootloader Unlock Tool

As the title says :)

This was reversed from http://forum.xda-developers.com/fire-tv/orig-development/firetv-partial-bootloader-unlock-t2925891

and I hate closed source things that do things to my systems :)

- Anyone sees anything else be sure to do a pull request

Also, If anyone wants to make an APK unlock for this! Let me know!!!!!!

Building
Download the Android Native Development Kit (NDK): http://developer.android.com/tools/sdk/ndk/index.html#Downloads

Extract into some directory and put that in your path: export PATH=ANDK_DIR:$PATH

In another directory clone this repo: git clone --recursive https://github.com/android-rooting-tools/android_run_root_shell

Change to the directory where the repo was cloned cd android_run_root_shell

To start build process use the following ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk

If all goes well you will get the compiled binary at: ./libs/armeabi/unlock_firetv

have fun guys and gals.

Submit your pulls and changes!
