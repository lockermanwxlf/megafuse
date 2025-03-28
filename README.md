# megafuse

A program to mount your root MEGA filesystem to a path on your local machine.

DISCLAIMER: Neither I nor this project are affiliated with MEGA nor the upstream sdk repo.

# Building

First you must obtain [vcpkg](https://github.com/microsoft/vcpkg). It is not listed as a submodule here.

```bash
git clone https://github.com/lockermanwxlf/megafuse.git
cd megafuse
git submodule update --init
cmake --preset Debug -DVCPKG_ROOT=<path-to-vcpkg>
cmake --build out/build/Default
```

# Usage
```bash
megafuse <path>
```

Upon first use, you'll be asked for your email and password.
On further uses, it will automatically log in with a session key
it saves to \<usercachefolder\>/megafuse/session.txt. This is cross-platform
with glib but since I use termios to disable echo during the credentials 
prompt, you can't use this program with Windows.

Mount points are not unmounted on program termination. 
You can do 

```bash
megafuse <path> & pid=$!; wait $pid; umount <path>;
```

in bash, or 

```fish
megafuse <path> &; set pid (jobs -lp); wait $pid; umount <path>;
```

in fish. It won't umount on SIGINT, but presumably you are using this
program noninteractively and will have to pkill it anyway.