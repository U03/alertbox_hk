
FROM ubuntu:20.04

RUN apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y \
        moreutils \
        apache2-utils \
        mosquitto-clients \
 && apt-get clean autoclean \
 && apt-get autoremove --yes

WORKDIR /alertbox

CMD mosquitto_sub $MOSQUITTO_SUB -F '%t %p' | ts '%Y/%m/%d %H:%M:%S ' | rotatelogs -l /alertbox/alertbox.%Y%m%d.log 86400
