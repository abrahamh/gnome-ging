%define prefix 	/usr
%define ver    	2.0
Summary:   	GNOME ding translation- and spellchecking for GNOME
Summary(de):   	GNOME ding ist ein Uebersetzungs- und Rechtschreibpruefprogramm
Name:      	gnome-ding
Version:   	%ver
Release:   	1
Copyright: 	GPL
Packager:  	Heiko Abraham  <abrahamh@web.de>
Group:     	Applications/System
Source:    	%{name}-%{ver}.tar.gz
BuildRoot: 	/var/tmp/%{name}-%{PACKAGE_VERSION}-root
Requires:  	aspell >= 0.50
Requires:  	grep 



%description
GNOME ding is a translator and spellchecker for Gtk2, inspired by ding.
For spellchecking, it will use installed aspell-dictionories and
for translations it will use ding-like dictinories, thas will be
shiped with this package.


%description -l de
GNOME ding ist ein Uebersetzungs- und Rechtschreibpruefprogramm fuer Gnome.
Es verwendet fuer die Rechtschreibpruefung die installierten aspell-
Woerterbuecher. Zum Uebersetzen verwendet gnome-ding ein mitgeliefertes
deu-eng-Woerterbuch, das aehnlich zum ding-Woerterbuch gehalten ist.


%prep
%setup -q

%build
# ./autogen.sh
./configure --prefix=%{prefix} --enable-maintainer-mode --enable-aspell
make -j2


%install
rm -rf $RPM_BUILD_ROOT

make DESTDIR=$RPM_BUILD_ROOT  install


%post
if which scrollkeeper-update>/dev/null 2>&1; then
    scrollkeeper-update
fi


%preun

%postun
which scrollkeeper-update>/dev/null 2>&1 && scrollkeeper-update

%clean
rm -rf $RPM_BUILD_ROOT/*


%files
%doc AUTHORS TODO README COPYING 
%{prefix}/bin/g*
/usr/share/dict/*
%{prefix}/share/pixmaps/*
%{prefix}/share/locale/*
%{prefix}/share/omf/*
%{prefix}/share/gnome/help/*
/usr/share/applications







