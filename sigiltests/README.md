installing sigil:
download source from https://gitlab.com/geoff-nagy/sigil/tree/master
install openal with:
```
sudo apt-get install libalut-dev
```
build according to these directions (4th post): https://openeuphoria.org/forum/133840.wc (saved in wayback machine april 13 2020)
except do `make` one directory up (lots of warnings)
also do a `make install`

To setup the shared library for the first time:
```
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
echo $LD_LIBRARY_PATH
```
