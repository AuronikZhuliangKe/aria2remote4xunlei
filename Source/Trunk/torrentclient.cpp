#include "torrentclient.h"
#include <QByteArray>

TorrentClient::TorrentClient()
{
}

QString TorrentClient::shChar( QChar ch )
{
    static const QString codes("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.-");
    int loc = codes.indexOf(ch);

    if (loc < 0) loc = 0;
    return QString("%1").arg(loc);
}

QString TorrentClient::getMnemonicEnd( char ch )
{
    switch( ch )
    {
        case 'b': case 'B': return QString(" (Beta)");
        case 'd': return QString(" (Debug)");
        case 'x': case 'X': case 'Z': return QString(" (Dev)");
    }
    return QString("");
}



QString TorrentClient::getClientName(const QString &origStr)
{
    static bool bFirst = true;
    if (bFirst)
    {
        azLikeClients4["AR"] = "Ares", azLikeClients4["AT"] = "Artemis", azLikeClients4["AV"] = "Avicora",
        azLikeClients4["BG"] = "BTGetit", azLikeClients4["BM"] = "BitMagnet", azLikeClients4["BP"] = "BitTorrent Pro (Azureus + Spyware)",
        azLikeClients4["BX"] = "BittorrentX", azLikeClients4["bk"] = "BitKitten (libtorrent)", azLikeClients4["BS"] = "BTSlave",
        azLikeClients4["BW"] = "BitWombat", azLikeClients4["EB"] = "EBit",
        azLikeClients4["DE"] = "Deluge", azLikeClients4["DP"] = "Propogate Data Client", azLikeClients4["FC"] = "FileCroc",
        azLikeClients4["FT"] = "FoxTorrent/RedSwoosh", azLikeClients4["GR"] = "GetRight", azLikeClients4["HN"] = "Hydranode",
        azLikeClients4["LC"] = "LeechCraft", azLikeClients4["LH"] = "LH-ABC", azLikeClients4["NX"] = "Net Transport",
        azLikeClients4["MO"] = "MonoTorrent", azLikeClients4["MR"] = "Miro", azLikeClients4["MT"] = "Moonlight",
        azLikeClients4["OT"] = "OmegaTorrent", azLikeClients4["PD"] = "Pando", azLikeClients4["QD"] = "QQDownload",
        azLikeClients4["RS"] = "Rufus", azLikeClients4["RT"] = "Retriever", azLikeClients4["RZ"] = "RezTorrent",
        azLikeClients4["SD"] = "Xunlei", azLikeClients4["SS"] = "SwarmScope", azLikeClients4["SZ"] = "Shareaza",
        azLikeClients4["S~"] = "Shareaza beta", azLikeClients4["st"] = "SharkTorrent", azLikeClients4["TN"] = "Torrent .NET",
        azLikeClients4["TS"] = "TorrentStorm", azLikeClients4["UL"] = "uLeecher!", azLikeClients4["VG"] = "Vagaa",
        azLikeClients4["WY"] = "Wyzo", azLikeClients4["XL"] = "Xunlei",
        azLikeClients4["XT"] = "XanTorrent", azLikeClients4["ZT"] = "Zip Torrent",
        azLikeClients4["GS"] = "GSTorrent", azLikeClients4["KG"] = "KGet", azLikeClients4["ST"] = "SymTorrent",
        azLikeClients4["BE"] = "BitTorrent SDK";
        azLikeClients3["AG"] = "Ares", azLikeClients3["A~"] = "Ares", azLikeClients3["ES"] = "Electric Sheep",
        azLikeClients3["HL"] = "Halite", azLikeClients3["LT"] = "libtorrent (Rasterbar)", azLikeClients3["lt"] = "libTorrent (Rakshasa)",
        azLikeClients3["MP"] = "MooPolice", azLikeClients3["TT"] = "TuoTu", azLikeClients3["qB"] = "qBittorrent",
        azLikeClients3["MG"] = "MediaGet";
        azLikeClients2x2["AX"] = "BitPump", azLikeClients2x2["BC"] = "BitComet", azLikeClients2x2["CD"] = "Enhanced CTorrent",
        azLikeClientsSpec["UM"] = QString("%1Torrent for Mac").arg(QChar(0x00B5)), azLikeClientsSpec["UT"] = QString("%1Torrent").arg(QChar(0x00B5)),
        azLikeClientsSpec["UE"] = QString("%1Torrent for Embedded").arg(QChar(0x00B5)), azLikeClientsSpec["TR"] = "Transmission",
        azLikeClientsSpec["AZ"] = "Azureus", azLikeClientsSpec["KT"] = "KTorrent", azLikeClientsSpec["BF"] = "BitFlu",
        azLikeClientsSpec["LW"] = "LimeWire", azLikeClientsSpec["BB"] = "BitBuddy", azLikeClientsSpec["BR"] = "BitRocket",
        azLikeClientsSpec["CT"] = "CTorrent", azLikeClientsSpec["XX"] = "Xtorrent", azLikeClientsSpec["LP"] = "Lphant",
        shLikeClients["O"] = "Osprey", shLikeClients["Q"] = "BTQueue",
        shLikeClients["A"] = "ABC", shLikeClients["R"] = "Tribler", shLikeClients["S"] = "Shad0w",
        shLikeClients["T"] = "BitTornado", shLikeClients["U"] = "UPnP NAT Bit Torrent";

        bFirst = false;
    }

    QString ret;
    QString str = QString(QByteArray::fromPercentEncoding(origStr.toAscii()));

    if (str.size() == 0)
    {
        return tr("Unknown");
    }

    if (str.contains(QRegExp("-[A-Za-z~][A-Za-z~][A-Za-z0-9][A-Za-z0-9]..-")))
    {
        QString sign = str.mid(1, 2);
        QString cli = azLikeClientsSpec[sign];
        if (cli.size() != 0)
        {
            if (!sign.compare("BF") || !sign.compare("LW"))
            {
                ret = cli;
            } else if (!sign.compare("UT") || !sign.compare("UM") || !sign.compare("UE"))
            {
                ret = cli + " " + str.at(3)+ "." + str.at(4) + "." + str.at(5) + getMnemonicEnd(str.at(6).toAscii());
            } else if (!sign.compare("TR"))
            {
                if (str.mid(3, 2).compare("00") == 0)
                {
                    if (str.at(5).toAscii() == '0')
                        ret = cli + " 0." + str.at(6);
                    else
                        ret = cli + " 0." + str.mid(5, 2);
                }
                else
                {
                    char type = str.at(6).toAscii();
                    ret = cli + " " + str.mid(3, 1) + "." + str.mid(4, 2) + (((type == 'Z') || (type == 'X')) ? "+" : "");
                }
            } else if (!sign.compare("KT"))
            {
                char ch = str.at(5).toAscii();
                if ( ch == 'D' )
                    ret = cli + " " + shChar(str.at(3)) + "." + shChar(str.at(4)) + " Dev " + shChar(str.at(6));
                else
                    if ( ch == 'R' )
                        ret = cli + " " + shChar(str.at(3)) + "." + shChar(str.at(4)) + " RC " + shChar(str.at(6));
                    else
                        ret = cli + " " + shChar(str.at(3)) + "." + shChar(str.at(4)) + "." + shChar(str.at(5));
            } else if (!sign.compare("AZ"))
            {
                if (str.at(3).toAscii() > '3' || ( str.at(3).toAscii() == '3' && str.at(4).toAscii() >= '1' ))
                    cli = "Vuze";
                ret = cli + " " + shChar(str.at(3)) + "." + shChar(str.at(4)) + "." + shChar(str.at(5)) + "." + shChar(str.at(6));
            } else if (!sign.compare("BB"))
            {
                ret = cli + " " + str.at(3) + "." + str.at(4) + str.at(5) + str.at(6);
            } else if (!sign.compare("BR"))
            {
                ret = cli + " " + str.at(3) + "." + str.at(4) + " (" + str.at(5) + str.at(6) + ")";
            } else if (!sign.compare("CT"))
            {
                ret = cli + " " + shChar(str.at(3)) + "." + shChar(str.at(4)) + "." + str.mid(5, 2);
            } else if (!sign.compare("XX"))
            {
                ret = cli + " " + shChar(str.at(3)) + "." + shChar(str.at(4)) + " (" + str.mid(5, 2) + ")";
            } else if (!sign.compare("LP"))
            {
                ret = cli + " " + str.mid(3, 1) + "." + str.mid(5, 2);
            } else
            {
                char ch = str.at(6).toAscii();
                ret = cli + " " + str.at(3) + "." + str.mid(4, 2);
                if ( (ch == 'Z') || (ch == 'X') )
                    ret += '+';
            }
        }
        else
        {
            QString cli = azLikeClients4[sign];
            if (cli.size() > 0)
                ret = cli + " " + shChar(str.at(3)) + "." + shChar(str.at(4)) + "." + shChar(str.at(5)) + "." + shChar(str.at(6));
            else
            {
                cli = azLikeClients3[sign];
                if (cli.size() > 0)
                    ret = cli + " " + shChar(str.at(3)) + "." + shChar(str.at(4)) + "." + shChar(str.at(5));
                else
                {
                    cli = azLikeClients2x2[sign];
                    if (cli.size() > 0)
                        ret = QString("%1 %2.%3").arg(cli).arg(str.mid(3, 2)).arg(str.mid(5, 2));
                }
            }
        }
    }

    if (ret.size() == 0)
    {
        if (str.contains(QRegExp("[MQ]\\d-\\d[\\-\\d][\\-\\d][\\-\\d]-")))
        {
            ret = (str.at(0).toAscii() == 'Q') ? "Queen Bee " : "BitTorrent ";
            if ( (str.at(4).toAscii() == '-') && (str.at(6).toAscii() == '-') )
                ret += (str.at(1) + '.' + str.at(3) + '.' + str.at(5));
            else if ((str.at(5).toAscii() == '-'))
                    ret += (str.at(1) + '.' + str.at(3) + str.at(4) + '.' + str.at(6));
        }
        else
        {
            if (str.contains(QRegExp("-BOW")))
            {
                if (str.mid(4, 4).compare("A0B") == 0)
                    ret = "Bits on Wheels 1.0.5";
                else
                {
                    ret = "Bits on Wheels ";
                    if (str.mid(4, 4).compare("A0C") == 0)
                        ret += "1.0.6";
                    else
                        ret += (str.at(4) + '.' + str.at(5) + '.' + str.at(6));
                }
            }
            else
            {
                if (str.contains(QRegExp("AZ2500BT")))
                    ret = "BitTyrant (Azureus Mod)";
                else if (str.contains(QRegExp("-FG\\d\\d\\d\\d")))
                    ret = QString("FlashGet ") + str.mid(3, 2) + "." + str.mid(5, 2);
                else if (str.contains(QRegExp("-SP\\d\\d\\d")))
                    ret = QString("BitSpirit ") + str.at(3) + "." + str.at(4) + "." + str.at(5);
                else if (str.contains(QRegExp("346-")))
                    ret = "TorrenTopia";
                else if (str.contains(QRegExp("-G3")))
                    ret = "G3 Torrent";
                else if (str.contains(QRegExp("LIME")))
                    ret = "Limewire";
                else if (str.contains(QRegExp("martini")))
                    ret = "Martini Man";
                else if (str.contains(QRegExp("Pando")))
                    ret = "Pando";
                else if (str.contains(QRegExp("a0[02]---0")))
                    ret = "Swarmy";
                else if (str.contains(QRegExp("10-------")))
                    ret = "JVtorrent";
                else if (str.contains(QRegExp("eX")))
                    ret = "eXeem";
                else if (str.contains(QRegExp("-aria2-")))
                    ret = "aria2";
                else if (str.contains(QRegExp("S3-.-.-.")))
                    ret = QString("Amazon S3 ") + str.at(3) + "." + str.at(5) + "." + str.at(7);
                else if (str.contains(QRegExp("OP")))
                    ret = "Opera (build " + str.mid(2, 4) + ")";
                else if (str.contains(QRegExp("-ML")))
                    ret = "MLDonkey " + str.mid(3, 5);
                else if (str.contains(QRegExp("AP")))
                    ret = "AllPeers " + str.mid(2, 4);
                else if (str.contains(QRegExp("DNA\\d\\d\\d\\d\\d\\d")))
                    ret = QString("BitTorrent DNA ") + str.mid(3, 2) + "." + str.mid(5, 2) + "." + str.mid(7, 2);
                else if (str.contains(QRegExp("Plus")))
                    ret = QString("Plus! v2 ") + str.at(4) + "." + str.at(5) + str.at(6);
                else if (str.contains(QRegExp("XBT\\d\\d\\d")))
                    ret = QString("XBT Client ") + str.at(3) + "." + str.at(4) + "." + str.at(5) + getMnemonicEnd(str.at(6).toAscii());
                else if (str.contains(QRegExp("Mbrst")))
                    ret = QString("burst! ") + str.at(5) + "." + str.at(7) + "." + str.at(9);
                else if (str.contains(QRegExp("btpd")))
                    ret = QString("BT Protocol Daemon ") + str.at(5) + str.at(6) + str.at(7);
                else if (str.contains(QRegExp("BLZ")))
                    ret = QString("Blizzard Downloader %1.%2").arg(str.at(3).toAscii() + 1).arg(str.at(4).toAscii());
                else if (str.contains(QRegExp(".*UDP0$")))
                    ret = "BitSpirit";
                else if (str.contains(QRegExp("QVOD")))
                    ret = QString("QVOD ") + str.at(4) + "." + str.at(5) + "." + str.at(6) + "." + str.at(7);
                else if (str.contains(QRegExp("-NE")))
                    ret = QString("BT Next Evolution ") + str.at(3) + "." + str.at(4) + "." + str.at(5) + "." + str.at(6);
                else if (str.contains(QRegExp("-WT-")))
                    ret = QString("BitLet ") + str.at(4) + "." + str.at(5) + "." + str.at(6) + "." + str.at(7);
                else
                {
                    QString mod;
                    if (str.contains(QRegExp("exbc")))
                        mod = "";
                    else if (str.contains(QRegExp("FUTB")))
                        mod = "(Solidox Mod) ";
                    else if (str.contains(QRegExp("xUTB")))
                        mod = "(Mod 2) ";
                    if (mod.size() > 0)
                    {
                        bool isBitlord = (str.mid(6, 4).compare("LORD") == 0);
                        QString name = isBitlord ? "BitLord " : "BitComet ";
                        char major = str.at(4).toAscii();
                        char minor = str.at(5).toAscii();
                        QString sep(".");
                        if (!(isBitlord && major > 0) && (minor < 10))
                            sep += "0";
                        ret = name + mod + major + sep + minor;
                    }
                }
            }
        }
    }
    if (ret.size() == 0)
    {
        if (str.contains(QRegExp("[A-Z]([A-Z0-9\\-\\.]{1,5})")))
        {
            QString cli = shLikeClients[str.at(0)];
            if (cli.size() > 0)
                ret = cli + " " + shChar(str.at(1)) + "." + shChar(str.at(2)) + "." + shChar(str.at(3));
        }
    }
    return (ret.size() > 0) ? ret : tr("Unknown") + "(" + origStr + ")";
}
