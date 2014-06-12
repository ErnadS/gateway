DESCRIPTION = "Move files to image"
LICENSE = "GPL"
# package revision
PR = "r0"

# file:// is my "/stuff/openembedded/recipes/elreg/elreg" directory
# Source are all files from tmp directory
SRC_URI = "file://tmp/* file://images/* file://cgi/* file://etc/* file://srv/* file://lib/* file://www/* file://gateway/*"

EXTRA_OEMAKE = ""

# ovo su mape koje imam skupa sa mojim BB-filom
S = "${WORKDIR}/tmp"
S1= "${WORKDIR}/www"
S2= "${WORKDIR}/images"
S3= "${WORKDIR}/srv"
S4= "${WORKDIR}/cgi"

# list of files to be installed
FILES_${PN} = "/etc/* /tmp/* /srv/* /gateway/* /lib/*"


do_install () {
# create directory
	install -d ${D}/etc/
	install -d ${D}/etc/init.d
	install -d ${D}/etc/rc5.d
	install -d ${D}/tmp/
	install -d ${D}/lib/
	install -d ${D}/srv/
	install -d ${D}/srv/www/
	install -d ${D}/srv/www/cgi-bin/
	install -d ${D}/srv/www/images
	install -d ${D}/gateway/dynamic
# copy file from source ($S) to destination ${D}/tmp
	install -m 0644 ${S}/testmail ${D}/tmp/
	install -m 0755 ${S}/ElregFoldersAndDriversScript.sh ${D}/tmp/
	install -m 0755 ${S}/ElregPermitionsScript.sh ${D}/tmp/
	install -m 0755 ${S}/SJA1000driver.ko ${D}/tmp/
	
	# ERROR: QA Issue with elreg: No GNU_HASH in the elf binary:
	# install -m 0755 ${S}/gatewayAT91SAM ${D}/tmp/

	install -m 0644 ${S3}/thttpd.conf ${D}/srv/

	install -m 0644 ${WORKDIR}/etc/resolv.conf ${D}/etc/
	# ne moze ovaj jer ga vec instalira sam thttpd
	# install -m 0755 ${WORKDIR}/etc/init.d/thttpd ${D}/etc/init.d/
	install -m 0755 ${WORKDIR}/etc/rc5.d/S20Elreg ${D}/etc/rc5.d/
	install -m 0755 ${WORKDIR}/etc/rc5.d/S20syslog ${D}/etc/rc5.d/

	# ERROR: QA Issue with elreg: No GNU_HASH in the elf binary:
	# install -m 0644 ${WORKDIR}/lib/libcommonGateway.so ${D}/lib/
	install -m 0644 ${WORKDIR}/lib/liblockfile.so.1 ${D}/lib/

	install -m 0644 ${WORKDIR}/gateway/dynamic/gw_setup_devicemonitor_1.htm ${D}/gateway/dynamic/
	install -m 0644 ${WORKDIR}/gateway/dynamic/gw_setup_devicemonitor_2.htm ${D}/gateway/dynamic/
	install -m 0644 ${WORKDIR}/gateway/dynamic/gw_setup_devicemonitor_3.htm ${D}/gateway/dynamic/

	for myFile in `ls -1 "${S1}/"`
	do		
		install -m 0644 "${S1}/${myFile}" "${D}/srv/www/"
	done

	for myFile in `ls -1 "${S2}/"`
	do		
		install -m 0644 "${S2}/${myFile}" "${D}/srv/www/images/"
	done


 #	for myFile in `ls -1 "${S4}/"`
#	 do		
#		install -m 0644 "${S4}/${myFile}" "${D}/srv/www/cgi-bin/"
#	 done
}


