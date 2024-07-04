%global _usrlibdir /usr/lib

Name: smlnj
Version: 2024.1
Release: alt1

Summary: The Standard ML of New Jersey Programming Language
License: BSD-3-Clause
Group: Development/Other
Url: https://github.com/smlnj/smlnj/tree/main

BuildRequires: curl gcc gcc-c++

Source: %name-%version.tar
Patch: %name-alt-cfg.patch 

%description
This is the main development repository for SML/NJ. We are currently reworking many components of the system, so it is not very stable. For most uses, we recommend the legacy version of the system.
As of the 2023.1 release, this version is known to work on AMD64 (a.k.a. x86-64) hardware running Linux or mac.

%prep
%setup -q
%patch0 -p1

%build
./build.sh

%install
mkdir -p %buildroot%_bindir
mkdir -p %buildroot%_usrlibdir
install -m 755 -D bin/* %buildroot%_bindir
cp -rp lib/* %buildroot%_usrlibdir/

%files
%_bindir/*
%_usrlibdir/*

%changelog
* Wed Jul 3 2024 Kirill Sharov <sheriffkorov@altlinux.org> 2024.1-alt1
- Initial release for Sisyphus.
