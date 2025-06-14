#!/bin/sh
pacman -S --needed --noconfirm $(cat packages_installed.lst)
