# Generated from rack-protection-1.5.3.gem by gem2rpm -*- rpm-spec -*-
%global gem_name rack-protection

Name: rubygem-%{gem_name}
Version: 1.5.3
Release: 1%{?dist}
Summary: You should use protection!
Group: Development/Languages
License: MIT
URL: http://github.com/rkh/rack-protection
Source0: https://rubygems.org/gems/%{gem_name}-%{version}.gem
BuildRequires: ruby(release)
BuildRequires: rubygems-devel
BuildRequires: ruby
# BuildRequires: rubygem(rack-test)
# BuildRequires: rubygem(rspec) => 2.0
# BuildRequires: rubygem(rspec) < 3
BuildArch: noarch

%description
You should use protection!


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
%{gem_instdir}/License
%{gem_libdir}
%exclude %{gem_cache}
%{gem_spec}

%files doc
%doc %{gem_docdir}
%doc %{gem_instdir}/README.md
%{gem_instdir}/Rakefile
%{gem_instdir}/rack-protection.gemspec
%{gem_instdir}/spec

%changelog
* Thu May 21 2015 Katharina Sabel - 1.5.3-1
- Initial package
