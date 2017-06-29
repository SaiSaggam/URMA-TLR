/*
 cellular_countries:
 A table containing a list of countries and their known cellular providers, along with username/password/APN settings for each provider.
 Note that the APN entry is a table because some providers have more than one APN.

 This table has been updated using data from the apns.xml file from the Android project.

 Note : Updated following country names from (ISO 3166-2) standard to match country_list.js

 Bolivia       --- Bolivia, Plurinational State of
 Brunei        --- Brunei Darussalam
 Ivory Coast   --- Côte d'Ivoire
 Macau         --- Macao
 Macedonia     --- Macedonia, the Former Yugoslav Republic of
 Reunion       --- Réunion
 Russia        --- Russian Federation
 Saint Vincent --- Saint Vincent and the Grenadines
 South Korea   --- Korea, Republic of
 Syria         --- Syrian Arab Republic
 Taiwan        --- Taiwan, Province of China
 UAE           --- United Arab Emirates
 UK            --- United Kingdom
 Venezuela     --- Venezuela, Bolivarian Republic of
 Vietnam       --- Viet Nam
 */
export const cellular_countries = {
    "Albania" : [
        { "provider" : "Vodafone AL",		"username" : "a",	 "password" : "a",	"APN" : [ "Twa" ] }
    ],
    "Algeria" : [
        { "provider" : "Djezzy",			"username" : "",	 "password" : "",	"APN" : [ "djezzy.internet", "djezzy.mms" ] },
        { "provider" : "Mobilis",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Nedjma",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Angola" : [
        { "provider" : "Movicel Angola",	"username" : "",	 "password" : "",	"APN" : [ "internet.movicel.co.ao" ] },
        { "provider" : "Unitel",			"username" : "",	 "password" : "",	"APN" : [ "internet.unitel.co.ao" ] },
        { "provider" : "Vodacom",			"username" : "",	 "password" : "",	"APN" : [ "vodanet" ] }
    ],
    "Argentina" : [
        { "provider" : "CTI",				"username" : "",	 "password" : "",	"APN" : [ "internet.ctimovil.com.ar", "mms.ctimovil.com.ar" ] },
        { "provider" : "Claro",				"username" : "",	 "password" : "",	"APN" : [ "igprs.claro.com.ar", "mms.ctimovil.com.ar" ] },
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "internet.unifon", "mms.gprs.unifon.com.ar", "wap.gprs.unifon.com.ar"] },
        { "provider" : "Personal",			"username" : "",	 "password" : "",	"APN" : [ "gprs.personal.com" ]	}
    ],
    "Armenia" : [
        { "provider" : "BeeLine",			"username" : "",	 "password" : "",	"APN" : [ "mms.beeline.am", "internet.beeline.am" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "VivaCell",			"username" : "",	 "password" : "",	"APN" : [ "connect.vivacell.am", "mms.vivacell.am" ] }
    ],
    "Aruba" : [
        { "provider" : "Setar",				"username" : "",	 "password" : "",	"APN" : [ "mms.setar.aw" ] }
    ],
    "Australia" : [
        { "provider" : "3",					"username" : "",	 "password" : "",	"APN" : [ "3services", "3netaccess" ] },
        { "provider" : "Apex Telecom",		"username" : "",	 "password" : "",	"APN" : [ "splns357" ] },
        { "provider" : "Exetel",			"username" : "",	 "password" : "",	"APN" : [ "exetel1" ] },
        { "provider" : "M2MOne",			"username" : "",	 "password" : "",	"APN" : [ "m2mone.com.au", "telstra.m2m", "telstra.internet", "telstra.extranet" ] },
        { "provider" : "Optus",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms", "om2moptus", "om2mpub1", "yesinternet" ] },
        { "provider" : "Telstra",			"username" : "",	 "password" : "",	"APN" : [ "telstra.pcpack", "Telstra.wap", "Telstra.mms", "telstra.internet", "telstra.m2m" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "live.vodafone.com", "vfinternet.au" ] },
        { "provider" : "Virgin AU",			"username" : "",	 "password" : "",	"APN" : [ "virgininternet", "virginmms" ] }
    ],
    "Austria" : [
        { "provider" : "A1",				"username" : "",	 "password" : "",	"APN" : [ "a1.net" ] },
        { "provider" : "Bob",				"username" : "",	 "password" : "",	"APN" : [ "bob.at", "mms.bob.at" ] },
        { "provider" : "Drei A",			"username" : "",	 "password" : "",	"APN" : [ "drei.at" ] },
        { "provider" : "Mobilkom",			"username" : "",	 "password" : "",	"APN" : [ "a1.net", "free.a1.net" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "web.one.at", "mms.one.at", "orange.mms", "orange.web" ] },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "gprsinternet", "gprsmms" ] },
        { "provider" : "tele.ring",			"username" : "",	 "password" : "",	"APN" : [ "mms", "web" ] }
    ],
    "Azerbaijan" : [
        { "provider" : "Azercell",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Bakcell",			"username" : "",	 "password" : "",	"APN" : [ "mms" ] }
    ],
    "Bahrain" : [
        { "provider" : "BATELCO",			"username" : "",	 "password" : "",	"APN" : [ "internet.batelco.com", "mms.batelco.com" ] },
        { "provider" : "MTC Vodafone",		"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Viva",				"username" : "",	 "password" : "",	"APN" : [ "viva.bh" ] }
    ],
    "Bangladesh" : [
        { "provider" : "Banglalink",		"username" : "",	 "password" : "",	"APN" : [ "blweb", "blmms" ] },
        { "provider" : "Grameenphone",		"username" : "",	 "password" : "",	"APN" : [ "gpinternet" ] },
        { "provider" : "Robi",				"username" : "",	 "password" : "",	"APN" : [ "internet", "WAP" ] },
        { "provider" : "Warid BD",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Barbados" : [
        { "provider" : "Digicel",			"username" : "",	 "password" : "",	"APN" : [ "wap.digicelbarbados.com" ] },
    ],
    "Belarus" : [
        { "provider" : "Life",				"username" : "",	 "password" : "",	"APN" : [ "mms.life.com.by" ] },
        { "provider" : "MTS",				"username" : "",	 "password" : "",	"APN" : [ "mts" ] },
        { "provider" : "Velcom",			"username" : "",	 "password" : "",	"APN" : [ "mms.velcom.by", "plus.velcom.by", "vmi.velcom.by", "web.velcom.by", "web1.velcom.by", "web2.velcom.by", "web3.velcom.by" ] },
    ],
    "Belgium" : [
        { "provider" : "BASE Belgium",		"username" : "",	 "password" : "",	"APN" : [ "gprs.base.be", "mms.base.be" ] },
        { "provider" : "Mobile Vikings",	"username" : "",	 "password" : "",	"APN" : [ "mms.be" ] },
        { "provider" : "Mobistar",			"username" : "",	 "password" : "",	"APN" : [ "mworld.be", "mms.be" ] },
        { "provider" : "Proximus",			"username" : "",	 "password" : "",	"APN" : [ "internet.proximus.be", "event.proximus.be" ] },
        { "provider" : "Telenet",			"username" : "",	 "password" : "",	"APN" : [ "telenetwap.be" ] }
    ],
    "Bolivia, Plurinational State of" : [
        { "provider" : "Viva GPRS",			"username" : "",	 "password" : "",	"APN" : [ "internet.nuevatel.com" ] }
    ],
    "Bosnia and Herzegovina" : [
        { "provider" : "BH Mobile",			"username" : "",	 "password" : "",	"APN" : [ "active.bhmobile.ba" ] },
        { "provider" : "ERONET",			"username" : "",	 "password" : "",	"APN" : [ "data1.eronet.ba", "data2.eronet.ba", "data3.eronet.ba", "data4.eronet.ba", "gprs.eronet.ba", "mms.eronet.ba", "web.eronet.ba"  ] }
    ],
    "Botswana" : [
        { "provider" : "Mascom",			"username" : "",	 "password" : "",	"APN" : [ "internet.mascom" ] },
        { "provider" : "Orange BW",			"username" : "",	 "password" : "",	"APN" : [ "internet.orange.co.bw" ] },
        { "provider" : "TNM",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Brazil" : [
        { "provider" : "Amazonia",			"username" : "",	 "password" : "",	"APN" : [ "gprs.amazoniacelular.com.br","mmsgprs.amazoniacelular.com.br" ] },
        { "provider" : "BrT",				"username" : "",	 "password" : "",	"APN" : [ "brt.br", "mms.brt.br" ] },
        { "provider" : "Claro",				"username" : "",	 "password" : "",	"APN" : [ "claro.com.br", "mms.claro.com.br", "wap.claro.com.br" ] },
        { "provider" : "Oi",				"username" : "",	 "password" : "",	"APN" : [ "gprs.oi.com.br", "mmsgprs.oi.com.br" ] },
        { "provider" : "SCTL",				"username" : "",	 "password" : "",	"APN" : [ "sercomtel.com.br", "mms.sercomtel.com.br" ] },
        { "provider" : "TIM",				"username" : "",	 "password" : "",	"APN" : [ "tim.br", "mms.tim.br" ] },
        { "provider" : "Telemig",			"username" : "",	 "password" : "",	"APN" : [ "gprs.telemigcelular.com.br", "mmsgprs.telemigcelular.com.br" ] },
        { "provider" : "VIVO",				"username" : "",	 "password" : "",	"APN" : [ "mms.vivo.com.br", "zap.vivo.com.br" ] }
    ],
    "Brunei Darussalam" : [
        { "provider" : "B-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "bmobilewap", "bmobilemms" ] },
        { "provider" : "DSTCom",			"username" : "",	 "password" : "",	"APN" : [ "dst.internet" ] }
    ],
    "Bulgaria" : [
        { "provider" : "GLOBUL Internet",	"username" : "",	 "password" : "",	"APN" : [ "globul" ] },
        { "provider" : "M-Tel",				"username" : "",	 "password" : "",	"APN" : [ "inet-gprs.mtel.bg","mms-gprs.mtel.bg" ] },
        { "provider" : "VIVACOM",			"username" : "",	 "password" : "",	"APN" : [ "internet.vivacom.bg", "mms.vivacom.bg" ] }
    ],
    "Cameroon" : [
        { "provider" : "MTN",				"username" : "",	 "password" : "",	"APN" : [ "INTERNET" ] }
    ],
    "Canada" : [
        { "provider" : "Bell",				"username" : "",	 "password" : "",	"APN" : [ "pda.bell.ca", "inet.bell.ca" ],              "carrier" : "bell" },
        { "provider" : "Fido",				"username" : "",	 "password" : "",	"APN" : [ "fido-core-appl1.apn", "internet.fido.ca", "isp.fido.apn" ] },
        { "provider" : "Koodo",				"username" : "",	 "password" : "",	"APN" : [ "sp.koodo.com" ] },
        { "provider" : "Mobilicity",		"username" : "",	 "password" : "",	"APN" : [ "wap.davewireless.com", "mms.davewireless.com" ] },
        { "provider" : "Rogers",			"username" : "",	 "password" : "",	"APN" : [ "rogers-core-appl1.apn", "isp.apn", "internet.com" ],              "carrier" : "rogers" },
        { "provider" : "Sasktel",			"username" : "",	 "password" : "",	"APN" : [ "inet.stm.sk.ca" ] },
        { "provider" : "Telus",				"username" : "",	 "password" : "",	"APN" : [ "sp.telus.com", "isp.telus.com" ],                          "carrier" : "telus" },
        { "provider" : "Wind",				"username" : "",	 "password" : "",	"APN" : [ "internet.windmobile.ca", "mms.windmobile.ca" ] }
    ],
    "Chile" : [
        { "provider" : "Claro",				"username" : "",	 "password" : "",	"APN" : [ "bam.clarochile.cl", "mms.clarochile.cl" ] },
        { "provider" : "Entel",				"username" : "",	 "password" : "",	"APN" : [ "bam.entelpcs.cl", "mms.entelpcs.cl" ] },
        { "provider" : "Internet Movil",	"username" : "",	 "password" : "",	"APN" : [ "bam.entelpcs.cl" ] },
        { "provider" : "EPCS",				"username" : "",	 "password" : "",	"APN" : [ "mms.entelpcs.cl", "imovil.entelpcs.cl" ] },
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "mms.tmovil.cl" ] }
    ],
    "China" : [
        { "provider" : "China Mobile",		"username" : "",	 "password" : "",	"APN" : [ "cmnet", "cmwap" ] },
        { "provider" : "China Unicom",		"username" : "",	 "password" : "",	"APN" : [ "3gnet", "uniwap" ] },
        { "provider" : "Mobitel",			"username" : "",	 "password" : "",	"APN" : [ "postpaid" ] }
    ],
    "Colombia" : [
        { "provider" : "COMCEL",			"username" : "",	 "password" : "",	"APN" : [ "internet.comcel.com.co", "mms.comcel.com.co" ] },
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "internet.movistar.com.co" ] },
        { "provider" : "OLA",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms.comcel.com.co" ] },
        { "provider" : "Tigo",				"username" : "",	 "password" : "",	"APN" : [ "mms.colombiamovil.com.co", "web.colombiamovil.com.co" ] }
    ],
    "Croatia" : [
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "mms.htgprs", "web.htgprs" ] },
        { "provider" : "Tele2",				"username" : "",	 "password" : "",	"APN" : [ "wap.tele2.hr" ] },
        { "provider" : "VIPnet",			"username" : "",	 "password" : "",	"APN" : [ "gprs0.vipnet.hr", "data.cip.hr" ] }
    ],
    "Cyprus" : [
        { "provider" : "Cytamobile",		"username" : "",	 "password" : "",	"APN" : [ "cytamobile", "internet" ] },
        { "provider" : "MTN",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Czech Republic" : [
        { "provider" : "O2",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "click" ] },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "internet.t-mobile.cz", "mms.t-mobile.cz" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] }
    ],
    "Denmark" : [
        { "provider" : "3",					"username" : "",	 "password" : "",	"APN" : [ "data.tre.dk" ] },
        { "provider" : "Bibob",				"username" : "",	 "password" : "",	"APN" : [ "internet.bibob.dk", "mms.bibob.dk" ] },
        { "provider" : "Onfone",			"username" : "",	 "password" : "",	"APN" : [ "internet.sp.dk" ] },
        { "provider" : "TDC",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms", "wap" ] },
        { "provider" : "Telenor",			"username" : "",	 "password" : "",	"APN" : [ "internet", "telenor" ] },
        { "provider" : "Telia",				"username" : "",	 "password" : "",	"APN" : [ "www.internet.mtelia.dk", "www.mms.mtelia.dk" ] }
    ],
    "Dominican Republic" : [
        { "provider" : "BATELCO",			"username" : "",	 "password" : "",	"APN" : [ "internet.btcbahamas.com" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "orangenet.com.do" ] }
    ],
    "Ecuador" : [
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "internet.movistar.com.ec" ] }
    ],
    "Egypt" : [
        { "provider" : "Mobinil",			"username" : "",	 "password" : "",	"APN" : [ "mobinilmms", "mobinilweb" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "internet.vodafone.net" ] }
    ],
    "El Salvador" : [
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "internet.movistar.sv", "mms.movistar.sv" ] }
    ],
    "Equatorial Guinea" : [
        { "provider" : "Orange GQ MMS",		"username" : "",	 "password" : "",	"APN" : [ "orangemms" ] }
    ],
    "Estonia" : [
        { "provider" : "EMT",				"username" : "",	 "password" : "",	"APN" : [ "internet.emt.ee", "mms.emt.ee" ] },
        { "provider" : "Elisa",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Tele2",				"username" : "",	 "password" : "",	"APN" : [ "internet.tele2.ee", "mms.emt.ee" ] }
    ],
    "Fiji" : [
        { "provider" : "Digicel",			"username" : "",	 "password" : "",	"APN" : [ "wap.digicelpacific.com" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "live.vodafone.com.fj", "vfinternet.fj" ] }
    ],
    "Finland" : [
        { "provider" : "DNA",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Elisa",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Saunalahti",		"username" : "",	 "password" : "",	"APN" : [ "internet.saunalahti", "mms.saunalahti.fi" ] },
        { "provider" : "Sonera",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "TDC",				"username" : "",	 "password" : "",	"APN" : [ "internet.song.fi", "mms.song.fi" ] },
        { "provider" : "TeliaSonera",		"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "France" : [
        { "provider" : "ACCESS DATA",		"username" : "",	 "password" : "",	"APN" : [ "websfr" ] },
        { "provider" : "Bouygues",			"username" : "",	 "password" : "",	"APN" : [ "a2bouygtel.com", "b2bouygtel.com", "ebouygtel.com", "fipbouygtel.com", "mmsbouygtel.com", "objcobytel.com" ] },
        { "provider" : "Free Mobile",		"username" : "",	 "password" : "",	"APN" : [ "free", "mmsfree" ] },
        { "provider" : "NRJ Mobile",		"username" : "",	 "password" : "",	"APN" : [ "fnetnrj", "mmsnrj" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "orange", "orange.acte", "orange.fr", "orange.m2m", "orange.m2m.spec" ] },
        { "provider" : "SFR",				"username" : "",	 "password" : "",	"APN" : [ "sl2sfr", "wapsfr", "websfr" ] },
        { "provider" : "Virgin",			"username" : "",	 "password" : "",	"APN" : [ "ofnew.fr" ] },
        { "provider" : "Web La Poste",		"username" : "",	 "password" : "",	"APN" : [ "wapdebitel" ] }
    ],
    "Georgia" : [
        { "provider" : "Geocell",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Germany" : [
        { "provider" : "1und1",				"username" : "",	 "password" : "",	"APN" : [ "web.vodafone.de" ] },
        { "provider" : "Blau",				"username" : "",	 "password" : "",	"APN" : [ "internet.eplus.de", "mms.eplus.de" ] },
        { "provider" : "Telecom Deutschland","username" : "", 	 "password" : "",	"APN" : [ "internet.telekom", "internet.t-d1.de" ] },
        { "provider" : "E-Plus",			"username" : "",	 "password" : "",	"APN" : [ "internet.eplus.de", "mms.eplus.de" ] },
        { "provider" : "O2",				"username" : "",	 "password" : "",	"APN" : [ "internet", "pinternet.interkon.de", "surfo2" ] },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "internet.t-mobile" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "event.vodafone.de", "web.vodafone.de" ] }
    ],
    "Greece" : [
        { "provider" : "Cosmote",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Q-Telecom",			"username" : "",	 "password" : "",	"APN" : [ "q-mms.myq.gr" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "internet", "internet.vodafone.gr", "mms.vodafone.net", "surfonly.vodafone.gr", "webkarta.vodafone.gr" ] },
        { "provider" : "WIND",				"username" : "",	 "password" : "",	"APN" : [ "gint.b-online.gr" ] }
    ],
    "Greenland" : [
        { "provider" : "Tele",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Guadeloupe" : [
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "orangewap" ] }
    ],
    "Hong Kong" : [
        { "provider" : "3",					"username" : "",	 "password" : "",	"APN" : [ "ipc.three.com.hk", "imobile.three.com.hk", "mobile.lte.three.com", "mobile.three.com.hk", "mms-g.three.com.hk", "web-g.three.com.hk" ] },
        { "provider" : "CSL, one2free",		"username" : "",	 "password" : "",	"APN" : [ "cslp1", "cslp2", "cslp3", "hkcsl", "lte.public", "lte.mobile", "lte.internet" ] },
        { "provider" : "China Mobile",		"username" : "",	 "password" : "",	"APN" : [ "cmhk", "peoples.mms" ] },
        { "provider" : "PCCW",				"username" : "",	 "password" : "",	"APN" : [ "pccw", "pccwdata", "pccwmms" ] },
        { "provider" : "PEOPLES",			"username" : "",	 "password" : "",	"APN" : [ "peoples.net", "170170" ] },
        { "provider" : "SMC-Voda",			"username" : "",	 "password" : "",	"APN" : [ "SmarTone-Vodafone" ] }
    ],
    "Hungary" : [
        { "provider" : "Pannon",			"username" : "",	 "password" : "",	"APN" : [ "net", "mms" ] },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "wnw", "mms", "internet" ] },
        { "provider" : "Telenor Net",		"username" : "",	 "password" : "",	"APN" : [ "net" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "internet.vodafone.net" ] }
    ],
    "Iceland" : [
        { "provider" : "Nova",				"username" : "",	 "password" : "",	"APN" : [ "mms.nova.is" ] },
        { "provider" : "Siminn",			"username" : "",	 "password" : "",	"APN" : [ "gprs.is", "internet", "mms.gprs.is", "mms.simi.is" ] }
    ],
    "India" : [
        { "provider" : "Aircel",			"username" : "",	 "password" : "",	"APN" : [ "aircelgprs", "airtelgprs.com", "aircelmms" ] },
        { "provider" : "BPL",				"username" : "",	 "password" : "",	"APN" : [ "mizone", "www" ] },
        { "provider" : "BSNL",				"username" : "",	 "password" : "",	"APN" : [ "bsnlnet", "mmssouth.cellone.in" ] },
        { "provider" : "IDEA",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mmsc" ] },
        { "provider" : "India Hutch",		"username" : "",	 "password" : "",	"APN" : [ "portalnmms" ] },
        { "provider" : "MTNL",				"username" : "",	 "password" : "",	"APN" : [ "gprsmtnldel", "gprsmtnlmum" ] },
        { "provider" : "Reliance",			"username" : "",	 "password" : "",	"APN" : [ "rcomnet", "rcommms" ] },
        { "provider" : "SPICE",				"username" : "",	 "password" : "",	"APN" : [ "spicegprs", "spicemms" ] },
        { "provider" : "TATA DoCoMo",		"username" : "",	 "password" : "",	"APN" : [ "TATA.DOCOMO.INTERNET", "TATA.DOCOMO.MMS" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "portalnmms", "www" ] }
    ],
    "Indonesia" : [
        { "provider" : "3",					"username" : "",	 "password" : "",	"APN" : [ "3gprs", "3mms" ] },
        { "provider" : "AXIS",				"username" : "",	 "password" : "",	"APN" : [ "AXIS" ] },
        { "provider" : "Excelcom",			"username" : "",	 "password" : "",	"APN" : [ "www.xlgprs.net", "www.xlmms.net" ] },
        { "provider" : "IM3",				"username" : "",	 "password" : "",	"APN" : [ "indosatmms", "www.indosat-m3.net" ] },
        { "provider" : "Indosat",			"username" : "",	 "password" : "",	"APN" : [ "indosat3g", "indosatgprs", "indosatmms" ] },
        { "provider" : "Telkomsel",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] }
    ],
    "Ireland" : [
        { "provider" : "3",					"username" : "",	 "password" : "",	"APN" : [ "3ireland.ie" ] },
        { "provider" : "Eircom/Meteor",		"username" : "",	 "password" : "",	"APN" : [ "data.mymeteor.ie", "mms.mymeteor.ie", "wap.mymeteor.ie" ] },
        { "provider" : "O2",				"username" : "",	 "password" : "",	"APN" : [ "internet", "wap.dol.ie" ] },
        { "provider" : "Vodafone IE",		"username" : "",	 "password" : "",	"APN" : [ "isp.vodafone.ie", "live.vodafone.com", "mms.vodafone.net" ] }
    ],
    "Israel" : [
        { "provider" : "CellCOM",			"username" : "",	 "password" : "",	"APN" : [ "internetg", "mms" ] },
        { "provider" : "Jawwal Internet",	"username" : "",	 "password" : "",	"APN" : [ "internet", "mms", "wap" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "internet", "wap.orange.co.il" ] },
        { "provider" : "Pelephone",			"username" : "",	 "password" : "",	"APN" : [ "internet.pelephone.net.il", "mms.pelephone.net.il" ] },
        { "provider" : "Rami Levey",		"username" : "",	 "password" : "",	"APN" : [ "internet.rl" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Italy" : [
        { "provider" : "3",					"username" : "",	 "password" : "",	"APN" : [ "tre.it" ] },
        { "provider" : "Fastweb",			"username" : "",	 "password" : "",	"APN" : [ "apn.fastweb.it" ] },
        { "provider" : "NOVERCA",			"username" : "",	 "password" : "",	"APN" : [ "mms.noverca.it", "web.noverca.it" ] },
        { "provider" : "TIM",				"username" : "",	 "password" : "",	"APN" : [ "ibox.tim.it", "mms.tim.it", "wap.tim.it" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "mobile.vodafone.it","mms.vodafone.it", "web.omnitel.it" ] },
        { "provider" : "Wind",				"username" : "",	 "password" : "",	"APN" : [ "internet.wind", "mms.wind" ] },
        { "provider" : "iTIM",				"username" : "",	 "password" : "",	"APN" : [ "unico.tim.it" ] }
    ],
    "Côte d'Ivoire" : [
        { "provider" : "MTN",				"username" : "",	 "password" : "",	"APN" : [ "mms.mtn.ci" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "orangecimms", "orangeciweb" ] }
    ],
    "Jamaica" : [
        { "provider" : "Digicel",			"username" : "",	 "password" : "",	"APN" : [ "wap.digiceljamaica.com"] }
    ],
    "Japan" : [
        { "provider" : "NTT Docomo",		"username" : "",	 "password" : "",	"APN" : [ "mopera.flat.foma.ne.jp", "mpr.ex-pkt.net", "mpr2.bizho.net", "open.mopera.net", "mopera.net" ] },
        { "provider" : "Softbank",			"username" : "",	 "password" : "",	"APN" : [ "open.softbank.ne.jp", "smile.world" ] }
    ],
    "Jordan" : [
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "net.orange.jo" ] },
        { "provider" : "Umniah",			"username" : "",	 "password" : "",	"APN" : [ "net", "mms" ] },
        { "provider" : "Zain",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] }
    ],
    "Kazakhstan" : [
        { "provider" : "BeeLine",			"username" : "",	 "password" : "",	"APN" : [ "internet.beeline.kz" ] },
        { "provider" : "Kcell",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Kenya" : [
        { "provider" : "Zain",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "yu",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Kosovo" : [
        { "provider" : "Mobitel",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Si.mobil",			"username" : "",	 "password" : "",	"APN" : [ "internet.simobil.si", "mms.simobil.si" ] },
        { "provider" : "T-2",				"username" : "",	 "password" : "",	"APN" : [ "internet.t-2.net", "mms.t-2.net" ] },
        { "provider" : "Tusmobil",			"username" : "",	 "password" : "",	"APN" : [ "internet.tusmobil.si" ] }
    ],
    "Kuwait" : [
        { "provider" : "MTC Vodafone",		"username" : "",	 "password" : "",	"APN" : [ "apn01", "mms" ] },
        { "provider" : "Viva",				"username" : "",	 "password" : "",	"APN" : [ "viva" ] },
        { "provider" : "Zain",				"username" : "",	 "password" : "",	"APN" : [ "pps" ] },
        { "provider" : "wataniya",			"username" : "",	 "password" : "",	"APN" : [ "action.wataniya.com" ] }
    ],
    "Latvia" : [
        { "provider" : "Bite",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "LMT",				"username" : "",	 "password" : "",	"APN" : [ "internet.lmt.lv", "mms.lmt.lv" ] },
        { "provider" : "Tele2",				"username" : "",	 "password" : "",	"APN" : [ "internet.tele2.lv", "mms.tele2.lv" ] }
    ],
    "Lebanon" : [
        { "provider" : "Alfa",				"username" : "",	 "password" : "",	"APN" : [ "internet.mic1.com.lb", "mms.mic1.com.lb", "wap.mic1.com.lb" ] },
        { "provider" : "Dialog Mobile",		"username" : "",	 "password" : "",	"APN" : [ "dialogbb" ] },
        { "provider" : "mtc touch",			"username" : "",	 "password" : "",	"APN" : [ "gprs.mtctouch.com.lb", "mms.mtctouch.com.lb" ] }
    ],
    "Libya" : [
        { "provider" : "Al-Madar",			"username" : "",	 "password" : "",	"APN" : [ "almadar.mms" ] },
        { "provider" : "Libyana",			"username" : "",	 "password" : "",	"APN" : [ "wap", "mms" ] },
        { "provider" : "Orange TN",			"username" : "",	 "password" : "",	"APN" : [ "keypro" ] }
    ],
    "Lithuania" : [
        { "provider" : "Bite",				"username" : "",	 "password" : "",	"APN" : [ "banga", "mms", "wap" ] },
        { "provider" : "Omnitel",			"username" : "",	 "password" : "",	"APN" : [ "gprs.startas.lt", "omnitel", "gprs.mms.lt" ] },
        { "provider" : "Tele2",				"username" : "",	 "password" : "",	"APN" : [ "internet.tele2.lt", "mms.tele2.lt" ] }
    ],
    "Luxembourg" : [
        { "provider" : "Luxgsm Internet",	"username" : "",	 "password" : "",	"APN" : [ "web.pt.lu" ] },
        { "provider" : "Tango",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] }
    ],
    "Macao" : [
        { "provider" : "CTM",				"username" : "",	 "password" : "",	"APN" : [ "ctm-mobile", "ctmmms" ] },
        { "provider" : "Hutchison Macau",	"username" : "",	 "password" : "",	"APN" : [ "web-g.three.com.hk", "mms.hutchisonmacau.com" ] },
        { "provider" : "SmarTone",			"username" : "",	 "password" : "",	"APN" : [ "smartweb", "smartgprs" ] }
    ],
    "Macedonia, the Former Yugoslav Republic of" : [
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Madagascar" : [
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "orangemms" ] }
    ],
    "Malaysia" : [
        { "provider" : "Celcom",			"username" : "",	 "password" : "",	"APN" : [ "celcom3g", "celcom.net.my", "mms.celcom.net.my" ] },
        { "provider" : "Digi",				"username" : "",	 "password" : "",	"APN" : [ "diginet", "digimms" ] },
        { "provider" : "Maxis",				"username" : "",	 "password" : "",	"APN" : [ "net", "unet" ] },
        { "provider" : "U Mobile",			"username" : "",	 "password" : "",	"APN" : [ "my3g" ] },
        { "provider" : "Xpax",				"username" : "",	 "password" : "",	"APN" : [ "celcom", "celcom3g" ] }
    ],
    "Mali" : [
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Mexico" : [
        { "provider" : "Mensajas",			"username" : "",	 "password" : "",	"APN" : [ "mms.itelcel.com" ] },
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "internet.movistar.mx" ] },
        { "provider" : "Telcel",			"username" : "",	 "password" : "",	"APN" : [ "internet.itelcel.com", "mms.itelcel.com" ] }
    ],
    "Montenegro" : [
        { "provider" : "ProMonte",			"username" : "",	 "password" : "",	"APN" : [ "gprs.promonte.com", "mms.promonte.com" ] },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "mms" ] }
    ],
    "Morocco" : [
        { "provider" : "IAM",				"username" : "",	 "password" : "",	"APN" : [ "www.iamgprs1.ma" ] },
        { "provider" : "Meditel",			"username" : "",	 "password" : "",	"APN" : [ "internet1.meditel.ma", "mms.meditel.ma" ] }
    ],
    "Netherlands" : [
        { "provider" : "KPN",				"username" : "",	 "password" : "",	"APN" : [ "internet", "portalmmm.nl" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms", "smartsites.t-mobile" ] },
        { "provider" : "Tele2",				"username" : "",	 "password" : "",	"APN" : [ "internet.tele2.nl" ] },
        { "provider" : "Telfort",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "live.vodafone.com", "office.vodafone.nl" ] }
    ],
    "New Caledonia" : [
        { "provider" : "KNC",				"username" : "",	 "password" : "",	"APN" : [ "3g",  "internet", "wap" ] }
    ],
    "New Zealand" : [
        { "provider" : "2 degrees",			"username" : "",	 "password" : "",	"APN" : [ "2degrees", "internet" ] },
        { "provider" : "Spark",             "username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Telecom NZ",		"username" : "",	 "password" : "",	"APN" : [ "internet.telecom.co.nz", "wap.telecom.co.nz" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "direct.vodafone.net.nz", "live.vodafone.com", "www.vodafone.net.nz" ] },
        { "provider" : "XT Mobile",			"username" : "",	 "password" : "",	"APN" : [ "internet.telecom.co.nz", "wap.telecom.co.nz" ] }
    ],
    "Nigeria" : [
        { "provider" : "Etisalat NG",		"username" : "",	 "password" : "",	"APN" : [ "etisalat" ] },
        { "provider" : "Glo",				"username" : "",	 "password" : "",	"APN" : [ "glo3gvideo",  "glomms" ] },
        { "provider" : "MTN",				"username" : "",	 "password" : "",	"APN" : [ "web.gprs.mtnnigeria.net" ] },
        { "provider" : "Zain",				"username" : "",	 "password" : "",	"APN" : [ "internet.ng.zain.com" ] }
    ],
    "Norway" : [
        { "provider" : "Chess",				"username" : "",	 "password" : "",	"APN" : [ "netcom", "mms.netcom.no" ] },
        { "provider" : "Ludo Internet",		"username" : "",	 "password" : "",	"APN" : [ "internet.ventelo.no", "mms.ventelo.no" ] },
        { "provider" : "Mobitalk",			"username" : "",	 "password" : "",	"APN" : [ "telenor" ] },
        { "provider" : "Netcom",			"username" : "",	 "password" : "",	"APN" : [ "internet.netcom.no" ] },
        { "provider" : "NwN",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Talkmore",			"username" : "",	 "password" : "",	"APN" : [ "telenor" ] },
        { "provider" : "Tele2",				"username" : "",	 "password" : "",	"APN" : [ "internet.tele2.no", "mms.tele2.no" ] },
        { "provider" : "Telenor",			"username" : "",	 "password" : "",	"APN" : [ "mms" ] }
    ],
    "Oman" : [
        { "provider" : "Nawras",			"username" : "",	 "password" : "",	"APN" : [ "isp.nawras.com.om" ] },
        { "provider" : "Oman Mobile",		"username" : "",	 "password" : "",	"APN" : [ "wap", "mms" ] }
    ],
    "Pakistan" : [
        { "provider" : "Mobilink",			"username" : "",	 "password" : "",	"APN" : [ "connect.mobilinkworld.com", "mms.mobilinkworld.com" ] },
        { "provider" : "Telenor",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Ufone",				"username" : "",	 "password" : "",	"APN" : [ "ufone.mms", "ufone.pinternet" ] },
        { "provider" : "Warid",				"username" : "",	 "password" : "",	"APN" : [ "mms.warid" ] },
        { "provider" : "Zong",				"username" : "",	 "password" : "",	"APN" : [ "zonginternet", "zongmms" ] }
    ],
    "Panama" : [
        { "provider" : "Cable and Wireless","username" : "",	 "password" : "",	"APN" : [ "apn01.cwpanama.com.pa" ] }
    ],
    "Philippines" : [
        { "provider" : "GLOBE",				"username" : "",	 "password" : "",	"APN" : [ "internet.globe.com.ph", "real.globe.com.ph" ] },
        { "provider" : "SMART",				"username" : "",	 "password" : "",	"APN" : [ "internet",  "mms" ] },
        { "provider" : "SUN",				"username" : "",	 "password" : "",	"APN" : [ "minternet" ] }
    ],
    "Poland" : [
        { "provider" : "ASTER",				"username" : "",	 "password" : "",	"APN" : [ "aster.internet", "aster.mms" ] },
        { "provider" : "Carrefour",			"username" : "",	 "password" : "",	"APN" : [ "www.mova.pl" ] },
        { "provider" : "Cyfrowy Polsat",	"username" : "",	 "password" : "",	"APN" : [ "internet.cyfrowypolsat.pl" ] },
        { "provider" : "Era",				"username" : "",	 "password" : "",	"APN" : [ "erainternet", "eramms" ] },
        { "provider" : "FM Group Mobile",	"username" : "",	 "password" : "",	"APN" : [ "www.fmgmobie.pl", "mms.fmgmobile.pl" ] },
        { "provider" : "GaduAIR",			"username" : "",	 "password" : "",	"APN" : [ "internet.gadu-gadu.pl", "mms.gadu-gadu.pl" ] },
        { "provider" : "Heyah",				"username" : "",	 "password" : "",	"APN" : [ "heyah.pl", "heyahmms" ] },
        { "provider" : "MNI",				"username" : "",	 "password" : "",	"APN" : [ "mni.internet", "mni.mms" ] },
        { "provider" : "Mobilking",			"username" : "",	 "password" : "",	"APN" : [ "wapMOBILKING", "mmsMOBILKING" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Play",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Plus",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms", "mms.plusgsm.pl" ] },
        { "provider" : "Tak-Tak",			"username" : "",	 "password" : "",	"APN" : [ "erainternettt" ] },
        { "provider" : "mBank mobile",		"username" : "",	 "password" : "",	"APN" : [ "www.mobile.pl", "mms.mobile.pl" ] }
    ],
    "Portugal" : [
        { "provider" : "Optimus",			"username" : "",	 "password" : "",	"APN" : [ "mms", "umts" ] },
        { "provider" : "TMN",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "net2.vodafone.pt", "vas.vodafone.pt" ] }
    ],
    "Puerto Rico" : [
        { "provider" : "Claro",				"username" : "",	 "password" : "",	"APN" : [ "internet.claropr.com" ] },
        { "provider" : "Videotron",			"username" : "",	 "password" : "",	"APN" : [ "media.videotron" ] }
    ],
    "Qatar" : [
        { "provider" : "Q-tel",				"username" : "",	 "password" : "",	"APN" : [ "web.qtel", "mms.qtel" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "vodafone.com.qa" ] }
    ],
    "Réunion" : [
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "orangerun.acte" ] }
    ],
    "Romania" : [
        { "provider" : "Cosmote",			"username" : "",	 "password" : "",	"APN" : [ "broadband", "internet", "mms", "wnw" ] },
        { "provider" : "Digi.Mobil",		"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "internet.vodafone.ro", "live.vodafone.com", "live.pre.vodafone.com", "mms.vodafone.ro",  ] }
    ],
    "Russian Federation" : [
        { "provider" : "BWC",				"username" : "",	 "password" : "",	"APN" : [ "inet.bwc.ru", "mms.bwc.ru" ] },
        { "provider" : "BeeLine",			"username" : "",	 "password" : "",	"APN" : [ "internet.beeline.ru", "mms.beeline.ru" ] },
        { "provider" : "ETK",				"username" : "",	 "password" : "",	"APN" : [ "internet.etk.ru", "mms.etk.ru" ] },
        { "provider" : "MMS",				"username" : "",	 "password" : "",	"APN" : [ "mms.ntc" ] },
        { "provider" : "MTS",				"username" : "",	 "password" : "",	"APN" : [ "internet.mts.ru", "mms.mts.ru" ] },
        { "provider" : "Megafon",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "NCC",				"username" : "",	 "password" : "",	"APN" : [ "mms" ] },
        { "provider" : "Rostelecom",		"username" : "",	 "password" : "",	"APN" : [ "internet.usi.ru", "mms.usi.ru" ] },
        { "provider" : "Smarts",			"username" : "",	 "password" : "",	"APN" : [ "internet.smarts.ru", "mms.smarts.ru" ] },
        { "provider" : "Tele2",				"username" : "",	 "password" : "",	"APN" : [ "internet.tele2.ru", "mms.tele2.ru" ] }
    ],
    "Saint Vincent and the Grenadines" : [
        { "provider" : "LIME",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] }
    ],
    "Saudi Arabia" : [
        { "provider" : "AL JA WAL",			"username" : "",	 "password" : "",	"APN" : [ "jawalnet.com.sa", "mms.net.sa" ] },
        { "provider" : "Mobily",			"username" : "",	 "password" : "",	"APN" : [ "mms1", "web1" ] }
    ],
    "Serbia" : [
        { "provider" : "MTS",				"username" : "",	 "password" : "",	"APN" : [ "gprswap", "mms" ] },
        { "provider" : "Telenor",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "VIP",				"username" : "",	 "password" : "",	"APN" : [ "vipmobile.mms" ] }
    ],
    "Singapore" : [
        { "provider" : "Gee!",				"username" : "",	 "password" : "",	"APN" : [ "shmms", "shwap" ] },
        { "provider" : "IDEAS",				"username" : "",	 "password" : "",	"APN" : [ "e-ideas" ] },
        { "provider" : "M1",				"username" : "",	 "password" : "",	"APN" : [ "Miworld", "sunsurf" ] },
        { "provider" : "SingTel",			"username" : "",	 "password" : "",	"APN" : [ "e-ideas", "m2minternet" ] },
        { "provider" : "StarHub",			"username" : "",	 "password" : "",	"APN" : [ "internet", "SHM2M" ] },
        { "provider" : "Sunsurf Internet",	"username" : "",	 "password" : "",	"APN" : [ "sunsurf" ] }
    ],
    "Slovakia" : [
        { "provider" : "O2",				"username" : "",	 "password" : "",	"APN" : [ "o2internet" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] }
    ],
    "South Africa" : [
        { "provider" : "Cell C",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "MTN",				"username" : "",	 "password" : "",	"APN" : [ "internet", "myMTN" ] },
        { "provider" : "Virgin Mobile",		"username" : "",	 "password" : "",	"APN" : [ "vdata", "vmms" ] },
        { "provider" : "Vlive!",			"username" : "",	 "password" : "",	"APN" : [ "vlive" ] },
        { "provider" : "Vodacom",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms.vodacom.net" ] }
    ],
    "Korea, Republic of" : [
        { "provider" : "SK Telecom",		"username" : "",	 "password" : "",	"APN" : [ "roaming.sktelecom.com", "web.sktelecom.com" ] },
        { "provider" : "Softbank",			"username" : "",	 "password" : "",	"APN" : [ "andglobal.softbank.ne.jp" ] }
    ],
    "Spain" : [
        { "provider" : "Blau",				"username" : "",	 "password" : "",	"APN" : [ "gprs-service.com" ] },
        { "provider" : "Jazztel",			"username" : "",	 "password" : "",	"APN" : [ "jazzinternet" ] },
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "movistar.es", "telefonica.es" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "internet", "orangemms", "orangeworld" ] },
        { "provider" : "Pepephone",			"username" : "",	 "password" : "",	"APN" : [ "gprsmov.pepephone.com", "mms.pepephone.com" ] },
        { "provider" : "Simyo",				"username" : "",	 "password" : "",	"APN" : [ "gprs-service.com" ] },
        { "provider" : "Tuenti",			"username" : "",	 "password" : "",	"APN" : [ "tuenti.com" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "ac.vodafone.es",  "airtelnet.es", "airtelwap.es", "mms.vodafone.net",  ] },
        { "provider" : "Yoigo",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] }
    ],
    "Sweden" : [
        { "provider" : "3",					"username" : "",	 "password" : "",	"APN" : [ "data.tre.se" ] },
        { "provider" : "Halebop",			"username" : "",	 "password" : "",	"APN" : [ "halebop.telia.se", "mms.telia.se" ] },
        { "provider" : "Spring data",		"username" : "",	 "password" : "",	"APN" : [ "data.springmobil.se" ] },
        { "provider" : "Tele2",				"username" : "",	 "password" : "",	"APN" : [ "internet.tele2.se" ] },
        { "provider" : "Telenor",			"username" : "",	 "password" : "",	"APN" : [ "internet.telenor.se", "services.telenor.se" ] },
        { "provider" : "Telia SE",			"username" : "",	 "password" : "",	"APN" : [ "online.telia.se", "mms.telia.se" ] },
        { "provider" : "Universal",			"username" : "",	 "password" : "",	"APN" : [ "sp-services" ] }
    ],
    "Switzerland" : [
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Sunrise",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms.sunrise.ch" ] },
        { "provider" : "Swisscom",			"username" : "",	 "password" : "",	"APN" : [ "event.swisscom.ch", "gprs.swisscom.ch" ] }
    ],
    "Syrian Arab Republic" : [
        { "provider" : "MTN",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Syriatel",			"username" : "",	 "password" : "",	"APN" : [ "net.syriatel.com", "mms.syriatel.com" ] }
    ],
    "Taiwan, Province of China" : [
        { "provider" : "Aurora",			"username" : "",	 "password" : "",	"APN" : [ "aurorawap", "auroraweb" ] },
        { "provider" : "Chunghwa",			"username" : "",	 "password" : "",	"APN" : [ "emome" ] },
        { "provider" : "CHT",				"username" : "",	 "password" : "",	"APN" : [ "emome", "internet" ] },
        { "provider" : "FarEasTone",		"username" : "",	 "password" : "",	"APN" : [ "fetnet01", "internet" ] },
        { "provider" : "KGT",				"username" : "",	 "password" : "",	"APN" : [ "internet", "kgtmms" ] },
        { "provider" : "Mobitai",			"username" : "",	 "password" : "",	"APN" : [ "gprs1", "internet" ] },
        { "provider" : "Taiwan Mobile",		"username" : "",	 "password" : "",	"APN" : [ "internet", "mms", "twm" ] },
        { "provider" : "TransAsia",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Vibo",				"username" : "",	 "password" : "",	"APN" : [ "internet", "vibo" ] }
    ],
    "Thailand" : [
        { "provider" : "AIS",				"username" : "",	 "password" : "",	"APN" : [ "internet", "multimedia" ] },
        { "provider" : "DTAC",				"username" : "",	 "password" : "",	"APN" : [ "mms", "www.dtac.co.th" ] },
        { "provider" : "TOT",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "True",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "TrueMove-H",		"username" : "",	 "password" : "",	"APN" : [ "internet", "hmms" ] }
    ],
    "Turkey" : [
        { "provider" : "AVEA",				"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Turkcell",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] }
    ],
    "Uganda" : [
        { "provider" : "Airtel",			"username" : "",	 "password" : "",	"APN" : [ "internet", "web.ug.zain.com" ] },
        { "provider" : "MTN",				"username" : "",	 "password" : "",	"APN" : [ "yellopix.mtn.co.ug" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "orange.ug" ] },
        { "provider" : "Uganda Telecom",	"username" : "",	 "password" : "",	"APN" : [ "utweb" ] }
    ],
    "Ukraine" : [
        { "provider" : "Ace_Base",			"username" : "",	 "password" : "",	"APN" : [ "www.ab.kyivstar.net", "mms.kyivstar.net" ] },
        { "provider" : "Beeline",			"username" : "",	 "password" : "",	"APN" : [ "internet.beeline.ua", "mms.beeline.ua" ] },
        { "provider" : "Djuice",			"username" : "",	 "password" : "",	"APN" : [ "www.djuice.com.ua" ] },
        { "provider" : "JEANS",				"username" : "",	 "password" : "",	"APN" : [ "mms.jeans.ua", "www.jeans.ua" ] },
        { "provider" : "Kyivstar",			"username" : "",	 "password" : "",	"APN" : [ "www.kyivstar.net" ] },
        { "provider" : "MTS",				"username" : "",	 "password" : "",	"APN" : [ "active", "hyper.net", "internet", "mms", "mms.umc.ua", "www.umc.ua", "www.mts.com.ua" ] },
        { "provider" : "life",				"username" : "",	 "password" : "",	"APN" : [ "internet", "mms" ] }
    ],
    "United Arab Emirates" : [
        { "provider" : "Etisalat UAE",		"username" : "",	 "password" : "",	"APN" : [ "etisalat.ae", "mms" ] }
    ],
    "United Kingdom" : [
        { "provider" : "3",					"username" : "",	 "password" : "",	"APN" : [ "3gpronto", "three.co.uk" ] },
        { "provider" : "British Telecom",	"username" : "",	 "password" : "",	"APN" : [ "btmobile.bt.com" ] },
        { "provider" : "Giffgaff",			"username" : "",	 "password" : "",	"APN" : [ "giffgaff.com" ] },
        { "provider" : "Jersey",			"username" : "",	 "password" : "",	"APN" : [ "pepper", "mms" ] },
        { "provider" : "Lebara",			"username" : "",	 "password" : "",	"APN" : [ "uk.lebara.mobi" ] },
        { "provider" : "Manx Telecom",		"username" : "",	 "password" : "",	"APN" : [ "3gpronto", "mms.manxpronto.net", "mms.prontogo.net" ] },
        { "provider" : "O2",				"username" : "",	 "password" : "",	"APN" : [ "idata.o2.co.uk", "mobile.o2.co.uk", "payandgo.o2.co.uk", "wap.o2.co.uk" ] },
        { "provider" : "Orange",			"username" : "",	 "password" : "",	"APN" : [ "orangeinternet", "orangemms" ] },
        { "provider" : "Sure Mobile",		"username" : "",	 "password" : "",	"APN" : [ "internet", "MMS", "mms" ] },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "general.t-mobile.uk" ] },
        { "provider" : "Talkmobile",		"username" : "",	 "password" : "",	"APN" : [ "payg.talkmobile.co.uk", "talkmobile.co.uk" ] },
        { "provider" : "UBIQUISYS",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Virgin",			"username" : "",	 "password" : "",	"APN" : [ "goto.virginmobile.uk" ] },
        { "provider" : "Vodafone",			"username" : "",	 "password" : "",	"APN" : [ "internet", "pp.vodafone.co.uk", "wap.vodafone.co.uk" ] }
    ],
    "United States" : [
        { "provider" : "AT&T",				"username" : "",	 "password" : "",	"APN" : [ "broadband", "wap.cingular" ],             "carrier" : "att" },
        { "provider" : "Sprint",			"username" : "",	 "password" : "",	"APN" : [], "fixedAPN" : "none",                     "carrier" : "sprint",  "gobiFirmware" : "2", "msl" : true },
        { "provider" : "T-Mobile",			"username" : "",	 "password" : "",	"APN" : [ "epc.tmobile.com" ] },
        { "provider" : "Verizon",			"username" : "",	 "password" : "",	"APN" : [ {"text": "VZWINTERNET", "value":"none"} ], "carrier" : "verizon", "gobiFirmware" : "1" }
    ],
    "Uruguay" : [
        { "provider" : "Ancel",				"username" : "",	 "password" : "",	"APN" : [ "ancel", "mms" ] },
        { "provider" : "Antel",				"username" : "",	 "password" : "",	"APN" : [ "wap" ] },
        { "provider" : "BAM Pepaga Antel",	"username" : "",	 "password" : "",	"APN" : [ "prepago.ancel" ] },
        { "provider" : "Claro",				"username" : "",	 "password" : "",	"APN" : [ "gprs.claro.com.uy", "mms.ctimovil.com.uy" ] },
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "apnumt.movistar.com.uy", "apnmms.movistar.com.uy" ] }
    ],
    "Venezuela, Bolivarian Republic of" : [
        { "provider" : "Digitel",			"username" : "",	 "password" : "",	"APN" : [ "expresate.digitel.ve", "gprsweb.digitel.ve" ] },
        { "provider" : "Movilnet",			"username" : "",	 "password" : "",	"APN" : [ "int.movilnet.com.ve" ] },
        { "provider" : "Movistar",			"username" : "",	 "password" : "",	"APN" : [ "internet.movistar.ve" ] }
    ],
    "Viet Nam" : [
        { "provider" : "BeeLine",			"username" : "",	 "password" : "",	"APN" : [ "internet" ] },
        { "provider" : "Mobifone",			"username" : "",	 "password" : "",	"APN" : [ "m-i090", "m-wap" ] },
        { "provider" : "Vietnam Mobile",	"username" : "",	 "password" : "",	"APN" : [ "mms", "wap" ] },
        { "provider" : "Viettel",			"username" : "",	 "password" : "",	"APN" : [ "v-internet", "v-mms" ] },
        { "provider" : "Vinaphone",			"username" : "",	 "password" : "",	"APN" : [ "m3-world", "m3-mms" ] }
    ]
};



