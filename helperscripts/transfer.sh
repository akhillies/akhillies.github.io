#!/bin/bash

fpath=public_html/$(dirname $1)
user="akhilb"
server="ssh.ocf.berkeley.edu"
pw="0thinnagains"

expect -c "
    spawn scp $1 $user@$server:$fpath
    expect assword: { send $pw\r }
    expect 100%
    sleep 1
    exit
    "
