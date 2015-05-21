# Generated from aes-0.5.0.gem by gem2rpm -*- rpm-spec -*-
%global gem_name aes

Name: rubygem-%{gem_name}
Version: 0.5.0
Release: 1%{?dist}
Summary: An AES encrypt/decrypt gem built ontop of OpenSSL.
Group: Development/Languages
License: MIT
URL: http://github.com/chicks/aes
Source0: https://rubygems.org/gems/%{gem_name}-%{version}.gem
BuildRequires: ruby(release)
BuildRequires: rubygems-devel
BuildRequires: ruby
# BuildRequires: rubygem(shoulda)
# BuildRequires: rubygem(jeweler)
# BuildRequires: rubygem(simplecov)
BuildArch: noarch

%description
An AES encrypt/decrypt gem built ontop of OpenSSL.  Not as quick as FastAES, but it doesn't require building native extensions and supports Base64 encoded input and output.

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




# Run the test suite
%check
pushd .%{gem_instdir}

popd

%files
%dir %{gem_instdir}
%license %{gem_instdir}/LICENSE.txt
%{gem_instdir}/VERSION
%{gem_libdir}
%exclude %{gem_cache}
%{gem_spec}

%files doc
%doc %{gem_docdir}
%doc %{gem_instdir}/.document
%{gem_instdir}/Gemfile
%doc %{gem_instdir}/README.rdoc
%{gem_instdir}/Rakefile
%{gem_instdir}/aes.gemspec
%{gem_instdir}/test

%changelog
* Thu May 21 2015 Katharina 'spacekookie' Sabel <sabel.katharina@gmail.com> - 0.5.0-1
- Initial package
