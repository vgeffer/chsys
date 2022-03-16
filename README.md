# chsys
An experimental binary/source package manager for Linux

Also contains testing package **test.tar**

Confirmed working on:
  - Arch Linux, Kernel v. 5.16.14
  - POP_os, Kernel v. 5.16.11


Compiling:
```
# make build && cp chsys-install /bin && cp chsys-query /bin
```


Installing package:
```
# chsys-install add test.tar
```
