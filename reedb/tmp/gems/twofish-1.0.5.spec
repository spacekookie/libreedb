# Generated from twofish-1.0.5.gem by gem2rpm -*- rpm-spec -*-
%global gem_name twofish

Name: rubygem-%{gem_name}
Version: 1.0.5
Release: 1%{?dist}
Summary: Twofish symmetric cipher in pure Ruby
Group: Development/Languages
License: BSD
URL: http://mcarpenter.org/projects/twofish
Source0: https://rubygems.org/gems/%{gem_name}-%{version}.gem
BuildRequires: ruby(release)
BuildRequires: rubygems-devel
BuildRequires: ruby
BuildArch: noarch

%description
Twofish symmetric cipher in pure Ruby with ECB and CBC cipher modes derived
from an original Perl implementation by Guido Flohr.


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
%license %{gem_instdir}/LICENSE
%{gem_libdir}
%exclude %{gem_cache}
%{gem_spec}

%files doc
%doc %{gem_docdir}
%doc %{gem_instdir}/README.rdoc
%{gem_instdir}/Rakefile
%{gem_instdir}/test

%changelog
* Thu May 21 2015 Katharina 'spacekookie' Sabel <sabel.katharina@gmail.com> - 1.0.5-1
- Initial package
