#!/bin/sh
BASE_PATH=/home/chronos/user/tv
INSTALL_PATH=/usr/share/tv-service

sudo cp -r "${BASE_PATH}/bin" "${INSTALL_PATH}/bin"
sudo cp -r "${BASE_PATH}/app" "${INSTALL_PATH}/app"
sudo cp "${BASE_PATH}/tv-service.conf.txt" "${INSTALL_PATH}/tv-service.conf"
sudo cp "${BASE_PATH}/auto-login.conf.txt" "${INSTALL_PATH}/auto-login.conf"
sudo initctl reload-configuration