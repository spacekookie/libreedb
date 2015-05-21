# Generated from reedb-0.10.rc2.gem by gem2rpm -*- rpm-spec -*-
%global gem_name reedb

Name: rubygem-%{gem_name}
Version: 0.10.rc1
Release: 1%{?dist}
Summary: Ruby storage containers/ database that uses completely encrpypted files for maximum security
Group: Development/Languages
License: LGPL v3.0
URL: https://github.com/tr-lonelyrobot/reedb/wiki
Source0: %{gem_name}-%{version}.gem
BuildRequires: ruby(release)
BuildRequires: rubygems-devel => 1.3.1
BuildRequires: ruby => 2.1
BuildArch: noarch

%description
Ruby storage containers/ database. Uses Rinjdael, Twofish, ... to keep your
data safe. Made for native and local application development. Spawns a system
daemon that manages vaults that applications can talk to. Check out the wiki
on Github to find out about how to use this API

%package doc
Summary: Documentation for %{name}
Group: Documentation
Requires: %{name} = %{version}-%{release}
BuildArch: noarch

%description doc
Documentation for %{name}.

%prep
gem unpack %{SOURCE0}

%setup -q -D -T -n  %{gem_name}-%{version}

gem spec %{SOURCE0} -l --ruby > %{gem_name}.gemspec

%build
# Create the gem as gem install only works on a gem file
gem build %{gem_name}.gemspec

# %%gem_install compiles any C extensions and installs the gem into ./%%gem_dir
# by default, so that we can move it into the buildroot in %%install
%gem_install

%install
mkdir -p %{buildroot}%{gem_dir}
cp -a .%{gem_dir}/* \
        %{buildroot}%{gem_dir}/

mkdir -p %{buildroot}%{_bindir}
cp -pa .%{_bindir}/* \
        %{buildroot}%{_bindir}/

find %{buildroot}%{gem_instdir}/bin -type f | xargs chmod a+x

# Run the test suite
%check
pushd .%{gem_instdir}

popd

%files
%dir %{gem_instdir}
%{_bindir}/reedbd
%{gem_instdir}/.ruby-gemset
%{gem_instdir}/.ruby-version
%{gem_instdir}/bin
%{gem_libdir}
%{gem_instdir}/rubygem-reedb-0.10.2-1.noarch.rpm
%{gem_instdir}/tmp
%exclude %{gem_cache}
%{gem_spec}

%files doc
%doc %{gem_docdir}
%{gem_instdir}/Gemfile
%{gem_instdir}/Gemfile.lock
%{gem_instdir}/Rakefile
%{gem_instdir}/reedb.gemspec
%{gem_instdir}/tests

%changelog
* Thu May 21 2015 Katharina Sabel <sabel.katharina@gmail.com> - 0.10.rc2-1
- Initial package creation
