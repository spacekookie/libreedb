# Generated from json-1.8.2.gem by gem2rpm -*- rpm-spec -*-
%global gem_name json

Name: rubygem-%{gem_name}
Version: 1.8.2
Release: 1%{?dist}
Summary: JSON Implementation for Ruby
Group: Development/Languages
License: Ruby
URL: http://flori.github.com/json
Source0: https://rubygems.org/gems/%{gem_name}-%{version}.gem
BuildRequires: ruby(release)
BuildRequires: rubygems-devel
BuildRequires: ruby-devel
# BuildRequires: rubygem(permutation)
# BuildRequires: rubygem(sdoc) => 0.3.16
# BuildRequires: rubygem(sdoc) < 0.4

%description
This is a JSON implementation as a Ruby extension in C.


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

mkdir -p %{buildroot}%{gem_extdir_mri}
cp -a .%{gem_extdir_mri}/{gem.build_complete,*.so} %{buildroot}%{gem_extdir_mri}/

# Prevent dangling symlink in -debuginfo (rhbz#878863).
rm -rf %{buildroot}%{gem_instdir}/ext/
# Prevent dangling symlink in -debuginfo (rhbz#878863).
rm -rf %{buildroot}%{gem_instdir}/ext/
# Prevent dangling symlink in -debuginfo (rhbz#878863).
rm -rf %{buildroot}%{gem_instdir}/ext/



# Run the test suite
%check
pushd .%{gem_instdir}

popd

%files
%dir %{gem_instdir}
%{gem_extdir_mri}
%{gem_instdir}/.
%exclude %{gem_instdir}/.gitignore
%exclude %{gem_instdir}/.travis.yml
%{gem_instdir}/CHANGES
%license %{gem_instdir}/COPYING
%license %{gem_instdir}/COPYING-json-jruby
%{gem_instdir}/GPL
%{gem_instdir}/TODO
%{gem_instdir}/VERSION
%{gem_instdir}/data
%{gem_instdir}/diagrams
%{gem_instdir}/ext
%{gem_instdir}/install.rb
%{gem_instdir}/java
%{gem_libdir}
%{gem_instdir}/tools
%exclude %{gem_cache}
%{gem_spec}

%files doc
%doc %{gem_docdir}
%{gem_instdir}/Gemfile
%doc %{gem_instdir}/README-json-jruby.markdown
%doc %{gem_instdir}/README.rdoc
%{gem_instdir}/Rakefile
%{gem_instdir}/json-java.gemspec
%{gem_instdir}/json.gemspec
%{gem_instdir}/json_pure.gemspec
%{gem_instdir}/tests

%changelog
* Thu May 21 2015 Katharina Sabel - 1.8.2-1
- Initial package
