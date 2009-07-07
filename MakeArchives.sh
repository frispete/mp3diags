#!/bin/bash

#read

function initialize
{
    echo Initializing ...
    rm -f -r package/out
    mkdir -p package/out/deb
    mkdir -p package/out/rpm

    #Ver=`pwd | sed -e 's%/.*/%%' -e 's% .*%%'`
    #Ver=`cat Release.txt`.$Ver
    Ver=`cat Release.txt`

    echo Version: $Ver

    cat package/rpm/MP3Diags.spec | sed "s+%define version .*$+%define version $Ver+" > package/out/rpm/MP3Diags.spec
    cat package/rpm/MP3Diags-Mandriva_2009.1.spec | sed "s+%define version .*$+%define version $Ver+" > package/out/rpm/MP3Diags-Mandriva_2009.1.spec
    cat changelogRpm.txt >> package/out/rpm/MP3Diags.spec
    cat changelogRpm.txt >> package/out/rpm/MP3Diags-Mandriva_2009.1.spec

    #cat package/deb/debian.changelog | sed "s%QQQVERQQQ%$Ver%g" > package/out/deb/debian.changelog
    #cat changelogDeb.txt >> package/out/deb/debian.changelog
    cp -p changelogDeb.txt package/out/deb/debian.changelog
    cp -p package/deb/debian.control package/out/deb
    cp -p package/deb/debian.rules package/out/deb
    cat package/deb/MP3Diags.dsc | sed "s%QQQVERQQQ%$Ver%g" > package/out/deb/MP3Diags.dsc
}


function createLinuxSrc
{
    echo Creating Linux source
    DestDir=MP3Diags-$Ver
    LongDestDir=package/out/$DestDir
    rm -f -r $LongDestDir
    mkdir -p $LongDestDir

    cp -pr desktop $LongDestDir
    cp -pr src $LongDestDir
    cat $LongDestDir/src/src.pro | sed 's%lboost_serialization%lboost_serialization-mt%' > $LongDestDir/src/src.pro1
    mv -f $LongDestDir/src/src.pro1 $LongDestDir/src/src.pro

    rm -f -r $LongDestDir/src/debug
    rm -f -r $LongDestDir/src/release
    rm -f -r $LongDestDir/src/.svn
    rm -f -r $LongDestDir/src/images/.svn
    rm -f -r $LongDestDir/src/licences/.svn
    cp -p COPYING $LongDestDir
    cp -p Install.sh $LongDestDir
    cp -p changelog.txt $LongDestDir
    #cp mp3diags.kdevelop $LongDestDir
    cat mp3diags.kdevelop | grep -v "cwd" | grep -v "home" > $LongDestDir/mp3diags.kdevelop
    cp -p mp3diags.pro $LongDestDir
    cp -p Uninstall.sh $LongDestDir
    cp -p AdjustMt.sh $LongDestDir

    echo const char* APP_VER '("'$Ver'");'> $LongDestDir/src/Version.cpp
    echo >> $LongDestDir/src/Version.cpp

    for i in $( ls src/licences | sed 's%.*/%%' ); do
        cp -p src/licences/$i $LongDestDir/license.$i
    done

    mkdir -p $LongDestDir/package/rpm
    cp -p package/out/rpm/* $LongDestDir/package/rpm
    mkdir -p $LongDestDir/package/deb
    cp -p package/out/deb/* $LongDestDir/package/deb
    rm -f -r $LongDestDir/desktop/.svn

    cd package/out
    tar czf $DestDir.tar.gz $DestDir
    cd ../..

    #rm -f -r $LongDestDir
}


function createWindowsSrc
{
    echo Creating Windows source
    DestDir=MP3DiagsWndSrc-$Ver
    LongDestDir=package/out/$DestDir
    rm -f -r $LongDestDir
    mkdir -p $LongDestDir

    #cp -pr desktop $LongDestDir
    cp -pr src $LongDestDir
    rm -f -r $LongDestDir/src/debug
    rm -f -r $LongDestDir/src/release
    rm -f -r $LongDestDir/src/.svn
    rm -f -r $LongDestDir/src/images/.svn
    rm -f -r $LongDestDir/src/licences/.svn
    cat COPYING | unix2dos > $LongDestDir/COPYING
    cat changelog.txt | unix2dos > $LongDestDir/changelog.txt
    #cp -p Install.sh $LongDestDir
    #cp mp3diags.kdevelop $LongDestDir
    #cat mp3diags.kdevelop | grep -v "cwd" | grep -v "home" > $LongDestDir/mp3diags.kdevelop
    #cp -p mp3diags.pro $LongDestDir
    #cp -p Uninstall.sh $LongDestDir
    cp -p Windows/SVGs/* $LongDestDir/src/images
    rm -f $LongDestDir/src/src.pro
    cp -p Windows/build.bat $LongDestDir
    cp -p Windows/README.TXT $LongDestDir
    cp -p Windows/Mp3DiagsWindows.pro $LongDestDir/src

    echo const char* APP_VER '("'$Ver'");'> $LongDestDir/src/Version.cpp
    echo >> $LongDestDir/src/Version.cpp

    for i in $( ls src/licences | sed 's%.*/%%' ); do
        cp -p src/licences/$i $LongDestDir/license.$i
    done

    cd package/out
    #tar czf $DestDir.tar.gz $DestDir
    zip -r -9 $DestDir.zip $DestDir > /dev/null
    cd ../..

    #rm -f -r $LongDestDir
}


#function updateDwnldLinks
#{
#    cat doc/html/010_getting_the_program.html | sed "s%QQQVERQQQ%$Ver%g" > doc/html/010_getting_the_program.html
#}


function fixVersion
{
    cat $1 | sed "s%QQQVERQQQ%$Ver%g" > QQTmpQQ
    rm $1
    mv QQTmpQQ $1
}


function createDoc
{
    echo Creating non-counted documentation
    DestDir=MP3DiagsDoc-$Ver
    LongDestDir=package/out/$DestDir
    rm -f -r $LongDestDir
    mkdir -p $LongDestDir

    cp -pr doc/html/*.html $LongDestDir
#    fixVersion $LongDestDir/010_getting_the_program.html
    package/AddChangeLog.py $LongDestDir

    cp -pr doc/html/*.css $LongDestDir
    cp -pr doc/html/*.png $LongDestDir
    cp -pr doc/html/*.ico $LongDestDir
    cp -p COPYING $LongDestDir
    mv -f $LongDestDir/010_getting_the_program_local.html $LongDestDir/010_getting_the_program.html

    #rm $LongDestDir/010a_getting_the_program.html

    cd package/out
    tar czf $DestDir.tar.gz $DestDir
    cd ../..

    #rm -f -r $LongDestDir
}


function createClicknetDoc
{
    echo Creating Clicknet documentation
    DestDir=MP3DiagsClicknetDoc-$Ver
    LongDestDir=package/out/$DestDir
    rm -f -r $LongDestDir
    mkdir -p $LongDestDir

    #cp -pr doc/html/*.html $LongDestDir
    for i in $( ls doc/html/*.html | sed 's%.*/%%' ); do
        cat doc/html/$i | sed 's%QQQStatCounterQQQ% Start of StatCounter Code --> <script type="text/javascript"> var sc_project=4841840; var sc_invisible=1; var sc_partition=56; var sc_click_stat=1; var sc_security="644c2333"; </script> <script type="text/javascript" src="http://www.statcounter.com/counter/counter.js"></script><noscript><div class="statcounter"><a title="free hit counter" href="http://www.statcounter.com/free_hit_counter.html" target="_blank"><img class="statcounter" src="http://c.statcounter.com/4841840/0/644c2333/1/" alt="free hit counter" ></a></div></noscript><!-- End of StatCounter Code %' > $LongDestDir/$i
    done
    fixVersion $LongDestDir/010_getting_the_program.html
    package/AddChangeLog.py $LongDestDir

    cp -pr doc/html/*.css $LongDestDir
    cp -pr doc/html/*.png $LongDestDir
    cp -pr doc/html/*.ico $LongDestDir
    cp -p COPYING $LongDestDir
    rm $LongDestDir/010_getting_the_program_local.html

    #rm $LongDestDir/010a_getting_the_program.html

    cd package/out
    tar czf $DestDir.tar.gz $DestDir
    cd ../..

    #rm -f -r $LongDestDir
}


#<!-- Piwik --><script type="text/javascript">var pkBaseURL = (("https:" == document.location.protocol) ? "https://sourceforge.net/apps/piwik/mp3diags/" : "http://sourceforge.net/apps/piwik/mp3diags/");document.write(unescape("%3Cscript src='" + pkBaseURL + "piwik.js' type='text/javascript'%3E%3C/script%3E"));</script><script type="text/javascript">piwik_action_name = '';piwik_idsite = 1;piwik_url = pkBaseURL + "piwik.php";piwik_log(piwik_action_name, piwik_idsite, piwik_url);</script><object><noscript><p><img src="http://sourceforge.net/apps/piwik/mp3diags/piwik.php?idsite=1" alt="piwik"/></p></noscript></object><!-- End Piwik Tag -->

function createSfDoc
{
    echo Creating SF documentation
    DestDir=MP3DiagsSfDoc-$Ver
    LongDestDir=package/out/$DestDir
    rm -f -r $LongDestDir
    mkdir -p $LongDestDir

    #cp -pr doc/html/*.html $LongDestDir
    for i in $( ls doc/html/*.html | sed 's%.*/%%' ); do
        cat doc/html/$i | sed 's#QQQStatCounterQQQ# Start of StatCounter Code --> <script type="text/javascript"> var sc_project=4765268; var sc_invisible=1; var sc_partition=54; var sc_click_stat=1; var sc_security="b8120652"; </script> <script type="text/javascript" src="http://www.statcounter.com/counter/counter.js"></script> <noscript> <div class="statcounter"> <a title="website statistics" href="http://www.statcounter.com/" target="_blank"> <img class="statcounter" src="http://c.statcounter.com/4765268/0/b8120652/1/" alt="website statistics" > </a> </div> </noscript> <!-- End of StatCounter Code -->          <!-- Piwik --><script type="text/javascript">var pkBaseURL = (("https:" == document.location.protocol) ? "https://sourceforge.net/apps/piwik/mp3diags/" : "http://sourceforge.net/apps/piwik/mp3diags/");document.write(unescape("%3Cscript src='\''" + pkBaseURL + "piwik.js'\'' type='\''text/javascript'\''%3E%3C/script%3E"));</script><script type="text/javascript">piwik_action_name = '\'''\'';piwik_idsite = 1;piwik_url = pkBaseURL + "piwik.php";piwik_log(piwik_action_name, piwik_idsite, piwik_url);</script><object><noscript><p><img src="http://sourceforge.net/apps/piwik/mp3diags/piwik.php?idsite=1" alt="piwik"/></p></noscript></object><!-- End Piwik Tag #' | sed 's#<!-- sf_hosting -->#<td border="0" class="HeaderText HeaderPadRight RightAlign"><a href="http://sourceforge.net/projects/mp3diags"><img border="0" align=middle src="http://sflogo.sourceforge.net/sflogo.php?group_id=260878\&amp;type=12" width="120" height="30" alt="Get MP3 Diags at SourceForge.net. Fast, secure and Free Open Source software downloads" /></a></td>#' > $LongDestDir/$i
    done
    fixVersion $LongDestDir/010_getting_the_program.html
    package/AddChangeLog.py $LongDestDir

    cp -pr doc/html/*.css $LongDestDir
    cp -pr doc/html/*.png $LongDestDir
    cp -pr doc/html/*.ico $LongDestDir
    cp -p COPYING $LongDestDir
    rm $LongDestDir/010_getting_the_program_local.html

    #rm $LongDestDir/010a_getting_the_program.html

    cd package/out
    tar czf $DestDir.tar.gz $DestDir
    cd ../..

    #rm -f -r $LongDestDir
}

#pwd > /home/ciobi/cpp/Mp3Utils/MP3Diags/d

initialize
createLinuxSrc
createWindowsSrc
#updateDwnldLinks
createDoc
createClicknetDoc
createSfDoc

#FileName=`find . -maxdepth 1 -mindepth 1 -type d | sed s#./##`
