#!/bin/bash

. /opt/esp32/esp-idf/export.sh > /dev/null 2>&1
idf.py app
curl -# "http://192.168.4.1/update" --compressed -H "Content-Type: application/octet-stream" --data-binary @build/sigfox-chicken-door.bin | tee /dev/null
