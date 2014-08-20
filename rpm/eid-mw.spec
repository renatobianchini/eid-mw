#Summary: Belgium electronic identity card PKCS#11 module and Firefox plugin
# Authority: dag

Version: 4.0.6
Release: 0.%{revision}%{?dist}
License: LGPL
Group: Applications/Communications
URL: http://eid.belgium.be/
Summary: Belgium electronic identity card PKCS#11 module and Firefox plugin
Name: eid-mw

Source: http://eidmw.yourict.net/dist/sources/eid-mw-%{version}-%{revision}.tar.gz
Source1: baselibs.conf
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: gtk2-devel
BuildRequires: pcsc-lite-devel
Requires(pre): /sbin/chkconfig
Requires(pre): /sbin/service
Requires: eid-mw-bin
Requires: eid-mw-libs
%if 0%{?suse_version}
Requires: pcsc-ccid
BuildRequires: gcc-c++
%else
Requires: ccid
%endif
Conflicts: openct

%description
The eID Middleware provides the libraries, a PKCS#11 module and a Firefox
plugin to use Belgian eID (electronic identity) card in order to access
websites and/or sign documents.

%package devel
Summary: Belgium electronic identity card PKCS#11 module - development package
Requires: eid-mw

%description devel
The eID Middleware provides the libraries, a PKCS#11 module and a Firefox
plugin to use Belgian eID (electronic identity) card in order to access
websites and/or sign documents. This package contains the files needed
to develop against the eID Middleware.

%package bin
Summary: Belgium electronic identity card PKCS#11 module - helper binaries
Requires: eid-mw

%description bin
The eID Middleware provides the libraries, a PKCS#11 module and a Firefox
plugin to use Belgian eID (electronic identity) card in order to access
websites and/or sign documents. This package contains a few helper
programs needed by the eID Middleware.

%package libs
Summary: Belgium electronic identity card PKCS#11 module - libraries

%description libs
The eID Middleware provides the libraries, a PKCS#11 module and a Firefox
plugin to use Belgian eID (electronic identity) card in order to access
websites and/or sign documents. This package contains the actual libraries.

%prep
%setup

%build
%configure
%{__make} %{?_smp_mflags}

%install
%{__rm} -rf %{buildroot}
%{__make} install DESTDIR="%{buildroot}"

%clean
%{__rm} -rf %{buildroot}

%post
/sbin/ldconfig

### Notify user if an action is required for the eID plugin to work.
if /usr/bin/pgrep 'firefox' &>/dev/null; then
    echo "INFO: You may have to restart Firefox for the Belgium eID add-on to work." >&2
elif /usr/bin/pgrep 'iceweasel' &>/dev/null; then
    echo "INFO: You may have to restart Iceweasel for the Belgium eID add-on to work." >&2
fi

%postun
/sbin/ldconfig

### Make pcscd reread configuration and rescan USB bus.
if /sbin/service pcscd status &>/dev/null; then
    %{_sbindir}/pcscd -H &>/dev/null || :
fi

%files
%defattr(-, root, root, 0755)
### Include license files
%doc ChangeLog NEWS README
%{_datadir}/mozilla/extensions/
%exclude %{_libdir}/*.la
%files libs
%doc ChangeLog NEWS README
%{_libdir}/libbeidcardlayer.so
%{_libdir}/libbeidcommon.so.*
%{_libdir}/libbeiddialogs.so
%{_libdir}/libbeidpkcs11.so.*
%exclude %{_libdir}/libbeidcommon.so
%exclude %{_libdir}/libbeidpkcs11.so
%files bin
%doc ChangeLog NEWS README
%{_libexecdir}/beid-askaccess
%{_libexecdir}/beid-askpin
%{_libexecdir}/beid-badpin
%{_libexecdir}/beid-changepin
%{_libexecdir}/beid-spr-askpin
%{_libexecdir}/beid-spr-changepin
%files devel
%doc ChangeLog NEWS README
%{_libdir}/libbeidcardlayer.a
%{_libdir}/libbeidpkcs11.a
%{_libdir}/libbeidpkcs11.so
%{_libdir}/libbeidcommon.a
%{_libdir}/libbeidcommon.so
%{_libdir}/libbeiddialogs.a

%changelog
* Thu Aug 14 2014 Wouter Verhelst <wouter.verhelst@fedict.be> - 4.0.6-0.R
- Split up somewhat further so that openSUSE-style multiarch works, too.

* Thu Jul 31 2014 Wouter Verhelst <wouter.verhelst@fedict.be> - 4.0.6-0.R
- Split package up into several subpackages so as to make multiarch work
  without much issues.

* Tue Oct 15 2013 Frank Marien <frank@apsu.be> - 4.0.6-0.R
- Upgrade to 4.0.6

* Thu May 3 2012 Frank Marien <frank@apsu.be> - 4.0.4-0.R
- Upgrade to 4.0.4

* Wed Mar 14 2012 Frank Marien <frank@apsu.be> - 4.0.2-0.R
- Upgrade to 4.0.2

* Fri Mar 18 2011 Frank Marien <frank@apsu.be> - 4.0.0-0.R
- Made Revision number variable to allow continuous builds.

* Thu Mar 17 2011 Dag Wieers <dag@wieers.com> - 4.0.0-0.6
- Split eid-mw and eid-viewer packages.

* Thu Feb 24 2011 Dag Wieers <dag@wieers.com> - 4.0.0-0.5
- Added post-install script and desktop file.

* Thu Feb 24 2011 Dag Wieers <dag@wieers.com> - 4.0.0-0.4
- Included pre-built JAR files.

* Wed Feb 23 2011 Dag Wieers <dag@wieers.com> - 4.0.0-0.3
- Added patched eid-applet core.

* Sun Feb 13 2011 Dag Wieers <dag@wieers.com> - 4.0.0-0.2
- Included eid-viewer build using maven.

* Mon Feb  7 2011 Dag Wieers <dag@wieers.com> - 4.0.0-0.1
- Initial package.

