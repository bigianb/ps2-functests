The following commands can be used to set-up a build environment using
ubuntu 20.04 LTS under WSL on a Windows 10 machine. This will allow you to compile
in a bash terminal but edit and access the files from a windows editor - visual studio code for example. This is useful when testing the output using a
windows based emulator such as PCSX2.

```
sudo apt-get update
sudo apt upgrade -y
sudo apt-get install gcc g++ make texinfo gettext bison flex libmpc-dev zlib1g-dev mkisofs

git clone https://github.com/ps2dev/ps2dev.git

sudo mkdir /usr/local/ps2dev
sudo chown $USER /usr/local/ps2dev

add the following to the end of ~/.profile

export PS2DEV=/usr/local/ps2dev
export PS2SDK=$PS2DEV/ps2sdk
export GSKIT=$PS2DEV/gsKit
export PATH=$PATH:$PS2DEV/bin:$PS2DEV/ee/bin:$PS2DEV/iop/bin:$PS2DEV/dvp/bin:$PS2SDK/bin

run build-all.sh in the target directory
```

If you're on a mac, install ps2dev and also cdrtools (use homebrew https://formulae.brew.sh/formula/cdrtools) for mkisofs.

