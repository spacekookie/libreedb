# Generated from daemons-1.2.2.gem by gem2rpm -*- rpm-spec -*-
%global gem_name daemons

Name: rubygem-%{gem_name}
Version: 1.2.2
Release: 1%{?dist}
Summary: A toolkit to create and control daemons in different ways
Group: Development/Languages
License: MIT
URL: https://github.com/thuehlinger/daemons
Source0: https://rubygems.org/gems/%{gem_name}-%{version}.gem
BuildRequires: ruby(release)
BuildRequires: rubygems-devel
BuildRequires: ruby
BuildArch: noarch

%description
Daemons provides an easy way to wrap existing ruby scripts (for example a
self-written server)  to be run as a daemon and to be controlled by simple
start/stop/restart commands.
You can also call blocks as daemons and control them from the parent or just
daemonize the current process.
Besides this basic functionality, daemons offers many advanced features like
exception backtracing and logging (in case your ruby script crashes) and
monitoring and automatic restarting of your processes if they crash.


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
%doc %{gem_instdir}/README.md
%doc %{gem_instdir}/Releases
%{gem_instdir}/examples

%changelog
* Thu May 21 2015 Katharina Sabel - 1.2.2-1
- Initial package
