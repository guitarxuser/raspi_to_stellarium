#!/bin/sh

#install within /usr/local/bin

ln -s $PWD/tty0tty/pts/tty0tty /usr/local/bin/tty0tty
ln -s $PWD/rot_enc/rot_enc /usr/local/bin/rot_enc
ln -s $PWD/rot_enc/rot_enc_dummy /usr/local/bin/rot_enc_dummy
ln -s $PWD/rot_enc/rot_enc_for_ts /usr/local/bin/rot_enc_for_ts
ln -s $PWD/telescope_server/src/TelescopeServerNexStar /usr/local/bin/TelescopeServerNexStar
ls -ltr /usr/local/bin
