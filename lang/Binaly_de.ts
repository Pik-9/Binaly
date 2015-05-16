<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="de_DE">
<context>
    <name>AboutDialog</name>
    <message>
        <location filename="../src/About.cpp" line="46"/>
        <source>Binaly is a binary/hex editor which shows the whole file on a colorful strip.
It subdivides the file into blocks of 1 Kib (1024 Bytes) and analyses them.
It knows the following kinds of data sections:
- Random binary data:      The byte&apos;s average value is in [108, 148] and the standard deviation is in [60, 68].
- Text data:               The average value is &gt; 64 and the std. dev. &lt; 40.
- Homogeneous data:        The std. dev. is &lt; 2.
- Other data section:      The block doesn&apos;t fit into any of the categories above.

This is very useful to get a quick overview over the file and its content.

(C) 2015 by Daniel Steinhauer</source>
        <translation>Binaly ist ein Hex-/Binäreditor der eine ganze Datei farbig auf einem Streifen darstellt.
Es unterteilt die Datei dabei in 1 KiB (1024 Byte) große Blöcke und analysiert dessen Inhalt.
Es unterscheidet die folgenden verschiedenen Datenarten:
- Zufällige Binärdaten:  Der Durchschnittswert der Bytes liegt in [108, 148] und ihre Standardabweichung liegt in [60, 68].
- Textdaten:  Der Durchschnitt ist &gt; 64 und die Std.Abw. &lt; 40.
- Homogene Daten:  Die Std.Abw. ist &lt; 2.
-Sonstige Daten: Keine der oben genannten Kategorien.

Das ist praktisch um einen schnellen Überblick über die Datei und ihren Inhalt zu bekommen.

(C) 2015 by Daniel Steinhauer</translation>
    </message>
    <message>
        <location filename="../src/About.cpp" line="37"/>
        <source>About Binaly</source>
        <translation>Über Binaly</translation>
    </message>
    <message>
        <location filename="../src/About.cpp" line="58"/>
        <source>OK</source>
        <translation>OK</translation>
    </message>
</context>
<context>
    <name>Binaly</name>
    <message>
        <location filename="../src/Binaly.cpp" line="41"/>
        <source>Binaly</source>
        <translation>Binaly</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="53"/>
        <source>Select file...</source>
        <translation>Wähle Datei...</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="57"/>
        <source>&amp;File</source>
        <translation>&amp;Datei</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="58"/>
        <source>&amp;Open</source>
        <translation>&amp;Öffnen</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="59"/>
        <source>&amp;Save</source>
        <translation>&amp;Speichern</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="60"/>
        <source>&amp;Quit</source>
        <translation>&amp;Beenden</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="65"/>
        <source>S&amp;ettings</source>
        <translation>&amp;Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="66"/>
        <source>Settings</source>
        <translation>Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="69"/>
        <source>&amp;Help</source>
        <translation>&amp;Hilfe</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="70"/>
        <source>About Binaly</source>
        <translation>Über Binaly</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="71"/>
        <location filename="../src/Binaly.cpp" line="217"/>
        <source>About Qt</source>
        <translation>Über Qt</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="90"/>
        <source>&lt;font color=&apos;%1&apos;&gt;Homogeneous data&lt;/font&gt;</source>
        <translation>&lt;font color=&apos;%1&apos;&gt;Homogene Daten&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="91"/>
        <source>&lt;font color=&apos;%1&apos;&gt;Random binary data&lt;/font&gt;</source>
        <translation>&lt;font color=&apos;%1&apos;&gt;Zufällige Binärdaten&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="92"/>
        <source>&lt;font color=&apos;%1&apos;&gt;Text data&lt;/font&gt;</source>
        <translation>&lt;font color=&apos;%1&apos;&gt;Textdaten&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="93"/>
        <source>&lt;font color=&apos;%1&apos;&gt;Other data&lt;/font&gt;</source>
        <translation>&lt;font color=&apos;%1&apos;&gt;Sonstige Daten&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="94"/>
        <source>Current position: 0</source>
        <translation>Aktuelle Position: 0</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="95"/>
        <source>&lt;= 1024 [Q]</source>
        <translation>&lt;= 1024 [Q]</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="96"/>
        <source>[W] 1024 =&gt;</source>
        <translation>[W] 1024 =&gt;</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="121"/>
        <source>Byte histogram</source>
        <translation>Byte Histogramm</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="122"/>
        <source>Fourier transform</source>
        <translation>Fourier Transformation</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="139"/>
        <source>Ready</source>
        <translation>Bereit</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="168"/>
        <source>Unsaved changes</source>
        <translation>Ungespeicherte Änderungen</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="169"/>
        <source>The current file has unsaved changes. Do you want to save it?</source>
        <translation>Die aktuelle Datei hat ungespeicherte Änderungen. Wollen Sie sie speichern?</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="207"/>
        <source>Current position: %1</source>
        <translation>Aktuelle Position: %1</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="242"/>
        <source>Loading file %1, please wait...</source>
        <translation>Lade Datei %1, Bitte warten...</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="249"/>
        <source>File changed.</source>
        <translation>Datei geändert.</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="250"/>
        <source>Binaly - %1 *</source>
        <translation>Binaly - %1 *</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="264"/>
        <source>File saved to %1.</source>
        <translation>Datei gespeichert in %1.</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="265"/>
        <location filename="../src/Binaly.cpp" line="280"/>
        <source>Binaly - %1</source>
        <translation>Binaly - %1</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="274"/>
        <source>File %1 loaded.</source>
        <translation>Datei %1 geladen.</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="293"/>
        <source>File error!</source>
        <translation>Dateifehler!</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="294"/>
        <source>An error occured while reading/writing the file!</source>
        <translation>Beim Lesen/Schreiben der Datei ist ein Fehler aufgetreten!</translation>
    </message>
    <message>
        <location filename="../src/Binaly.cpp" line="296"/>
        <source>IO error</source>
        <translation>Ein-/Ausgabefehler</translation>
    </message>
</context>
<context>
    <name>BinaryBar</name>
    <message>
        <location filename="../src/custom_comp.cpp" line="159"/>
        <source>No data</source>
        <translation>Keine Daten</translation>
    </message>
</context>
<context>
    <name>FourierSheet</name>
    <message>
        <location filename="../src/custom_comp.cpp" line="386"/>
        <location filename="../src/custom_comp.cpp" line="478"/>
        <source>No data</source>
        <translation>Keine Daten</translation>
    </message>
    <message>
        <location filename="../src/custom_comp.cpp" line="394"/>
        <source>F(f(1/%1)) = %2 + %3i</source>
        <translation>F(f(1/%1)) = %2 + %3i</translation>
    </message>
    <message>
        <location filename="../src/custom_comp.cpp" line="467"/>
        <source>Real</source>
        <translation>Realteil</translation>
    </message>
    <message>
        <location filename="../src/custom_comp.cpp" line="470"/>
        <source>Imaginary</source>
        <translation>Imaginärteil</translation>
    </message>
</context>
<context>
    <name>HexWidget</name>
    <message>
        <location filename="../src/Hexedit.cpp" line="42"/>
        <source>Binary</source>
        <translation>Binär</translation>
    </message>
    <message>
        <location filename="../src/Hexedit.cpp" line="43"/>
        <source>Octal</source>
        <translation>Oktal</translation>
    </message>
    <message>
        <location filename="../src/Hexedit.cpp" line="44"/>
        <source>Decimal</source>
        <translation>Dezimal</translation>
    </message>
    <message>
        <location filename="../src/Hexedit.cpp" line="45"/>
        <source>Hexadecimal</source>
        <translation>Hexadezimal</translation>
    </message>
</context>
<context>
    <name>Histogram</name>
    <message>
        <location filename="../src/custom_comp.cpp" line="261"/>
        <location filename="../src/custom_comp.cpp" line="316"/>
        <source>No data</source>
        <translation>Keine Daten</translation>
    </message>
    <message>
        <location filename="../src/custom_comp.cpp" line="267"/>
        <source>%2 x Byte 0x%1</source>
        <translation>%2 x Byte 0x%1</translation>
    </message>
</context>
<context>
    <name>SettingsDialog</name>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="85"/>
        <source>Binaly - Settings</source>
        <translation>Binaly - Einstellungen</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="101"/>
        <source>Color settings</source>
        <translation>Farbeinstellungen</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="104"/>
        <source>Text section color:</source>
        <translation>Farbe für Textabschnitt:</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="105"/>
        <source>Binary section color:</source>
        <translation>Farbe für Binärabschnitt:</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="106"/>
        <source>Homogeneous section color:</source>
        <translation>Farbe für homogenen Datenabschnitt:</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="107"/>
        <source>Other data section color:</source>
        <translation>Farbe für sonstigen Datenabschnitt:</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="122"/>
        <source>Language settings</source>
        <translation>Spracheinstellungen</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="124"/>
        <source>Language:</source>
        <translation>Sprache:</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="126"/>
        <source>English</source>
        <translation>Englisch</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="127"/>
        <source>German</source>
        <translation>Deutsch</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="136"/>
        <source>Apply</source>
        <translation>Übernehmen</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="137"/>
        <source>Cancel</source>
        <translation>Abbrechen</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="168"/>
        <source>Settings saved</source>
        <translation>Einstellungen übernommen</translation>
    </message>
    <message>
        <location filename="../src/SettingsDialog.cpp" line="169"/>
        <source>The new settings have been applied. Some changes will take effect after you restart the application.</source>
        <translation>Die Einstellungen wurden übernommen. Einige Einstellungen werden erst aktiv, nachdem die Anwendung neu gestartet wurde.</translation>
    </message>
</context>
</TS>
