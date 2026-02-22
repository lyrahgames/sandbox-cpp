<div align="center">

# sandbox - C++ Library for Scriptable Graphics Demos

`sandbox` is a C++ library used for quick prototyping of scriptable graphics demos.

![top-language-badge](https://img.shields.io/github/languages/top/lyrahgames/sandbox-cpp.svg?style=for-the-badge)
![code-size-badge](https://img.shields.io/github/languages/code-size/lyrahgames/sandbox-cpp.svg?style=for-the-badge)
![repo-size-badge](https://img.shields.io/github/repo-size/lyrahgames/sandbox-cpp.svg?style=for-the-badge)
[![license-badge](https://img.shields.io/github/license/lyrahgames/sandbox-cpp.svg?style=for-the-badge&color=blue)](#copyright-and-license)

![GitHub commit activity](https://img.shields.io/github/commit-activity/y/lyrahgames/sandbox-cpp?style=for-the-badge)
![GitHub last commit (by committer)](https://img.shields.io/github/last-commit/lyrahgames/sandbox-cpp?style=for-the-badge)
![GitHub tag (with filter)](https://img.shields.io/github/v/tag/lyrahgames/sandbox-cpp?style=for-the-badge)

<!-- [![cppget.org](https://img.shields.io/website/https/cppget.org/libsandbox.svg?down_message=offline&label=cppget.org&style=for-the-badge&up_color=blue&up_message=online)](https://cppget.org/libsandbox) -->

</div>

## Requirements and Dependencies

`sandbox` uses the [standard `build2` CI service][ci.cppget.org] to build and test its source code with a [variety of build configurations][ci.cppget.org-build-configs].
For a thorough list of requirements, see the [`manifest`][manifest] file.
To see detailed information about the current build status, [set up `sandbox` for development and run `bdep ci`](#how-to-set-up-sandbox-for-development).
<!-- To see the build status of already published package versions, see [the package's build status report]. -->

`sandbox` deliberately targets the newest C++ language standards and makes extensive use of recently standardized features.
As such, it requires a modern C++ compiler with strong support for the latest standards (typically recent versions of Clang or GCC).

<!-- Here is a short list of essential requirements and dependencies: -->

<!-- **Architecture:** -->
<!-- - `x86_64` | `aarch64` -->

<!-- **Operating System:** -->
<!-- - Linux | MacOS | Windows -->

<!-- **C++ Compiler:** -->
<!-- - GCC 14 | Clang 18 -->
<!-- | MinGW | MSVC | Emscripten -->
<!-- - `libstdc++` | `libc++` -->

<!-- **Build Toolchain:** -->
<!-- - [`build2`][build2] -->

<!-- **Automatically handled Dependencies:** -->
<!-- **Manually handled Requirements:** -->

<!-- ## Introduction -->

## Getting Started

`sandbox` relies on [the `build2` toolchain][build2] for building and
dependency management.
Install it via your system's package manager or follow the instructions in [The `build2` Installation Manual][build2-install] to build it from source.
If you are new to `build2`, see [The `build2` Toolchain Introduction][build2-intro] and [The `build2` Build System Manual][build2-build-system] for more information.

### Usage in `build2`-Based Projects

1. Add this repository to the `repositories.manifest` of your `build2` project. To track the latest development version, append `#main` to the repository
location.

```
:
role: prerequisite
location: https://github.com/lyrahgames/sandbox-cpp.git
```

2. In the `manifest` file of each `build2` package that requires it, declare a dependency on `libsandbox`. An [optional version constraint](https://build2.org/bpkg/doc/build2-package-manager-manual.xhtml#package-version-constraint) may be specified.

```
depends: libsandbox == 0.1.0
```

3. In every `buildfile` that defines targets depending on `sandbox`, import the library and add it as dependency.

```
import sandbox = libsandbox%lib{sandbox}
exe{myexe}: {hxx cxx}{**} $sandbox
```

## Configuration

Currently, `sandbox` does not expose any public configuration options.

## Documentation

This `README.md` currently contains all project documentation.
Additional documentation may be added in the future as the project evolves.

### Usage, Examples, and Tutorials

No usage examples or tutorials are currently provided.
They will be added as the library matures.

### FAQs and HOWTOs

#### How to install or use `sandbox` in projects not based on `build2`?

You can make a `build2` package available for non-`build2` projects by following these essential steps.
For more information, consult [The `build2` Toolchain Introduction: Package Consumption][build2-intro-consumption] and [The `build2` Toolchain Introduction: Using Unpackaged Dependencies][build2-intro-unpackaged-dependencies].

1. Create a build configuration for C/C++-based `build2` packages.

```
bpkg create -d build2-packages cc \
  config.cxx=g++ \
  config.cc.coptions="-O3 -march=native" \
  config.install.root=/usr/local \
  config.install.sudo=sudo
```

2. Fetch the repository's packages and build them.

```
bpkg build https://github.com/lyrahgames/sandbox-cpp.git
```

3. Install the built package.

```
bpkg install libsandbox
```

4. Optionally, uninstall the built packages when you do not need them any longer.

```
bpkg uninstall libsandbox
```

5. To upgrade the package, fetch all new information and uninstall the older version. Afterwards, build the new version and install it.

```
bpkg fetch
bpkg status

bpkg uninstall libsandbox
bpkg build libsandbox
bpkg install libsandbox
```

#### How to set up `sandbox` for development?

These steps prepare the repository for development.
Familiarity with `build2` is recommended.
For full details, see [The `build2` Toolchain Introduction][build2-intro].

1. Clone the repository.

```
git clone https://github.com/lyrahgames/sandbox-cpp.git
cd sandbox-cpp
```

2. Use `bdep` to create the initial build configuration. If not stated otherwise, it will be placed within the previously created project-specific directory. For example, the command below shows how to create and initialize a configuration named `gcc-release` with installation and distribution support.

```
bdep init -C @gcc-release cc \
  config.cxx=g++ \
  config.cxx.coptions="-O3 -march=native" \
  config.install.root=../.install \
  config.dist.root=../.dist
```

3. Common Development Tasks

  + Use `bdep` to print the status of project packages and their immediate dependencies.

```
bdep status -i
```

  + Use `b` to build and test the source code.

```
b                      # Build all the code.
b test                 # Only build and run the tests.
b {clean update}       # Clean build all the code.
b {clean update test}  # Clean build all the code and run the tests.
```

  + Similarly, to build and test the source in a specific build configuration with `bdep`, run the following commands.

```
bdep update @gcc-release
bdep test @gcc-release
bdep clean @gcc-release
```

  + Use `b` to test the installation of the project. In the example configuration `gcc-release` created above, the installation directory `.install` is part of the development folder.

```
b install
b uninstall
```

  + Use `b` to test the distribution of the project's packages. In the example configuration `gcc-release` created above, the directory for distributions `.dist` is part of the development folder.

```
b dist: libsandbox
```

  + After thoroughly testing the aforementioned steps, make a commit and leverage [`build2`'s standard CI service][ci.cppget.org] to simultaneously build and test the project across a [range of build configurations][ci.cppget.org-build-configs].

```
bdep ci
```

### API Reference

A formal API reference is not yet available.
For now, you can explore the library’s source files directly.
The inline comments provide guidance on the API usage and implementation details.

<!-- ### Background and Discussions -->
<!-- Currently, there are no background documentation or discussions available. -->

<!-- ## Roadmap -->
<!-- Currently, there is no roadmap available. -->

## Changelog

All notable changes to this project are documented in [`CHANGELOG.md`](CHANGELOG.md).
The changelog follows the [Keep a Changelog][keep-a-changelog] and [Common Changelog][common-changelog] formats.
This project also adheres to [Semantic Versioning][semantic-versioning] to clearly communicate version changes and compatibility.

## Contributing

Contributions are welcome and greatly appreciated!
Please start by [opening an issue](https://github.com/lyrahgames/sandbox-cpp/issues) to report a bug or suggest an improvement.
This helps us coordinate efforts and avoid duplicate work.
You are then welcome to submit a [pull request](https://github.com/lyrahgames/sandbox-cpp/pulls) that references the issue.

<!-- ## Code of Conduct -->

<!-- All contributors are expected to adhere to [our established code of conduct][code-of-conduct]. -->

<!-- ## Contact -->

<!-- Email us at lyrahgames@mailbox.org with any questions or comments about `sandbox`. -->
<!-- You may also tell us about your project or specific use case if you use `sandbox`. -->
<!-- We're happy to hear from you. -->

## Copyright and License

The copyright for the code is held by the contributors of the code.
The revision history in the version control system is the primary source of authorship information for copyright purposes.
Please see individual source files for appropriate copyright notices.
For more information, see the file [`AUTHORS.md`](AUTHORS.md).

`sandbox` is free software, distributed under the terms of the GNU General
Public License as published by the Free Software Foundation,
version 3 of the License (or any later version).  For more information,
see the [GNU General Public License][GPLv3] or the file [`COPYING.md`](COPYING.md).

`sandbox` is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

Copyright years on `sandbox` source files may be listed using range notation, e.g., 1987-2012, indicating that every year in the range, inclusive, is a copyrightable year that could otherwise be listed individually.

Copying and distribution of this file, with or without modification, are permitted in any medium without royalty provided the copyright notice and this notice are preserved.
This file is offered as-is, without any warranty.

## References and Other Resources

- [`build2` | C/C++ Build Toolchain][build2]
<!-- - [`cppget.org` | Standard `build2` Package Repository][cppget.org] -->
<!-- - [`ci.cppget.org` | Standard `build2` CI Service][ci.cppget.org] -->
<!-- - [`build2` | Installation][build2-install] -->
<!-- - [`build2` | Documentation][build2-docs] -->
<!-- - [The `build2` Toolchain Introduction][build2-intro] -->
<!-- - [The `build2` Build System][build2-build-system] -->
<!-- - [The `build2` Package Manager][build2-package-manager] -->
<!-- - [The `build2` Toolchain Introduction: Package Consumption][build2-intro-consumption] -->
<!-- - [The `build2` Toolchain Introduction: Using Unpackaged Dependencies][build2-intro-unpackaged-dependencies] -->

- [GNU Licenses][GNU-licenses]
<!-- - [GNU General Public License Version 3][GPLv3] -->

- [Keep a Changelog][keep-a-changelog]
- [Common Changelog][common-changelog]
- [Semantic Versioning][semantic-versioning]

[manifest]: libsandbox/manifest (libsandbox build2 Package Manifest)
[project-issues]: https://github.com/lyrahgames/sandbox-cpp/issues (Project Issues)

[build2]: https://build2.org (build2 | C/C++ Build Toolchain)
[build2-install]: https://build2.org/install.xhtml (build2 | Installation)
[build2-docs]: https://build2.org/doc.xhtml (build2 | Documentation)
[build2-intro]: https://build2.org/build2-toolchain/doc/build2-toolchain-intro.xhtml (The build2 Toolchain Introduction)
[build2-build-system]: https://build2.org/build2/doc/build2-build-system-manual.xhtml (The build2 Build System)
[build2-package-manager]: https://build2.org/bpkg/doc/build2-package-manager-manual.xhtml (The build2 Package Manager)
[build2-intro-consumption]: https://build2.org/build2-toolchain/doc/build2-toolchain-intro.xhtml#guide-consume-pkg (The build2 Toolchain Introduction: Package Consumption)
[build2-intro-unpackaged-dependencies]: https://build2.org/build2-toolchain/doc/build2-toolchain-intro.xhtml#guide-unpackaged-deps (The build2 Toolchain Introduction: Using Unpackaged Dependencies)

[cppget.org]: https://cppget.org/ (Standard build2 Package Repositories)
[ci.cppget.org]: https://ci.cppget.org/ (Standard build2 CI Service)
[ci.cppget.org-build-configs]: https://ci.cppget.org/?build-configs (Build Configurations in Standard build2 CI Service)

[GNU-licenses]: https://www.gnu.org/licenses/ (GNU Licenses)
[GPLv3]: https://www.gnu.org/licenses/gplv3.html (GNU General Public License Version 3)

[keep-a-changelog]: https://keepachangelog.com/ (keep a changelog)
[common-changelog]: https://common-changelog.org/ (Common Changelog)
[semantic-versioning]: https://semver.org/ (Semantic Versioning)
