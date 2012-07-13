#include "geoip.h"
#include <QFile>
#include <QFileInfo>
#include "util.h"

using namespace util;
QMutex GeoIP::mutex;

GeoIP::GeoIP(QString& filename, QObject *parent):
    QObject(parent)
{
    mutex.lock();
    FileInput.setFileName(filename);

    try
    {
        FileInput.open(QIODevice::ReadOnly);
    }
    catch (...) { }
    mutex.unlock();
}

GeoIP::~GeoIP()
{
    mutex.lock();
    try
    {
        FileInput.close();
    }
    catch (...) { }
    mutex.unlock();
}

QString GeoIP::lookupCountryCode(QString str)
{
    QHostAddress addr(str);
    return lookupCountryCode(addr);
}

QString GeoIP::lookupCountryCode(QHostAddress addr)
{
    static const char *countryCode[] = { "--","AP","EU","AD","AE","AF","AG","AI","AL","AM","CW","AO","AQ","AR","AS","AT","AU","AW","AZ","BA","BB","BD","BE","BF","BG","BH","BI","BJ","BM","BN","BO","BR","BS","BT","BV","BW","BY","BZ","CA","CC","CD","CF","CG","CH","CI","CK","CL","CM","CN","CO","CR","CU","CV","CX","CY","CZ","DE","DJ","DK","DM","DO","DZ",
                                    "EC","EE","EG","EH","ER","ES","ET","FI","FJ","FK","FM","FO","FR","FX","GA","GB","GD","GE","GF","GH","GI","GL","GM","GN","GP","GQ","GR","GS","GT","GU","GW","GY","HK","HM","HN","HR","HT","HU","ID","IE","IL","IN","IO","IQ","IR","IS","IT","JM","JO","JP","KE","KG","KH","KI","KM","KN","KP","KR","KW","KY","KZ",
                                    "LA","LB","LC","LI","LK","LR","LS","LT","LU","LV","LY","MA","MC","MD","MG","MH","MK","ML","MM","MN","MO","MP","MQ","MR","MS","MT","MU","MV","MW","MX","MY","MZ","NA","NC","NE","NF","NG","NI","NL","NO","NP","NR","NU","NZ","OM","PA","PE","PF","PG","PH","PK","PL","PM","PN","PR","PS","PT","PW","PY","QA",
                                    "RE","RO","RU","RW","SA","SB","SC","SD","SE","SG","SH","SI","SJ","SK","SL","SM","SN","SO","SR","ST","SV","SY","SZ","TC","TD","TF","TG","TH","TJ","TK","TM","TN","TO","TL","TR","TT","TV","TW","TZ","UA","UG","UM","US","UY","UZ","VA","VC","VE","VG","VI","VN","VU","WF","WS","YE","YT","RS","ZA","ZM","ME","ZW","A1","A2",
                                         "O1","AX","GG","IM","JE","BL","MF", "BW"
                                       };
    static const quint32 sizeCountryCode = sizeof(countryCode) / sizeof(countryCode[0]);
    quint32 countryPos = seekCountry(0, addr.toIPv4Address(), 31);
    if (countryPos >= sizeCountryCode)
        countryPos = 0;
    return(countryCode[countryPos]);
}

QString GeoIP::lookupCountryName(QString str)
{
    QHostAddress addr(str);
    return lookupCountryName(addr);
}

QString GeoIP::lookupCountryName(QHostAddress addr)
{
    static const char *countryName[] =  {QT_TR_NOOP("N/A"),QT_TR_NOOP("Asia/Pacific Region"),QT_TR_NOOP("Europe"),QT_TR_NOOP("Andorra"),QT_TR_NOOP("United Arab Emirates"),QT_TR_NOOP("Afghanistan"),QT_TR_NOOP("Antigua and Barbuda"),QT_TR_NOOP("Anguilla"),QT_TR_NOOP("Albania"),QT_TR_NOOP("Armenia"),QT_TR_NOOP("Curacao"),QT_TR_NOOP("Angola"),QT_TR_NOOP("Antarctica"),QT_TR_NOOP("Argentina"),QT_TR_NOOP("American Samoa"),QT_TR_NOOP("Austria"),QT_TR_NOOP("Australia"),QT_TR_NOOP("Aruba"),QT_TR_NOOP("Azerbaijan"),QT_TR_NOOP("Bosnia and Herzegovina"),QT_TR_NOOP("Barbados"),QT_TR_NOOP("Bangladesh"),QT_TR_NOOP("Belgium"),
                                    QT_TR_NOOP("Burkina Faso"),QT_TR_NOOP("Bulgaria"),QT_TR_NOOP("Bahrain"),QT_TR_NOOP("Burundi"),QT_TR_NOOP("Benin"),QT_TR_NOOP("Bermuda"),QT_TR_NOOP("Brunei Darussalam"),QT_TR_NOOP("Bolivia"),QT_TR_NOOP("Brazil"),QT_TR_NOOP("Bahamas"),QT_TR_NOOP("Bhutan"),QT_TR_NOOP("Bouvet Island"),QT_TR_NOOP("Botswana"),QT_TR_NOOP("Belarus"),QT_TR_NOOP("Belize"),QT_TR_NOOP("Canada"),QT_TR_NOOP("Cocos (Keeling) Islands"),QT_TR_NOOP("Congo, The Democratic Republic of the"),QT_TR_NOOP("Central African Republic"),QT_TR_NOOP("Congo"),QT_TR_NOOP("Switzerland"),QT_TR_NOOP("Cote D'Ivoire"),
                                    QT_TR_NOOP("Cook Islands"),QT_TR_NOOP("Chile"),QT_TR_NOOP("Cameroon"),QT_TR_NOOP("China"),QT_TR_NOOP("Colombia"),QT_TR_NOOP("Costa Rica"),QT_TR_NOOP("Cuba"),QT_TR_NOOP("Cape Verde"),QT_TR_NOOP("Christmas Island"),QT_TR_NOOP("Cyprus"),QT_TR_NOOP("Czech Republic"),QT_TR_NOOP("Germany"),QT_TR_NOOP("Djibouti"),QT_TR_NOOP("Denmark"),QT_TR_NOOP("Dominica"),QT_TR_NOOP("Dominican Republic"),QT_TR_NOOP("Algeria"),QT_TR_NOOP("Ecuador"),QT_TR_NOOP("Estonia"),QT_TR_NOOP("Egypt"),QT_TR_NOOP("Western Sahara"),QT_TR_NOOP("Eritrea"),QT_TR_NOOP("Spain"),QT_TR_NOOP("Ethiopia"),QT_TR_NOOP("Finland"),QT_TR_NOOP("Fiji"),QT_TR_NOOP("Falkland Islands (Malvinas)"),
                                    QT_TR_NOOP("Micronesia, Federated States of"),QT_TR_NOOP("Faroe Islands"),QT_TR_NOOP("France"),QT_TR_NOOP("France, Metropolitan"),QT_TR_NOOP("Gabon"),QT_TR_NOOP("United Kingdom"),QT_TR_NOOP("Grenada"),QT_TR_NOOP("Georgia"),QT_TR_NOOP("French Guiana"),QT_TR_NOOP("Ghana"),QT_TR_NOOP("Gibraltar"),QT_TR_NOOP("Greenland"),QT_TR_NOOP("Gambia"),QT_TR_NOOP("Guinea"),QT_TR_NOOP("Guadeloupe"),QT_TR_NOOP("Equatorial Guinea"),QT_TR_NOOP("Greece"),QT_TR_NOOP("South Georgia and the South Sandwich Islands"),QT_TR_NOOP("Guatemala"),QT_TR_NOOP("Guam"),QT_TR_NOOP("Guinea-Bissau"),QT_TR_NOOP("Guyana"),
                                    QT_TR_NOOP("Hong Kong"),QT_TR_NOOP("Heard Island and McDonald Islands"),QT_TR_NOOP("Honduras"),QT_TR_NOOP("Croatia"),QT_TR_NOOP("Haiti"),QT_TR_NOOP("Hungary"),QT_TR_NOOP("Indonesia"),QT_TR_NOOP("Ireland"),QT_TR_NOOP("Israel"),QT_TR_NOOP("India"),QT_TR_NOOP("British Indian Ocean Territory"),QT_TR_NOOP("Iraq"),QT_TR_NOOP("Iran, Islamic Republic of"),QT_TR_NOOP("Iceland"),QT_TR_NOOP("Italy"),QT_TR_NOOP("Jamaica"),QT_TR_NOOP("Jordan"),QT_TR_NOOP("Japan"),QT_TR_NOOP("Kenya"),QT_TR_NOOP("Kyrgyzstan"),QT_TR_NOOP("Cambodia"),QT_TR_NOOP("Kiribati"),QT_TR_NOOP("Comoros"),QT_TR_NOOP("Saint Kitts and Nevis"),
                                    QT_TR_NOOP("Korea, Democratic People's Republic of"),QT_TR_NOOP("Korea, Republic of"),QT_TR_NOOP("Kuwait"),QT_TR_NOOP("Cayman Islands"),QT_TR_NOOP("Kazakstan"),QT_TR_NOOP("Lao People's Democratic Republic"),QT_TR_NOOP("Lebanon"),QT_TR_NOOP("Saint Lucia"),QT_TR_NOOP("Liechtenstein"),QT_TR_NOOP("Sri Lanka"),QT_TR_NOOP("Liberia"),QT_TR_NOOP("Lesotho"),QT_TR_NOOP("Lithuania"),QT_TR_NOOP("Luxembourg"),QT_TR_NOOP("Latvia"),QT_TR_NOOP("Libyan Arab Jamahiriya"),QT_TR_NOOP("Morocco"),QT_TR_NOOP("Monaco"),QT_TR_NOOP("Moldova, Republic of"),QT_TR_NOOP("Madagascar"),
                                    QT_TR_NOOP("Marshall Islands"),QT_TR_NOOP("Macedonia"),QT_TR_NOOP("Mali"),QT_TR_NOOP("Myanmar"),QT_TR_NOOP("Mongolia"),QT_TR_NOOP("Macau"),QT_TR_NOOP("Northern Mariana Islands"),QT_TR_NOOP("Martinique"),QT_TR_NOOP("Mauritania"),QT_TR_NOOP("Montserrat"),QT_TR_NOOP("Malta"),QT_TR_NOOP("Mauritius"),QT_TR_NOOP("Maldives"),QT_TR_NOOP("Malawi"),QT_TR_NOOP("Mexico"),QT_TR_NOOP("Malaysia"),QT_TR_NOOP("Mozambique"),QT_TR_NOOP("Namibia"),QT_TR_NOOP("New Caledonia"),QT_TR_NOOP("Niger"),QT_TR_NOOP("Norfolk Island"),QT_TR_NOOP("Nigeria"),QT_TR_NOOP("Nicaragua"),QT_TR_NOOP("Netherlands"),
                                    QT_TR_NOOP("Norway"),QT_TR_NOOP("Nepal"),QT_TR_NOOP("Nauru"),QT_TR_NOOP("Niue"),QT_TR_NOOP("New Zealand"),QT_TR_NOOP("Oman"),QT_TR_NOOP("Panama"),QT_TR_NOOP("Peru"),QT_TR_NOOP("French Polynesia"),QT_TR_NOOP("Papua New Guinea"),QT_TR_NOOP("Philippines"),QT_TR_NOOP("Pakistan"),QT_TR_NOOP("Poland"),QT_TR_NOOP("Saint Pierre and Miquelon"),QT_TR_NOOP("Pitcairn Islands"),QT_TR_NOOP("Puerto Rico"),QT_TR_NOOP("Palestinian Territory"),QT_TR_NOOP("Portugal"),QT_TR_NOOP("Palau"),QT_TR_NOOP("Paraguay"),QT_TR_NOOP("Qatar"),QT_TR_NOOP("Reunion"),QT_TR_NOOP("Romania"),QT_TR_NOOP("Russian Federation"),QT_TR_NOOP("Rwanda"),QT_TR_NOOP("Saudi Arabia"),
                                    QT_TR_NOOP("Solomon Islands"),QT_TR_NOOP("Seychelles"),QT_TR_NOOP("Sudan"),QT_TR_NOOP("Sweden"),QT_TR_NOOP("Singapore"),QT_TR_NOOP("Saint Helena"),QT_TR_NOOP("Slovenia"),QT_TR_NOOP("Svalbard and Jan Mayen"),QT_TR_NOOP("Slovakia"),QT_TR_NOOP("Sierra Leone"),QT_TR_NOOP("San Marino"),QT_TR_NOOP("Senegal"),QT_TR_NOOP("Somalia"),QT_TR_NOOP("Suriname"),QT_TR_NOOP("Sao Tome and Principe"),QT_TR_NOOP("El Salvador"),QT_TR_NOOP("Syrian Arab Republic"),QT_TR_NOOP("Swaziland"),QT_TR_NOOP("Turks and Caicos Islands"),QT_TR_NOOP("Chad"),QT_TR_NOOP("French Southern Territories"),QT_TR_NOOP("Togo"),
                                    QT_TR_NOOP("Thailand"),QT_TR_NOOP("Tajikistan"),QT_TR_NOOP("Tokelau"),QT_TR_NOOP("Turkmenistan"),QT_TR_NOOP("Tunisia"),QT_TR_NOOP("Tonga"),QT_TR_NOOP("Timor-Leste"),QT_TR_NOOP("Turkey"),QT_TR_NOOP("Trinidad and Tobago"),QT_TR_NOOP("Tuvalu"),QT_TR_NOOP("Taiwan"),QT_TR_NOOP("Tanzania, United Republic of"),QT_TR_NOOP("Ukraine"),QT_TR_NOOP("Uganda"),QT_TR_NOOP("United States Minor Outlying Islands"),QT_TR_NOOP("United States"),QT_TR_NOOP("Uruguay"),QT_TR_NOOP("Uzbekistan"),QT_TR_NOOP("Holy See (Vatican City State)"),QT_TR_NOOP("Saint Vincent and the Grenadines"),
                                    QT_TR_NOOP("Venezuela"),QT_TR_NOOP("Virgin Islands, British"),QT_TR_NOOP("Virgin Islands, U.S."),QT_TR_NOOP("Vietnam"),QT_TR_NOOP("Vanuatu"),QT_TR_NOOP("Wallis and Futuna"),QT_TR_NOOP("Samoa"),QT_TR_NOOP("Yemen"),QT_TR_NOOP("Mayotte"),QT_TR_NOOP("Serbia"),QT_TR_NOOP("South Africa"),QT_TR_NOOP("Zambia"),QT_TR_NOOP("Montenegro"),QT_TR_NOOP("Zimbabwe"),QT_TR_NOOP("Anonymous Proxy"),QT_TR_NOOP("Satellite Provider"),
                                    QT_TR_NOOP("Other"),QT_TR_NOOP("Aland Islands"),QT_TR_NOOP("Guernsey"),QT_TR_NOOP("Isle of Man"),QT_TR_NOOP("Jersey"),QT_TR_NOOP("Saint Barthelemy"),QT_TR_NOOP("Saint Martin"), QT_TR_NOOP("Bonaire, Saint Eustatius and Saba")
                                   };
    static const quint32 sizeCountryName = sizeof(countryName) / sizeof(countryName[0]);
    quint32 countryPos = seekCountry(0, addr.toIPv4Address(), 31);
    if (countryPos >= sizeCountryName)
        countryPos = 0;
    return(tr(countryName[countryPos]));
}

quint32 GeoIP::seekCountry(quint32 offset, quint32 ipnum, int depth)
{
    QByteArray buf;
    quint32 x[2];

    if (depth < 0)
    {
        return 0; // N/A
    }

    try
    {
        mutex.lock();
        FileInput.seek(6 * offset);
        buf = FileInput.read(6);
    }
    catch (...)
    {
        mutex.unlock();
        return 0;
    }
    mutex.unlock();
    for (int i = 0; i<2; i++)
    {
        x[i] = 0;
        for (int j = 0; j<3; j++)
        {
            int y = buf[i*3+j];
            if (y < 0)
            {
                y+= 256;
            }
            x[i] += (y << (j * 8));
        }
    }

    if ((ipnum & (1 << depth)) > 0)
    {
        if (x[1] >= COUNTRY_BEGIN)
        {
            return x[1] - COUNTRY_BEGIN;
        }
        return seekCountry(x[1], ipnum, depth-1);
    }
    else
    {
        if (x[0] >= COUNTRY_BEGIN)
        {
            return x[0] - COUNTRY_BEGIN;
        }
        return seekCountry(x[0], ipnum, depth-1);
    }
}

bool GeoIP::update(const QByteArray &content)
{
    mutex.lock();
    QFile file(util::getHomePath() + "geoip.dat");
    if (!file.open(QIODevice::WriteOnly))
    {
        mutex.unlock();
        return false;
    }
    file.write(util::gzipDecompress(content));
    file.close();
    mutex.unlock();
    return true;
}
