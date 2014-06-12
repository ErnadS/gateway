#!/bin/sh

CGI="/srv/www/cgi-bin"
S20ELREG="/etc/rc5.d"
MAIN="/tmp"

cd ${CGI}
pwd
echo -n "Starter Elreg script."

echo -n "change permitions"

chmod 767 /srv/www/cgi-bin

chmod 777 . cgiAlarmPanel
chmod 777 . cgiAlarmPanelInfo
chmod 777 . cgiCommonData
chmod 777 . cgiHelp
chmod 777 . cgiIsoLocator
chmod 777 . cgiIsoLocatorInfo
chmod 777 . cgiIsoLocatorMeasur
chmod 777 . cgiLeftMenu
chmod 777 . cgiLog
chmod 777 . cgiSwitchover
chmod 777 . cgiSwitchoverInfo
chmod 777 . cgiSwitchoverMeasur
chmod 777 . cgiTrafoguard
chmod 777 . cgiTrafoguardInfo
chmod 777 . cgiTrafoGuardMeasur
chmod 777 . login

cd ${S20ELREG}
chmod 770 . S20Elreg

cd ${MAIN}
chmod 770 . gatewayAT91SAM



echo -n "Elreg script end"

