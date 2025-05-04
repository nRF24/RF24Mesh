# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).
<!-- markdownlint-disable MD024 -->

## [2.0.3] - 2025-05-04

### <!-- 4 --> 🛠️ Fixed

- Return on setAddress if nodeID is non-zero by \@TMRh20 in [#255](https://github.com/nRF24/RF24Mesh/pull/255)

### <!-- 9 --> 🗨️ Changed

- Bump doxygen version by \@2bndy5 in [`6b0e475`](https://github.com/nRF24/RF24Mesh/commit/6b0e475c604dc018181102864f387f7cfedc7fee)

[2.0.3]: https://github.com/nRF24/RF24Mesh/compare/v2.0.2...v2.0.3

Full commit diff: [`v2.0.2...v2.0.3`][2.0.3]

## [2.0.2] - 2024-10-06

### <!-- 1 --> 🚀 Added

- Enhance checkConnection() function by \@TMRh20 in [#240](https://github.com/nRF24/RF24Mesh/pull/240)
- Expose releaseAddress() for master node by \@2bndy5 in [#244](https://github.com/nRF24/RF24Mesh/pull/244)
- Address build warnings about ncurses example by \@2bndy5 in [#243](https://github.com/nRF24/RF24Mesh/pull/243)
- Mesh.checkConnection via parent not master by \@TMRh20 in [#250](https://github.com/nRF24/RF24Mesh/pull/250)
- Add pyproject.toml by \@2bndy5 in [#252](https://github.com/nRF24/RF24Mesh/pull/252)

### <!-- 4 --> 🛠️ Fixed

- Fix spelling of MESH_NOMASTER in py wrapper by \@2bndy5 in [`2fb4d25`](https://github.com/nRF24/RF24Mesh/commit/2fb4d25a4f789370b88063cfcf20a2cfe93cf6ff)
- Fix spelling of MESH_NOMASTER in RF24Mesh.cpp by \@2bndy5 in [`a8c63bc`](https://github.com/nRF24/RF24Mesh/commit/a8c63bc6793bcace4408d208f8783f6adc932d0c)

### <!-- 8 --> 📝 Documentation

- Remove sphinx and update RTD config by \@2bndy5 in [#253](https://github.com/nRF24/RF24Mesh/pull/253)

### <!-- 9 --> 🗨️ Changed

- Change MESH_DEBUG into RF24MESH_DEBUG by \@2bndy5 in [#239](https://github.com/nRF24/RF24Mesh/pull/239)
- Update doxygen CSS and use v1.11.0 by \@2bndy5 in [#245](https://github.com/nRF24/RF24Mesh/pull/245)
- Bump version to v2.0.2 by \@2bndy5 in [`7baf483`](https://github.com/nRF24/RF24Mesh/commit/7baf4835631c39b3a7f777a7aa16369554c4a70d)

[2.0.2]: https://github.com/nRF24/RF24Mesh/compare/v2.0.1...v2.0.2

Full commit diff: [`v2.0.1...v2.0.2`][2.0.2]

## [2.0.1] - 2024-06-11

### <!-- 1 --> 🚀 Added

- Enhance requestAddress by looping through poll responses by \@TMRh20 in [#238](https://github.com/nRF24/RF24Mesh/pull/238)

### <!-- 8 --> 📝 Documentation

- Nrf52x - docs & library files by \@TMRh20 in [#234](https://github.com/nRF24/RF24Mesh/pull/234)
- Minor doc updates by \@2bndy5 in [#236](https://github.com/nRF24/RF24Mesh/pull/236)

### <!-- 9 --> 🗨️ Changed

- Update clang format by \@2bndy5 in [#231](https://github.com/nRF24/RF24Mesh/pull/231)
- Update Doxyfile 2024 by \@TMRh20 in [#235](https://github.com/nRF24/RF24Mesh/pull/235)
- Bump version to v2.0.1 by \@2bndy5 in [`79078c4`](https://github.com/nRF24/RF24Mesh/commit/79078c4a55d0c57e0f3ebb3a2462f4b6b287bf02)

[2.0.1]: https://github.com/nRF24/RF24Mesh/compare/v2.0.0...v2.0.1

Full commit diff: [`v2.0.0...v2.0.1`][2.0.1]

## [2.0.0] - 2023-06-21

### <!-- 8 --> 📝 Documentation

- Template layer for radio HW abstraction by \@2bndy5 in [#223](https://github.com/nRF24/RF24Mesh/pull/223)

### <!-- 9 --> 🗨️ Changed

- Limit Mesh v2.x to Network v2.x tags by \@2bndy5 in [`38ee022`](https://github.com/nRF24/RF24Mesh/commit/38ee0225c336349fab513907f9798d308d2db497)

[2.0.0]: https://github.com/nRF24/RF24Mesh/compare/v1.1.9...v2.0.0

Full commit diff: [`v1.1.9...v2.0.0`][2.0.0]

## [1.1.9] - 2023-06-06

### <!-- 1 --> 🚀 Added

- Support NRF52x by \@TMRh20 in [`03e73fb`](https://github.com/nRF24/RF24Mesh/commit/03e73fb29c6f849429f327250cd77621266c631d)
- Add `deploy-release` arg to PIO reusable CI call by \@2bndy5 in [`673a76f`](https://github.com/nRF24/RF24Mesh/commit/673a76f40d8e35bc9de2d87d6b529c3f6c59acb2)

### <!-- 3 --> 🗑️ Removed

- Remove nrf_to_nrf support by \@TMRh20 in [`85a372f`](https://github.com/nRF24/RF24Mesh/commit/85a372f26bcd4efac5a5545a9a3d862f74ef78dd)

### <!-- 8 --> 📝 Documentation

- Manually trigger docs CI by \@2bndy5 in [`607f227`](https://github.com/nRF24/RF24Mesh/commit/607f227f2b100b6ccf4d48b36ca4b0e61038bd65)
- Doc updates and reusable CI by \@2bndy5 in [#217](https://github.com/nRF24/RF24Mesh/pull/217)

### <!-- 9 --> 🗨️ Changed

- Update examples for min PA by \@TMRh20 in [#216](https://github.com/nRF24/RF24Mesh/pull/216)
- Update for nrf52 by \@TMRh20 in [`b774f03`](https://github.com/nRF24/RF24Mesh/commit/b774f03bc8b2ba86ea1cc65e8581a7f1ad15e8b7)
- Update version for release by \@TMRh20 in [`e0ba23c`](https://github.com/nRF24/RF24Mesh/commit/e0ba23c2c6c641d91aca16cd25cd408744530004)

[1.1.9]: https://github.com/nRF24/RF24Mesh/compare/v1.1.8...v1.1.9

Full commit diff: [`v1.1.8...v1.1.9`][1.1.9]

## [1.1.8] - 2022-07-10

### <!-- 4 --> 🛠️ Fixed

- Fix mesh.begin() by \@TMRh20 in [#214](https://github.com/nRF24/RF24Mesh/pull/214)

### <!-- 9 --> 🗨️ Changed

- Minor version bump by \@2bndy5 in [`105386b`](https://github.com/nRF24/RF24Mesh/commit/105386baa2a13e24e595c1bd4f173b353b6c9f26)

[1.1.8]: https://github.com/nRF24/RF24Mesh/compare/v1.1.7...v1.1.8

Full commit diff: [`v1.1.7...v1.1.8`][1.1.8]

## [1.1.7] - 2022-07-09

### <!-- 1 --> 🚀 Added

- Add missing semi-colon from last commit by \@2bndy5 in [`1dada5f`](https://github.com/nRF24/RF24Mesh/commit/1dada5fe5b249420b1a7c926c3be9f30ce92dac7)
- Add topology to ncurses example by \@TMRh20 in [`9936933`](https://github.com/nRF24/RF24Mesh/commit/993693391a658002e23be7d8ce5475a4fb0b91b7)
- Merge pull request \#207 from nRF24/Topology by \@TMRh20 in [#207](https://github.com/nRF24/RF24Mesh/pull/207)
- Add clang-format config by \@2bndy5 in [`4ed33df`](https://github.com/nRF24/RF24Mesh/commit/4ed33df7f7b52e93bd8100f0fe83c8d7842c7fda)

### <!-- 3 --> 🗑️ Removed

- Remove rogue documented return for a void function by \@2bndy5 in [`ccb1b5f`](https://github.com/nRF24/RF24Mesh/commit/ccb1b5f9fc0b44dfbac2b761244d98e665d7b5d4)
- Remove comment about eeprom by \@TMRh20 in [`8bf14f9`](https://github.com/nRF24/RF24Mesh/commit/8bf14f92c2187a146a8ebd696afdee5d8e497800)

### <!-- 4 --> 🛠️ Fixed

- Fix python wrappers linking order by \@2bndy5 in [`559bf57`](https://github.com/nRF24/RF24Mesh/commit/559bf570ba070e5221faf3fcc31e19e6ae470b54)
- Fix setup.py for no IRQ support by \@2bndy5 in [`3a5cdb7`](https://github.com/nRF24/RF24Mesh/commit/3a5cdb7856aa6c39e64fa7385f8066c037174d50)

### <!-- 8 --> 📝 Documentation

- [docs] clarify renewAddress return val on failure by \@2bndy5 in [`804f7e6`](https://github.com/nRF24/RF24Mesh/commit/804f7e6baf6e6c1d7cbb4b8f6bcd05ecf7b4da72)
- Various improvments to docs by \@2bndy5 in [#209](https://github.com/nRF24/RF24Mesh/pull/209)
- Update .readthedocs.yaml by \@2bndy5 in [`afe7ce5`](https://github.com/nRF24/RF24Mesh/commit/afe7ce504a98b72c0d56ba54b94a3294682c1739)
- Update sphinx doc config by \@2bndy5 in [`a61b390`](https://github.com/nRF24/RF24Mesh/commit/a61b390397e9075a3ef63121c0c713d9e6b421bb)

### <!-- 9 --> 🗨️ Changed

- Only run linux CI on "created release" events by \@2bndy5 in [`6f8091e`](https://github.com/nRF24/RF24Mesh/commit/6f8091e1e0f63f68ea8b7b516ded4c6ebcca904f)
- Clean up ncurses example by \@TMRh20 in [`0e739e0`](https://github.com/nRF24/RF24Mesh/commit/0e739e0b77ca0aae14b3b722a08da4d71fe0ddff)
- Astyle formatting ncurses example by \@TMRh20 in [`49fd7da`](https://github.com/nRF24/RF24Mesh/commit/49fd7da6283ffa78b7aa625a54bbe6f0c0d2c7ad)
- Initialize meshStarted as false by \@2bndy5 in [`10e3961`](https://github.com/nRF24/RF24Mesh/commit/10e3961fac9ce7a8765389ade7fc7239d472e764)
- Properly init addrMemAllocated by \@2bndy5 in [`aacb9dc`](https://github.com/nRF24/RF24Mesh/commit/aacb9dc00cdc2cec871d1c49bc08a502beb4f94b)
- Modify examples by \@2bndy5 in [`a9a7fc0`](https://github.com/nRF24/RF24Mesh/commit/a9a7fc0b5c2aa306bd967aa8e882c890591bd565)
- More reconnecting loops; fix a use of renewAddress by \@2bndy5 in [`1123f19`](https://github.com/nRF24/RF24Mesh/commit/1123f19dbd7b23d90a67e22c74fae4da84e25288)
- Use do while (duh) by \@2bndy5 in [`59985d5`](https://github.com/nRF24/RF24Mesh/commit/59985d5c314f01b3d77f6ed44060396773aaef93)
- Merge pull request \#205 from nRF24/better-arduino-example-setup by \@TMRh20 in [#205](https://github.com/nRF24/RF24Mesh/pull/205)
- [CMake] link Linux examples to pigpio if detected by \@2bndy5 in [`2269f5e`](https://github.com/nRF24/RF24Mesh/commit/2269f5e3b3255d3b41bc7cccaa35a8ae04914658)
- [cmake] build examples w/ a specified driver by \@2bndy5 in [`206ce59`](https://github.com/nRF24/RF24Mesh/commit/206ce59a86ed950dfc5ccb6552ee1de971fef98e)
- [cmake] forgot the ncurses example by \@2bndy5 in [`26fa1a6`](https://github.com/nRF24/RF24Mesh/commit/26fa1a6173e11ec70fb4f82dab62373ea8948d86)
- -DRF24_NO_IRQ when no pigpio while building lib by \@2bndy5 in [`02d772a`](https://github.com/nRF24/RF24Mesh/commit/02d772a35bd8d6f2d246f80cb63ffa7be35bb195)
- Allow RF24_NO_IRQ for building examples by \@2bndy5 in [`e4021ab`](https://github.com/nRF24/RF24Mesh/commit/e4021abb5d527cee089a76cceb44aaa80c751ded)
- Merge branch 'pigpio-support' by \@TMRh20 in [`f6aec9f`](https://github.com/nRF24/RF24Mesh/commit/f6aec9f0d943eb0c39935676092e14a6a6fd9a7e)
- Improve doxygen CI by \@2bndy5 in [`7fd7d0f`](https://github.com/nRF24/RF24Mesh/commit/7fd7d0f46a9a787b0f008653bd45700537ff89f2)
- Resolve \#211 by \@2bndy5 in [`a3bc47d`](https://github.com/nRF24/RF24Mesh/commit/a3bc47d97244b3a30d92baf16f20fddd7131f7cd)
- Update python examples per testing by \@2bndy5 in [`1851ff4`](https://github.com/nRF24/RF24Mesh/commit/1851ff4840b644cae5b438e084ccfe8318252172)
- Merge pull request \#212 from nRF24/update-py-examples by \@TMRh20 in [#212](https://github.com/nRF24/RF24Mesh/pull/212)
- Ran clang-format on all C++ sources by \@2bndy5 in [`f40a116`](https://github.com/nRF24/RF24Mesh/commit/f40a116e53568c760830ee228586811e380e7b0e)
- Arduino CI make PR size delta reports by \@2bndy5 in [`9f6ac9b`](https://github.com/nRF24/RF24Mesh/commit/9f6ac9b5d67cce121249a550e8d66792072c502e)
- Update workflows by \@2bndy5 in [`7f6c717`](https://github.com/nRF24/RF24Mesh/commit/7f6c717e4d5fd269a90e49bceb02a9f02aa3ec23)
- MESH_CALLBACK macro must be 2 lines by \@2bndy5 in [`5cc0967`](https://github.com/nRF24/RF24Mesh/commit/5cc09671d4bd555e68557efdb93305a389e7c789)
- Update build_linux.yml by \@2bndy5 in [`c785a09`](https://github.com/nRF24/RF24Mesh/commit/c785a09ac1974d134284484e05b7452463657f8a)
- Update build_rp2xxx.yml by \@2bndy5 in [`35d1135`](https://github.com/nRF24/RF24Mesh/commit/35d113592584035e72a08912bdc7c4bd8bfc3ac8)
- Only check pico examples in rp2xxx CI by \@2bndy5 in [`557dac5`](https://github.com/nRF24/RF24Mesh/commit/557dac52397272a7fc5457506d0db17968751769)
- Merge pull request \#213 from nRF24/clang-format by \@TMRh20 in [#213](https://github.com/nRF24/RF24Mesh/pull/213)
- Disable latex builds on RTD by \@2bndy5 in [`250ccc1`](https://github.com/nRF24/RF24Mesh/commit/250ccc1aaf29083bb8f6377369c7899e5b4de05e)
- Minor version bump by \@2bndy5 in [`3b6c2e0`](https://github.com/nRF24/RF24Mesh/commit/3b6c2e0a36fb90637179cba9fdde478782c1bdb3)

[1.1.7]: https://github.com/nRF24/RF24Mesh/compare/v1.1.6...v1.1.7

Full commit diff: [`v1.1.6...v1.1.7`][1.1.7]

## [1.1.6] - 2021-11-27

### <!-- 1 --> 🚀 Added

- Add GPLv2 license; this builds locally by \@2bndy5 in [`a90920a`](https://github.com/nRF24/RF24Mesh/commit/a90920ab3782b568734f5e6e419af8ae6521370a)
- Add CMake user preset json to ignore by \@2bndy5 in [`b10b4b5`](https://github.com/nRF24/RF24Mesh/commit/b10b4b5bf18f2cf984ca8c81ee6db1b80d9095d0)
- Add while(!Serial) and if(!mesh.begin()) by \@2bndy5 in [`2e76b79`](https://github.com/nRF24/RF24Mesh/commit/2e76b7995f1327ea6fc467a8db48affb39e5ea25)
- Add comment about double check by \@2bndy5 in [`6f8285f`](https://github.com/nRF24/RF24Mesh/commit/6f8285f890a7cfc1497f2669f8dc33e69a72244f)
- Add MESH_SLOW_ADDRESS_RESPONSE; by \@2bndy5 in [`2d6c1ca`](https://github.com/nRF24/RF24Mesh/commit/2d6c1caa7bcd67c2cc5f357d5aab580ab8d1fd2d)
- Address \#195 by \@2bndy5 in [`af43cb2`](https://github.com/nRF24/RF24Mesh/commit/af43cb229a0895f16ba6644f858c2a7752815ba5)
- Add PicoSDK CI workflow by \@2bndy5 in [`2ee51d3`](https://github.com/nRF24/RF24Mesh/commit/2ee51d37ea5b7047cfd7c7f5003192942a43903f)
- Merge pull request \#188 from nRF24/CMake-4-Linux by \@TMRh20 in [#188](https://github.com/nRF24/RF24Mesh/pull/188)

### <!-- 3 --> 🗑️ Removed

- Remove artifact; resolves \#190 by \@2bndy5 in [`656bca1`](https://github.com/nRF24/RF24Mesh/commit/656bca17e8b92157abf74c160b5175654c859a5b)

### <!-- 4 --> 🛠️ Fixed

- Fix code fmt; add ncurses example to cmake targets by \@2bndy5 in [`20bb320`](https://github.com/nRF24/RF24Mesh/commit/20bb3206f5ad5f01423147ab25da22aa62dea94f)
- Fix docs about addrList by \@2bndy5 in [`819eb30`](https://github.com/nRF24/RF24Mesh/commit/819eb30650da51bb6d16395c1cde8d46684ee965)
- Fix problem in last commit by \@2bndy5 in [`fc48984`](https://github.com/nRF24/RF24Mesh/commit/fc4898454d34005afcd3e0c286c027dce26f764b)
- Fix broken links on docs main pg by \@2bndy5 in [`578e541`](https://github.com/nRF24/RF24Mesh/commit/578e54147bf09fe289617b535b7256b4034464db)
- Fix compile for examples by \@TMRh20 in [`e1eb4a9`](https://github.com/nRF24/RF24Mesh/commit/e1eb4a966064081d448e36df7c87b40c9b33b6ac)
- Fix example compilation by \@TMRh20 in [`b6c6f1c`](https://github.com/nRF24/RF24Mesh/commit/b6c6f1c878270c8dd988945e1afe6d4d425f3b31)

### <!-- 8 --> 📝 Documentation

- Ready for docs hosting on RTD by \@2bndy5 in [`0981279`](https://github.com/nRF24/RF24Mesh/commit/0981279491889aa414e5387924fa679347663f98)
- Small docs update by \@2bndy5 in [`9b94c34`](https://github.com/nRF24/RF24Mesh/commit/9b94c34b82b8aed1d4db630600dd1384e267dd3b)
- [docs] fix multiline API signatures by \@2bndy5 in [`a8a7f21`](https://github.com/nRF24/RF24Mesh/commit/a8a7f210158b7e17b7f775626ab1d0a83e2dab77)
- Update for Docs by \@TMRh20 in [`c9a691d`](https://github.com/nRF24/RF24Mesh/commit/c9a691d9d487a4403e9cb238db01960a161cfc97)

### <!-- 9 --> 🗨️ Changed

- Initial CMake files for pico-sdk by \@kripton in [#185](https://github.com/nRF24/RF24Mesh/pull/185)
- [no ci] preliminary CMake implementation by \@2bndy5 in [`7387ea1`](https://github.com/nRF24/RF24Mesh/commit/7387ea1555ac0c76237158d34e5bfc577113c441)
- Avoid duplicate triggers when PR is open by \@2bndy5 in [`2fa51c5`](https://github.com/nRF24/RF24Mesh/commit/2fa51c544590c826575cfa9f12712c9b006054d4)
- No need for --target with make cmd by \@2bndy5 in [`4ee2e51`](https://github.com/nRF24/RF24Mesh/commit/4ee2e51d474020d032cb3834690949113524128a)
- Automate building of ncurses example by \@2bndy5 in [`d1c3217`](https://github.com/nRF24/RF24Mesh/commit/d1c321755a09f2df525303e1387a351dd14d312e)
- Default BUILD_TYPE to Release by \@2bndy5 in [`a1ecabc`](https://github.com/nRF24/RF24Mesh/commit/a1ecabc25bd809a8fc11635506331818190e6c5b)
- Update CONTRIBUTING.md by \@2bndy5 in [`1452062`](https://github.com/nRF24/RF24Mesh/commit/14520623c2254dc461b50019b5326a0888391626)
- Manually trigger CI workflows by \@2bndy5 in [`903c6d7`](https://github.com/nRF24/RF24Mesh/commit/903c6d798c81985785f8016d4d34e01d44d9f94d)
- Doxygen CI triggers on push & PR for all branches by \@2bndy5 in [`ff92d8e`](https://github.com/nRF24/RF24Mesh/commit/ff92d8e8c8105c39f0177cfadb4b843bf8a8c636)
- Try new Arduino CI workflow by \@2bndy5 in [`0902b49`](https://github.com/nRF24/RF24Mesh/commit/0902b4932ded951858a116963139eb9022659b10)
- Forgot to add arduino examples fmt conf file by \@2bndy5 in [`4471b05`](https://github.com/nRF24/RF24Mesh/commit/4471b0575ee5682628e2ee095cd8e40eb86ee80b)
- RF24Mesh_Example_Master is incompatible w/ non-AVR by \@2bndy5 in [`f0bc5e1`](https://github.com/nRF24/RF24Mesh/commit/f0bc5e1fe5a89fd9c3f495bf61a8449f7614599e)
- Gimme them badges by \@2bndy5 in [`f2e3167`](https://github.com/nRF24/RF24Mesh/commit/f2e316777a3e7ef6d6ded7ce37bb13b0253c9ac3)
- Try new PlatformIO CI by \@2bndy5 in [`69b1548`](https://github.com/nRF24/RF24Mesh/commit/69b154803507f7e41b6d363cd42ba4fd334fc120)
- Oops use linux FS delimiter by \@2bndy5 in [`e16f600`](https://github.com/nRF24/RF24Mesh/commit/e16f6005c14bf7e9c6c009fdbc6e51cb1a717ef1)
- Gimme a badge by \@2bndy5 in [`a9f8313`](https://github.com/nRF24/RF24Mesh/commit/a9f83131c7a7acb93cac16e9b9a2b8bf74df3724)
- Polish library.json by \@2bndy5 in [`0a98838`](https://github.com/nRF24/RF24Mesh/commit/0a988380f597dcb580d446e41fcb21ddf424381b)
- [PIO CI] validate library.json & save as artifact by \@2bndy5 in [`85ad20c`](https://github.com/nRF24/RF24Mesh/commit/85ad20c769d5f7f2d9e0cb86ca55c59d9faf5bbc)
- [PIO CI] fix path to pio pkg by \@2bndy5 in [`29f7294`](https://github.com/nRF24/RF24Mesh/commit/29f7294b7e26198e18a5490036206e72507f5deb)
- Exclude linux stuff from PIO by \@2bndy5 in [`31d7ece`](https://github.com/nRF24/RF24Mesh/commit/31d7ecebba93332321589f88a3fb302f82c31f1f)
- Exclude more useless stuff from PIO pkg by \@2bndy5 in [`76f99b0`](https://github.com/nRF24/RF24Mesh/commit/76f99b0ecf361a8238ea4f499b01be3f7e024f3a)
- Use CMake CLI to change RF24Mesh_config.h macros by \@2bndy5 in [`f0dc453`](https://github.com/nRF24/RF24Mesh/commit/f0dc4536a91a995de8da2e23ee692628582fa0a5)
- [no ci] adjust some whitespace by \@2bndy5 in [`ea7746c`](https://github.com/nRF24/RF24Mesh/commit/ea7746c9d1a5081c308507c0b8d856004331690d)
- Use new macro propsed in nRF24/RF24Network\#179 by \@2bndy5 in [`a47b242`](https://github.com/nRF24/RF24Mesh/commit/a47b242d7f3f12a4b3003947605d5305dc71c36d)
- Following NASA C style guide by \@2bndy5 in [`d79fec3`](https://github.com/nRF24/RF24Mesh/commit/d79fec3861cebf7ae1e1d2cbe079bc393d0557b9)
- Uniform MESH_DEBUG* calls by \@2bndy5 in [`10ce90e`](https://github.com/nRF24/RF24Mesh/commit/10ce90e72f75f168f9913994760039c227e2a006)
- Alias NET_MULTICAST_ADDR by \@2bndy5 in [`6d130d8`](https://github.com/nRF24/RF24Mesh/commit/6d130d80f08fe8184e0c0d5cbab7ece38e4d5fe9)
- Trigger Arduino CI workflow by \@2bndy5 in [`057a2e5`](https://github.com/nRF24/RF24Mesh/commit/057a2e58f452356ba9f9bdb3676da78867799e05)
- Trigger PlatformIO CI by \@2bndy5 in [`31039cf`](https://github.com/nRF24/RF24Mesh/commit/31039cf62e14e96588759f58402cd4b36c3c460c)
- [Arduino CI] url option must end in .git by \@2bndy5 in [`ce300b6`](https://github.com/nRF24/RF24Mesh/commit/ce300b64f7bc8a82db134ede064a56df3675884e)
- Use %iu instead of %ui by \@2bndy5 in [`9af7858`](https://github.com/nRF24/RF24Mesh/commit/9af78584adb14dedc8711011962ff6c0419afa05)
- Use just %u by \@2bndy5 in [`aaed824`](https://github.com/nRF24/RF24Mesh/commit/aaed824be7e70f39c592639efdaeafb264f89ef9)
- [py wrapper] augment setup.py by \@2bndy5 in [`fcf70ab`](https://github.com/nRF24/RF24Mesh/commit/fcf70ab38680651cb59988d3b699aaeec94c20fc)
- Use an OR condition instead by \@2bndy5 in [`be5d66c`](https://github.com/nRF24/RF24Mesh/commit/be5d66c03b2b4b89b90c02e61bd86ee1c0ac5e5d)
- Revert to nested if statements by \@2bndy5 in [`3a776de`](https://github.com/nRF24/RF24Mesh/commit/3a776de12e8b87ae8107b06e166795083446a8f0)
- Merge branch 'CMake-4-Linux' of https://github.com/nRF24/RF24Mesh into CMake-4-Linux by \@2bndy5 in [`928ed19`](https://github.com/nRF24/RF24Mesh/commit/928ed196c3f85cdb53665ddba22f35f316ac4094)
- Py pkgs from apt are very out-of-date by \@2bndy5 in [`61a7f91`](https://github.com/nRF24/RF24Mesh/commit/61a7f913ea86f40ba6b510daa9ea537770dc3a87)
- [py wrapper] ammend license info by \@2bndy5 in [`b78d5cd`](https://github.com/nRF24/RF24Mesh/commit/b78d5cd1c81dc9377c7de0c9d17d590a0ccf8248)
- Move new mcro up 1 OSI lvl by \@2bndy5 in [`95c75b1`](https://github.com/nRF24/RF24Mesh/commit/95c75b151643f50675acbf7abcf64a6a38b2fe54)
- Revert SLOW_ADDR_POLL_RESPONSE by \@2bndy5 in [`4afe8c8`](https://github.com/nRF24/RF24Mesh/commit/4afe8c8059fbb52a46ae45a981b179fdd126e65c)
- Revert "revert SLOW_ADDR_POLL_RESPONSE" by \@2bndy5 in [`3b69930`](https://github.com/nRF24/RF24Mesh/commit/3b69930bceed9ad1f7ed599b68807594c8f58115)
- Relative CMakeLists.txt by \@2bndy5 in [`0160dd2`](https://github.com/nRF24/RF24Mesh/commit/0160dd2fd0b9bdb9e65459cd9263f1256af1f756)
- [CMake] distinguish project_option/warnings by \@2bndy5 in [`951bbde`](https://github.com/nRF24/RF24Mesh/commit/951bbde8e0597931075b0c482b5dee4690049376)
- Allow pybind11 to build in source by \@2bndy5 in [`f01e3bb`](https://github.com/nRF24/RF24Mesh/commit/f01e3bbb8db82fada688b9cfd1b5abd93d820c0e)
- Introduce undefined macro USE_RF24_LIB_SRC by \@2bndy5 in [`c88e79d`](https://github.com/nRF24/RF24Mesh/commit/c88e79dcb4550f8d8e8a4a0bcccccce955b24a73)
- Further the utility of USE_RF24_LIB_SRC macro by \@2bndy5 in [`0e9af67`](https://github.com/nRF24/RF24Mesh/commit/0e9af6790e4bb96d982eaf4b31ac215626689cfc)
- [CMakeLists.txt] fix last commit by \@2bndy5 in [`42cd430`](https://github.com/nRF24/RF24Mesh/commit/42cd43012ee9f11a703f916e9faabe63a15c787d)
- Auto-publish new releases to PIO by \@2bndy5 in [`51e9e8b`](https://github.com/nRF24/RF24Mesh/commit/51e9e8bdb32a817e2cc2a2b47c1a7d8772546522)
- Use --non-interactive option in PIO CI by \@2bndy5 in [`91198db`](https://github.com/nRF24/RF24Mesh/commit/91198dbc2a2128167d1eb52d86efeb134cacbd81)
- [linux CI] use RF24 master branch by \@2bndy5 in [`f19b503`](https://github.com/nRF24/RF24Mesh/commit/f19b503bbc3b7c4191ca755fc2b0110673dec401)
- Review pico examples by \@2bndy5 in [`a20bf32`](https://github.com/nRF24/RF24Mesh/commit/a20bf32067b589902fbd8163c5602e525d2b7383)
- Pico examples should wait until Serial is active by \@2bndy5 in [`e38cc31`](https://github.com/nRF24/RF24Mesh/commit/e38cc31628cd54e541dd56be153dd525d5973ac4)
- [pico examples] use milliseconds not microseconds by \@2bndy5 in [`4db1ab4`](https://github.com/nRF24/RF24Mesh/commit/4db1ab47774bf31c3df9f69210fbff982c8d9eb6)
- Node ID max is 255 by \@2bndy5 in [`9e34e87`](https://github.com/nRF24/RF24Mesh/commit/9e34e875f43afac5b823c05036b7adf522144587)
- Gimme that RTD badge by \@2bndy5 in [`e73cca0`](https://github.com/nRF24/RF24Mesh/commit/e73cca08dcd1c5c00edbf4fe44da247e3dfd3753)
- Use RF24Network master branch in linux workflow by \@2bndy5 in [`67d25b9`](https://github.com/nRF24/RF24Mesh/commit/67d25b91a5828adc92bc437a3cf1b6b26a8bfb0d)
- Use RF24Network master branch in arduino workflow by \@2bndy5 in [`0df592e`](https://github.com/nRF24/RF24Mesh/commit/0df592ed59d926d414e64cdf86a3384ca85c44dc)
- Use RF24Network master branch in PIO workflow by \@2bndy5 in [`13cae65`](https://github.com/nRF24/RF24Mesh/commit/13cae652b35d76ba1dfb93889bebabfbe4d326fc)
- Use RF24* master branch in rp2xxx workflow by \@2bndy5 in [`a33695b`](https://github.com/nRF24/RF24Mesh/commit/a33695b6a220b05426feb75759be4abd079f621a)
- Update pyRF24Mesh.cpp by \@RasmusHRAAU in [#179](https://github.com/nRF24/RF24Mesh/pull/179)
- MESH_CALLBACK and eliminate delay in renewAddress() by \@BCsabaEngine in [`804c68f`](https://github.com/nRF24/RF24Mesh/commit/804c68f094db1362b8f9074b4d1bd5958929fdde)
- Merge pull request \#194 from BCsabaEngine/master by \@TMRh20 in [#194](https://github.com/nRF24/RF24Mesh/pull/194)
- Merge branch 'master' of https://github.com/nRF24/RF24Mesh by \@TMRh20 in [`3f60d8b`](https://github.com/nRF24/RF24Mesh/commit/3f60d8ba07b1b821b6d7dbdcdd22ca11046c3512)
- Merge branch 'master' into CMake-4-Linux by \@2bndy5 in [`b3915b2`](https://github.com/nRF24/RF24Mesh/commit/b3915b248e98b5a4df52d5aab623229ae990abb7)

[1.1.6]: https://github.com/nRF24/RF24Mesh/compare/v1.1.5...v1.1.6

Full commit diff: [`v1.1.5...v1.1.6`][1.1.6]

## New Contributors
* \@BCsabaEngine made their first contribution
* \@RasmusHRAAU made their first contribution in [#179](https://github.com/nRF24/RF24Mesh/pull/179)
## [1.1.5] - 2021-04-13

### <!-- 8 --> 📝 Documentation

- Docs abstraction & code formating by \@2bndy5 in [#181](https://github.com/nRF24/RF24Mesh/pull/181)

### <!-- 9 --> 🗨️ Changed

- Merge branch 'master' into master by \@2bndy5 in [`749c56a`](https://github.com/nRF24/RF24Mesh/commit/749c56a8194622933bec9a6e8d5e924340d21522)
- Merge pull request \#152 from positron96/master by \@TMRh20 in [#152](https://github.com/nRF24/RF24Mesh/pull/152)
- Make most \#defines in RF24Mesh_config.h conditional so they can be set from project code or compiler command line. by \@kripton in [#183](https://github.com/nRF24/RF24Mesh/pull/183)
- Version bump (patch+1) by \@2bndy5 in [`3acca6e`](https://github.com/nRF24/RF24Mesh/commit/3acca6e4035ae2fc36e9cd53c3f0e07a02a868c5)

[1.1.5]: https://github.com/nRF24/RF24Mesh/compare/v1.1.4...v1.1.5

Full commit diff: [`v1.1.4...v1.1.5`][1.1.5]

## New Contributors
* \@kripton made their first contribution in [#183](https://github.com/nRF24/RF24Mesh/pull/183)
## [1.1.4] - 2020-12-14

### <!-- 4 --> 🛠️ Fixed

- Fix warning by \@2bndy5 in [`e75fc07`](https://github.com/nRF24/RF24Mesh/commit/e75fc07e332c570add942e995fc54074e4a7c59b)
- Fix missing images by \@2bndy5 in [`50712c8`](https://github.com/nRF24/RF24Mesh/commit/50712c880b51e3dd8c7758a7c6fc0f97783fb0f7)

### <!-- 8 --> 📝 Documentation

- Redirect doc links to nRF24 org by \@2bndy5 in [`3e6e5a8`](https://github.com/nRF24/RF24Mesh/commit/3e6e5a8f94a87a8af5420c1f2afefef33280484c)
- Merge pull request \#177 from 2bndy5/master by \@TMRh20 in [#177](https://github.com/nRF24/RF24Mesh/pull/177)

### <!-- 9 --> 🗨️ Changed

- Create CONTRIBUTING.md by \@Avamander in [`870a31d`](https://github.com/nRF24/RF24Mesh/commit/870a31d183c23e72cbfac803cfc02755591723cb)
- Edit doxyfile OUTPUT_DIR & add workflow by \@2bndy5 in [`6f1b126`](https://github.com/nRF24/RF24Mesh/commit/6f1b126558cebf7441cbf878437dddf62acbf7c2)
- Format doxygen css file & include latest changes by \@2bndy5 in [`44bde34`](https://github.com/nRF24/RF24Mesh/commit/44bde349a7943470be1130f5795db278ef73d37b)
- Only run doxygen on master branch by \@2bndy5 in [`60ceaa8`](https://github.com/nRF24/RF24Mesh/commit/60ceaa8023a7da6f61b65af446bf72fd184bd3e7)
- Merge pull request \#178 from 2bndy5/master by \@2bndy5 in [#178](https://github.com/nRF24/RF24Mesh/pull/178)
- Update library.properties by \@TMRh20 in [`8442df9`](https://github.com/nRF24/RF24Mesh/commit/8442df9cb2771c73f22df2c035408a074b2f2d93)
- Update library.json by \@TMRh20 in [`f098df0`](https://github.com/nRF24/RF24Mesh/commit/f098df063fe6e2200c96a760ecbd51215d7acfbd)

[1.1.4]: https://github.com/nRF24/RF24Mesh/compare/v1.1.3...v1.1.4

Full commit diff: [`v1.1.3...v1.1.4`][1.1.4]

## New Contributors
* \@2bndy5 made their first contribution in [#178](https://github.com/nRF24/RF24Mesh/pull/178)
## [1.1.3] - 2020-08-17

### <!-- 1 --> 🚀 Added

- Add ability to set call-back function by \@TMRh20 in [`c2354ff`](https://github.com/nRF24/RF24Mesh/commit/c2354ff959f0b4b32d73254fa37b3242932dd496)

### <!-- 3 --> 🗑️ Removed

- Many bug fixes by \@TMRh20 in [`f445630`](https://github.com/nRF24/RF24Mesh/commit/f44563093681fdfe44a7f5c6eedf78c4b985606a)

### <!-- 9 --> 🗨️ Changed

- Doc update by \@TMRh20 in [`43f8445`](https://github.com/nRF24/RF24Mesh/commit/43f84455ce75acf69eb73f748f7f739708a98e1b)
- Adjust setAddress function by \@TMRh20 in [`7a4a8e5`](https://github.com/nRF24/RF24Mesh/commit/7a4a8e5349120056198baa1ea319f526bdc1b45e)
- Update for release by \@TMRh20 in [`fce5235`](https://github.com/nRF24/RF24Mesh/commit/fce52350416070d3fae40a866119010fad631cc8)

[1.1.3]: https://github.com/nRF24/RF24Mesh/compare/v1.1.2...v1.1.3

Full commit diff: [`v1.1.2...v1.1.3`][1.1.3]

## [1.1.2] - 2020-08-08

### <!-- 1 --> 🚀 Added

- Address request/renew improvement by \@TMRh20 in [`2af9653`](https://github.com/nRF24/RF24Mesh/commit/2af965350d55a219bed9344fc79b7e28fe9b8207)
- Address one more issue by \@TMRh20 in [`9516910`](https://github.com/nRF24/RF24Mesh/commit/951691059b7f49a0aa741f9a873f8fc5baad26cb)

### <!-- 3 --> 🗑️ Removed

- Fixes,cleanup and improvements by \@TMRh20 in [`8be00fd`](https://github.com/nRF24/RF24Mesh/commit/8be00fde92febe8c7e4bda26c1c951b6574c78ff)
- Minor correction on master nodes by \@TMRh20 in [`bbc996b`](https://github.com/nRF24/RF24Mesh/commit/bbc996bbf33501deee153fea3c6a627cc534445f)
- More minor cleanup by \@TMRh20 in [`c957217`](https://github.com/nRF24/RF24Mesh/commit/c957217c3cd4951c5dfa98139112c049e504be7f)
- Adjust for RF24Network change by \@TMRh20 in [`18d14e7`](https://github.com/nRF24/RF24Mesh/commit/18d14e786fcd3deca8c7db291723e6edff0de263)

### <!-- 9 --> 🗨️ Changed

- One little adjustment by \@TMRh20 in [`7c7947c`](https://github.com/nRF24/RF24Mesh/commit/7c7947c86d3222de07971aea5b49d53cc2f13ec8)
- Update for release by \@TMRh20 in [`b1430b1`](https://github.com/nRF24/RF24Mesh/commit/b1430b1a1f71f8949977503e0b07eb833074cf2c)

[1.1.2]: https://github.com/nRF24/RF24Mesh/compare/v1.1.1...v1.1.2

Full commit diff: [`v1.1.1...v1.1.2`][1.1.2]

## [1.1.1] - 2020-08-01

### <!-- 3 --> 🗑️ Removed

- BIg cleanup and some fixes by \@TMRh20 in [`ea116be`](https://github.com/nRF24/RF24Mesh/commit/ea116be29b6b04b00927ac6340d18906f65af9e6)

### <!-- 4 --> 🛠️ Fixed

- Issue with renewAddress() by \@TMRh20 in [`e00a9d5`](https://github.com/nRF24/RF24Mesh/commit/e00a9d522fc217257298e653001a68e2afb3e60b)

### <!-- 9 --> 🗨️ Changed

- Dynamic memory allocation & fixes by \@TMRh20 in [`b30b182`](https://github.com/nRF24/RF24Mesh/commit/b30b18281cf64812266b8e4db0b823e3cd088fef)
- Update version & architectures by \@TMRh20 in [`e0fd05c`](https://github.com/nRF24/RF24Mesh/commit/e0fd05c225a58fc5f31342ee90b77ad79d074e67)
- Update version for release by \@TMRh20 in [`7e5e920`](https://github.com/nRF24/RF24Mesh/commit/7e5e920ae82fa03942505fccba1e78ed7aa734d4)

[1.1.1]: https://github.com/nRF24/RF24Mesh/compare/v1.1.0...v1.1.1

Full commit diff: [`v1.1.0...v1.1.1`][1.1.1]

## [1.1.0] - 2020-06-08

### <!-- 9 --> 🗨️ Changed

- Update versioning to fix problems by \@TMRh20 in [`5563474`](https://github.com/nRF24/RF24Mesh/commit/556347426608ffb4d5f4df0d64474c605bf9ffa5)

[1.1.0]: https://github.com/nRF24/RF24Mesh/compare/v1.0.8...v1.1.0

Full commit diff: [`v1.0.8...v1.1.0`][1.1.0]

## [1.0.8] - 2020-06-08

### <!-- 7 -->🚦 Tests

- CheckConnection verify connection both ways by \@TMRh20 in [`eacd24f`](https://github.com/nRF24/RF24Mesh/commit/eacd24f63e3f3cccfecb2997696070cd06fa3cc2)

### <!-- 9 --> 🗨️ Changed

- Update library.properties by \@TMRh20 in [`409e409`](https://github.com/nRF24/RF24Mesh/commit/409e4092296b199250c6d79e64c1515c641d97b2)
- Update version for release by \@TMRh20 in [`d4c8e68`](https://github.com/nRF24/RF24Mesh/commit/d4c8e68b8b4f88e3e70222739b1d5718145e1422)

[1.0.8]: https://github.com/nRF24/RF24Mesh/compare/v1.0.7...v1.0.8

Full commit diff: [`v1.0.7...v1.0.8`][1.0.8]

## [1.0.7] - 2019-05-26

### <!-- 1 --> 🚀 Added

- Add mesh.begin() after failed renewal of address by \@TMRh20 in [`4b036ce`](https://github.com/nRF24/RF24Mesh/commit/4b036cef093a2a262d88960616a2b4ca67f9debb)
- Add one more step to address renewal by \@TMRh20 in [`c992646`](https://github.com/nRF24/RF24Mesh/commit/c992646742918e1ed479829c895e83b8677f1715)
- Add a second lookup to the end of address renewal by \@TMRh20 in [`c27604a`](https://github.com/nRF24/RF24Mesh/commit/c27604a4dfb173a97fec0ad30f880b2f40442c18)

### <!-- 3 --> 🗑️ Removed

- Remove address confirmations from address renewal by \@TMRh20 in [`92a03c0`](https://github.com/nRF24/RF24Mesh/commit/92a03c0c00a3a82c42704222dee46494e8da8ac4)

### <!-- 8 --> 📝 Documentation

- Update RF24Mesh.h documentation by \@TMRh20 in [`6252c3a`](https://github.com/nRF24/RF24Mesh/commit/6252c3ab3896a9cc0fa52011caae51056bfe57dd)

### <!-- 9 --> 🗨️ Changed

- Correct debug output by \@TMRh20 in [`c7f64a6`](https://github.com/nRF24/RF24Mesh/commit/c7f64a6c37d798104c8239063df3184d278bda38)
- Change timeout for nodeID lookups by \@TMRh20 in [`e7f90d8`](https://github.com/nRF24/RF24Mesh/commit/e7f90d898620f7d5d848d77b39b93a47ed912b24)
- Update readme by \@TMRh20 in [`67bc9ab`](https://github.com/nRF24/RF24Mesh/commit/67bc9ab7b9bab5522382d286bd26a1839f1d0f5e)
- Update version for release by \@TMRh20 in [`7760101`](https://github.com/nRF24/RF24Mesh/commit/7760101df09402ba7daf653431971486a2877965)

[1.0.7]: https://github.com/nRF24/RF24Mesh/compare/v1.0.6...v1.0.7

Full commit diff: [`v1.0.6...v1.0.7`][1.0.7]

## [1.0.6] - 2019-05-08

### <!-- 1 --> 🚀 Added

- Adds back the delay in DHCP as without address renewal fails by \@MAKOMO in [`79ac487`](https://github.com/nRF24/RF24Mesh/commit/79ac487ed025d7362f71ada9581409a259769cfe)
- Added compiler variable into makefiles for easy compiler switching by \@igormp in [`2022b93`](https://github.com/nRF24/RF24Mesh/commit/2022b937d6074294266187bffc148e6a081c84e4)
- Added compiler variable into ncurses makefile by \@igormp in [`63da607`](https://github.com/nRF24/RF24Mesh/commit/63da60709158535b5e6168576e0b2a221eff4e7c)
- Added default initialization value for bool by \@igormp in [`c31387a`](https://github.com/nRF24/RF24Mesh/commit/c31387abc051e2a246faa6eda9d5c87c1aaca9d8)
- Added .gitignore by \@igormp in [`8502bc4`](https://github.com/nRF24/RF24Mesh/commit/8502bc49a2fa1e9b4651be2679a842414e60c48b)
- Adds boost python wrapper by \@joernesdohr in [`e228902`](https://github.com/nRF24/RF24Mesh/commit/e228902faffc55ca896ce86e22174cd9c9936e57)
- Adds namespace by \@joernesdohr in [`d601161`](https://github.com/nRF24/RF24Mesh/commit/d601161dd4a7f6ccd612cfcf66abe4421d1a8ba3)
- Adds python example for master node by \@joernesdohr in [`d3a17b3`](https://github.com/nRF24/RF24Mesh/commit/d3a17b3269b3ade560fca08bad5eebfbff5aa39c)
- Adds missing function by \@joernesdohr in [`9c2de0f`](https://github.com/nRF24/RF24Mesh/commit/9c2de0f3e9a10f545f177d49efaf85ed17d0a626)
- Added missing throw_ba_exception() function by \@joernesdohr in [`4a1b9a8`](https://github.com/nRF24/RF24Mesh/commit/4a1b9a8889158feb612eb79edb47304a4e131105)
- Adds rf24network library reference to setup; fixes typo in setNodeId() function name by \@joernesdohr in [`68cce8d`](https://github.com/nRF24/RF24Mesh/commit/68cce8d7fc988b9989e3e80513419b1a76496c39)

### <!-- 3 --> 🗑️ Removed

- Removed unnecessary EEPROM include. Autoformat. by \@Avamander in [`9a4682f`](https://github.com/nRF24/RF24Mesh/commit/9a4682f5885cb50024d45dfa911e61758a4b39df)
- Merge pull request \#93 from Avamander/master by \@Avamander in [#93](https://github.com/nRF24/RF24Mesh/pull/93)
- Code Styling in [`8f20094`](https://github.com/nRF24/RF24Mesh/commit/8f200945245ca70fc47ef845a9c70afa9fed553a)
- Removes static_cast agains; adds [return_internal_reference<>()] as seen on http://www.boost.org/doc/libs/1_38_0/libs/python/doc/v2/overloads.html by \@joernesdohr in [`8121fda`](https://github.com/nRF24/RF24Mesh/commit/8121fdafcfb1327453c5d3cb9fecf420bb4ce9c2)
- Remove [bp::return_internal_reference<>()] because those functions don't actually return any internal references... by \@joernesdohr in [`93046a9`](https://github.com/nRF24/RF24Mesh/commit/93046a969b03f7c3c3d8b1313b910f7c3238d690)

### <!-- 4 --> 🛠️ Fixed

- Fixed issue described in \#86 by \@Avamander in [`836aa76`](https://github.com/nRF24/RF24Mesh/commit/836aa76ae90c7acb8199a3de70c06e3ee3a46740)
- Fixes arguments by \@joernesdohr in [`d2381c8`](https://github.com/nRF24/RF24Mesh/commit/d2381c8fbff71c4fdafbc07747a99beee8879ca8)

### <!-- 9 --> 🗨️ Changed

- Refactor if(nodeID()) in [`a990593`](https://github.com/nRF24/RF24Mesh/commit/a990593437fb794de87114e2ee812a284b001c6e)
- Merge pull request \#94 from ciberkids/master by \@TMRh20 in [#94](https://github.com/nRF24/RF24Mesh/pull/94)
- Links MESH_DEFAULT_ADDRESS NETWORK_DEFAULT_ADDRESS by \@MAKOMO in [`359b226`](https://github.com/nRF24/RF24Mesh/commit/359b226729324ef32695bffd357fbc8120a57611)
- Extends debug logging by \@MAKOMO in [`187f4d0`](https://github.com/nRF24/RF24Mesh/commit/187f4d0b47b2191f9db5c8cfb9bce6a29c9cc94e)
- Merge pull request \#139 from MAKOMO/master by \@TMRh20 in [#139](https://github.com/nRF24/RF24Mesh/pull/139)
- Check value of radio.begin by \@positron96 in [`e96477e`](https://github.com/nRF24/RF24Mesh/commit/e96477e6ea4fc0b101534f878b3d58644c8e0109)
- Merge pull request \#153 from igormp/makeupdate by \@Avamander in [#153](https://github.com/nRF24/RF24Mesh/pull/153)
- Split increment and assignment in order to avoid UB by \@igormp in [`33aad0b`](https://github.com/nRF24/RF24Mesh/commit/33aad0ba9f8a50981ca5a8019d2cb55df0b18939)
- Merge pull request \#154 from igormp/clangwarning by \@Avamander in [#154](https://github.com/nRF24/RF24Mesh/pull/154)
- Examples' makefiles now create a `bin` dir by \@igormp in [`37e0626`](https://github.com/nRF24/RF24Mesh/commit/37e0626bcb62094fef89683f6353645995cc57e6)
- Merge pull request \#155 from igormp/master by \@TMRh20 in [#155](https://github.com/nRF24/RF24Mesh/pull/155)
- Static_cast the overoaded wrappers by \@joernesdohr in [`faa87ee`](https://github.com/nRF24/RF24Mesh/commit/faa87ee7399e9e5c04f49d363f833c134df46ee3)
- Single bp:arg() instead of args by \@joernesdohr in [`f87bdfd`](https://github.com/nRF24/RF24Mesh/commit/f87bdfd0c42981808cdd56755b4d9fd9033cceb0)
- Back to bp::args; moves [bp::return_internal_reference<>()] into correct position by \@joernesdohr in [`3f79b2c`](https://github.com/nRF24/RF24Mesh/commit/3f79b2c82d77eb13b6682ecdc2a06997455b23dc)
- Trying manual wrapper functions by \@joernesdohr in [`27c3f47`](https://github.com/nRF24/RF24Mesh/commit/27c3f4788a232f237d0032fc16685a198ca99188)
- Wrapper functions for byte buffer parameters by \@joernesdohr in [`ebbe80d`](https://github.com/nRF24/RF24Mesh/commit/ebbe80df848ace5ec0e57a46d1c82bf97fb0c430)
- Example doesn't need to be a class by \@joernesdohr in [`0625000`](https://github.com/nRF24/RF24Mesh/commit/06250007c473e8dd972f4471e751070df1189246)
- Adopts correct capitalization of ID by \@joernesdohr in [`e3f833d`](https://github.com/nRF24/RF24Mesh/commit/e3f833d09207125b29cbc9683c9c4b04dfaf3e8b)
- Clean up by \@joernesdohr in [`7bbece9`](https://github.com/nRF24/RF24Mesh/commit/7bbece99ab79bb99b5eddf550240ddc035625608)
- Typo in comment by \@joernesdohr in [`d228ce6`](https://github.com/nRF24/RF24Mesh/commit/d228ce6674f2295c9f675fd6612d3b8baf2c2aea)
- Files moved to standard structure by \@filipek92 in [`c783fdb`](https://github.com/nRF24/RF24Mesh/commit/c783fdb64c30dd268185c7e7481ab9c42c341382)
- Python example RF24Mesh_Example_Master.py rewrited to work as C++ example by \@filipek92 in [`e628d65`](https://github.com/nRF24/RF24Mesh/commit/e628d65c2e1fd04b3315e19a0bd785ede9315feb)
- Mesh node example in Python 3 by \@filipek92 in [`7708cfc`](https://github.com/nRF24/RF24Mesh/commit/7708cfc9f4cbc615b759401394d973e7d51cd2b5)
- Merge pull request \#1 from nRF24/master by \@filipek92 in [`30d9fa2`](https://github.com/nRF24/RF24Mesh/commit/30d9fa219cbd928e9ef32d08f5f8f374ec31d64b)
- Merge pull request \#123 from filipek92/master by \@TMRh20 in [#123](https://github.com/nRF24/RF24Mesh/pull/123)
- Update library.json by \@Avamander in [`4ed90d3`](https://github.com/nRF24/RF24Mesh/commit/4ed90d33f2d8b5fd62260131212e81177bf713c6)
- Update library.properties by \@Avamander in [`5a1ec37`](https://github.com/nRF24/RF24Mesh/commit/5a1ec3797216cea1c3397296b03841cdec803697)
- Update version for release by \@TMRh20 in [`c76ab82`](https://github.com/nRF24/RF24Mesh/commit/c76ab82c9a07edef467a77d137a8707e0338793a)

[1.0.6]: https://github.com/nRF24/RF24Mesh/compare/v1.0.51...v1.0.6

Full commit diff: [`v1.0.51...v1.0.6`][1.0.6]

## New Contributors
* \@igormp made their first contribution
* \@positron96 made their first contribution
* \@filipek92 made their first contribution
* \@MAKOMO made their first contribution
* \@joernesdohr made their first contribution
## [1.0.51] - 2016-06-04

### <!-- 3 --> 🗑️ Removed

- Fix for use with RF24Ethernet by \@TMRh20 in [`73b7971`](https://github.com/nRF24/RF24Mesh/commit/73b7971b833c8550ba2a64f0f7b6cfae8a769708)

[1.0.51]: https://github.com/nRF24/RF24Mesh/compare/v1.0.5...v1.0.51

Full commit diff: [`v1.0.5...v1.0.51`][1.0.51]

## [1.0.5] - 2016-03-27

### <!-- 1 --> 🚀 Added

- Add library.json for PlatformIO registry in [`fbd486c`](https://github.com/nRF24/RF24Mesh/commit/fbd486cb9d98f2e52d53243871280d32f6cb3d8c)
- Add deps to library.json for PlatformIO in [`c599c52`](https://github.com/nRF24/RF24Mesh/commit/c599c529c4033b0bf5800c62a2780bfec96dfb0e)
- Additional child node, fix warning, spidev issue by \@TMRh20 in [`a58a257`](https://github.com/nRF24/RF24Mesh/commit/a58a25785c46bbd368466b9cf584449f9930cbf6)

### <!-- 3 --> 🗑️ Removed

- Remove examples from library.json in [`09e2a96`](https://github.com/nRF24/RF24Mesh/commit/09e2a96c90606796d64ec4217b1070be823a8f73)

### <!-- 4 --> 🛠️ Fixed

- Fix for address renewal confirmation by \@TMRh20 in [`adabf7d`](https://github.com/nRF24/RF24Mesh/commit/adabf7d49be230c496e8e94cbb8c67cfb5463b16)
- Last change causing segfaults on non-master by \@TMRh20 in [`c874003`](https://github.com/nRF24/RF24Mesh/commit/c8740036eb25004643524541af5835ed535c5d06)

### <!-- 9 --> 🗨️ Changed

- RPI2 autodetection by \@yozik04 in [`91c0ba3`](https://github.com/nRF24/RF24Mesh/commit/91c0ba36e7148ba95ede67a879de7d16f69613ce)
- RPI2 autodetection in ncurses example by \@yozik04 in [`5e9c030`](https://github.com/nRF24/RF24Mesh/commit/5e9c03041c27dbaaf952244c5d42c59ff445fbc0)
- Merge pull request \#54 from yozik04/master by \@TMRh20 in [`62a4fa5`](https://github.com/nRF24/RF24Mesh/commit/62a4fa54c0ea8e501fc7b7d771fac25b383837f6)
- Initlialize addrListTop to 0 by \@julianpas in [`eff2775`](https://github.com/nRF24/RF24Mesh/commit/eff27756723d5c3f7fa171be43d0396e5eeafbdb)
- Merge pull request \#56 from julianpas/patch-1 by \@Avamander in [`601c605`](https://github.com/nRF24/RF24Mesh/commit/601c60526577f42c344753a9a36397aba90ef997)
- Merge pull request \#68 from harmon25/master by \@Avamander in [`206b733`](https://github.com/nRF24/RF24Mesh/commit/206b733961523b3c3cf52f1c153b38b4c58c1064)
- Use tagged source code from repo; fix include/exclude fields by \@ivankravets in [`aa17378`](https://github.com/nRF24/RF24Mesh/commit/aa1737807ab0cb435ac55a0eb0b5dc03348240e0)
- Merge pull request \#69 from ivankravets/patch-1 by \@Avamander in [`e573ebb`](https://github.com/nRF24/RF24Mesh/commit/e573ebba23231a8ada6ce0843ebcc2daf60ceefa)
- Major Bugfix: Buffer overrun in renewAddress by \@TMRh20 in [`0450c5c`](https://github.com/nRF24/RF24Mesh/commit/0450c5ca4aca71581d8ad61b3cff7cdc3dc42b6c)
- Update version due to major bugfix by \@TMRh20 in [`b420fbc`](https://github.com/nRF24/RF24Mesh/commit/b420fbc0d43536f534779030cd5ad1872eb2ce40)

[1.0.5]: https://github.com/nRF24/RF24Mesh/compare/v1.0.4-beta...v1.0.5

Full commit diff: [`v1.0.4-beta...v1.0.5`][1.0.5]

## New Contributors
* \@ivankravets made their first contribution
* \@ made their first contribution
* \@julianpas made their first contribution
* \@yozik04 made their first contribution
## [1.0.4-beta] - 2015-12-15

### <!-- 4 --> 🛠️ Fixed

- Fix setChild() per \@ricmail85 \#49 by \@TMRh20 in [`a4fcd06`](https://github.com/nRF24/RF24Mesh/commit/a4fcd06f58974fd5e80c3904614279c6f658122c)
- Fixes for address & nodeID lookups by \@TMRh20 in [`8f73c01`](https://github.com/nRF24/RF24Mesh/commit/8f73c013ee483154978848ad6241f73a91df5f7f)
- Fix setAddress, set default addy on renew fail by \@TMRh20 in [`39eaf27`](https://github.com/nRF24/RF24Mesh/commit/39eaf27f1039aecbe894d48c2c7302a9c54a39d1)

### <!-- 9 --> 🗨️ Changed

- Reduce wait for address confirmation by \@TMRh20 in [`1440149`](https://github.com/nRF24/RF24Mesh/commit/14401497d9b2d051b51b27bff03c9340a458589f)
- Copied wrong file... by \@TMRh20 in [`08d8a77`](https://github.com/nRF24/RF24Mesh/commit/08d8a7753e68477590de15d1eaba2c488c7cde8a)
- Debug info for address reqs. , improve addr renew by \@TMRh20 in [`590d4d6`](https://github.com/nRF24/RF24Mesh/commit/590d4d64e71c1331f5ccb8b867ccb313267c18df)
- Update RF24Mesh_Example_Node2NodeExtra.ino by \@Avamander in [`96a77af`](https://github.com/nRF24/RF24Mesh/commit/96a77afd2fc25d601fade7a94e5d324888e9b41c)
- Merge pull request \#25 from Avamander/patch-2 by \@Avamander in [`455697e`](https://github.com/nRF24/RF24Mesh/commit/455697efab4bb21406f2fe0237f389a9e1460fc0)
- Update library.properties by \@Avamander in [`170bfb1`](https://github.com/nRF24/RF24Mesh/commit/170bfb1c3f5093fb70dfe8345fdb4305ec777e4c)

[1.0.4-beta]: https://github.com/nRF24/RF24Mesh/compare/v1.0.3-beta...v1.0.4-beta

Full commit diff: [`v1.0.3-beta...v1.0.4-beta`][1.0.4-beta]

## [1.0.3-beta] - 2015-11-27

### <!-- 4 --> 🛠️ Fixed

- Fix static address assign, add new functions by \@TMRh20 in [`fd423d5`](https://github.com/nRF24/RF24Mesh/commit/fd423d531044f2d91012e3485699140313445204)

### <!-- 9 --> 🗨️ Changed

- Increment version by \@TMRh20 in [`ead3bac`](https://github.com/nRF24/RF24Mesh/commit/ead3bac660b939d4d9ecedb6f278b07efe969edd)

[1.0.3-beta]: https://github.com/nRF24/RF24Mesh/compare/v1.0.2-beta...v1.0.3-beta

Full commit diff: [`v1.0.2-beta...v1.0.3-beta`][1.0.3-beta]

## [1.0.2-beta] - 2015-11-06

### <!-- 3 --> 🗑️ Removed

- Improve address requests, docs update by \@TMRh20 in [`f7682e3`](https://github.com/nRF24/RF24Mesh/commit/f7682e3e96a9dff726dfbd4405564d1ab6b321f9)

[1.0.2-beta]: https://github.com/nRF24/RF24Mesh/compare/v1.0.1-beta...v1.0.2-beta

Full commit diff: [`v1.0.1-beta...v1.0.2-beta`][1.0.2-beta]

## [1.0.1-beta] - 2015-10-30

### <!-- 4 --> 🛠️ Fixed

- Fix return of getNodeID by \@TMRh20 in [`d49dea1`](https://github.com/nRF24/RF24Mesh/commit/d49dea100a1ec82a67c8fbf784b0a7c78d4a485f)

### <!-- 9 --> 🗨️ Changed

- Adj timeout, fix address release by \@TMRh20 in [`7d0548c`](https://github.com/nRF24/RF24Mesh/commit/7d0548c9755380241b7fe3144e54d0cd0a844afa)

[1.0.1-beta]: https://github.com/nRF24/RF24Mesh/compare/v1.0-beta...v1.0.1-beta

Full commit diff: [`v1.0-beta...v1.0.1-beta`][1.0.1-beta]

## [1.0-beta] - 2015-09-26

### <!-- 1 --> 🚀 Added

- Add initial cpp and h files by \@TMRh20 in [`0faa4fb`](https://github.com/nRF24/RF24Mesh/commit/0faa4fb6f0b66707db41d8b56361cadfb410c7fc)
- Addresssing and convergence changes by \@TMRh20 in [`8413448`](https://github.com/nRF24/RF24Mesh/commit/841344840dbd26de7410f54be09f22d90762ccb3)
- Add monitoring example to its own folder by \@TMRh20 in [`9c95b1e`](https://github.com/nRF24/RF24Mesh/commit/9c95b1ee28a9ecd9e23faa10c736413d50dfc400)
- Add getAddress() function, similar to DNS by \@TMRh20 in [`6f720f1`](https://github.com/nRF24/RF24Mesh/commit/6f720f1e2fa558df1e5a0d48f454147d7a86c594)
- Add releaseAddress(), change addr request process by \@TMRh20 in [`360f8a1`](https://github.com/nRF24/RF24Mesh/commit/360f8a1196601abddfb9a6338c15804a8bb85212)
- Add timeouts per \#23 by \@TMRh20 in [`1a9b0c9`](https://github.com/nRF24/RF24Mesh/commit/1a9b0c9e31bb29777ca388a1f8e00101aa68e77c)

### <!-- 3 --> 🗑️ Removed

- Remove retries from mesh.write, debug signal lvl by \@TMRh20 in [`f6f18a5`](https://github.com/nRF24/RF24Mesh/commit/f6f18a50340b86bae62d086a0dce4aab75d38dab)
- Remove EEPROM functionality, add MESH_NOMASTER by \@TMRh20 in [`bdd6694`](https://github.com/nRF24/RF24Mesh/commit/bdd6694b3b51501c4d5f9185f2b10ccb3588efee)
- Fixes/Updates for address lookups by \@TMRh20 in [`e5aab22`](https://github.com/nRF24/RF24Mesh/commit/e5aab2239561b53f5652a54f91915ac9db08654e)

### <!-- 4 --> 🛠️ Fixed

- Fix examples by \@TMRh20 in [`caaeed3`](https://github.com/nRF24/RF24Mesh/commit/caaeed3c9cfa6f6fd1ad29ccabf2e2ed9296c43b)
- Fix Serial printout of address by \@TMRh20 in [`aaa23b6`](https://github.com/nRF24/RF24Mesh/commit/aaa23b63ba8599ed93cb15d98c12ea6b012333ee)
- Fix errors in address assignments by \@TMRh20 in [`1e0f7b0`](https://github.com/nRF24/RF24Mesh/commit/1e0f7b0d5bd407395654551dbfecc0efb80b5617)
- Fix error, Use progmem for avr debug, max children by \@TMRh20 in [`75fe167`](https://github.com/nRF24/RF24Mesh/commit/75fe1671a67202b7783a8ce9d5c0187bd5f9e18c)
- Fix errors in address assignment, updates by \@TMRh20 in [`717b1e5`](https://github.com/nRF24/RF24Mesh/commit/717b1e5a5e3056123a2e24b1d36ede0976843db8)
- Fixes for addressing, add address confirmation by \@TMRh20 in [`359c77f`](https://github.com/nRF24/RF24Mesh/commit/359c77ff2a5fad65bbc434532b55dc57b5940875)
- Fix example Makefiles for non-RPi by \@TMRh20 in [`375867f`](https://github.com/nRF24/RF24Mesh/commit/375867f344a5790e9cb5029fc64d5a25ba3dfc16)

### <!-- 8 --> 📝 Documentation

- Minor fixes & changes by \@TMRh20 in [`9e7637b`](https://github.com/nRF24/RF24Mesh/commit/9e7637b9c06ead4b7802190e0474ef3ae78c23b7)

### <!-- 9 --> 🗨️ Changed

- Initial commit by \@TMRh20 in [`19f69b3`](https://github.com/nRF24/RF24Mesh/commit/19f69b30f552157cbed36e717fea716499f5aea8)
- Very Basic/Rough 'mesh' functionality by \@TMRh20 in [`0ff7dbc`](https://github.com/nRF24/RF24Mesh/commit/0ff7dbcf929c0824ebf03324620104a8aeeb4036)
- Small changes, add RPi example by \@TMRh20 in [`68fbdc1`](https://github.com/nRF24/RF24Mesh/commit/68fbdc126421792ffbf429b82f7eefab2a1f9534)
- Consolidate Arduino/RPi code, Small changes by \@TMRh20 in [`2f22181`](https://github.com/nRF24/RF24Mesh/commit/2f22181ec4678c5b2cf420a0538a61c8de3697b2)
- Minor fix, reduce address request times by \@TMRh20 in [`4dd5d74`](https://github.com/nRF24/RF24Mesh/commit/4dd5d743efb41adb62f10b4a5672c5fef431dd52)
- Clean up existing code by \@TMRh20 in [`0690413`](https://github.com/nRF24/RF24Mesh/commit/0690413514dff75b93e00fa17a6c832e6a79ed66)
- Small fixes by \@TMRh20 in [`4a90f22`](https://github.com/nRF24/RF24Mesh/commit/4a90f22ca803267c964ab1efb7f775b363b167ce)
- Revert one change, add print by \@TMRh20 in [`d940c5f`](https://github.com/nRF24/RF24Mesh/commit/d940c5f580a831d4420a258179e15097270d1e80)
- Changes to address requests by \@TMRh20 in [`72d1976`](https://github.com/nRF24/RF24Mesh/commit/72d197687238768a8741bd41d66e267244c359c1)
- Figure out why pstr not working by \@TMRh20 in [`8b584d8`](https://github.com/nRF24/RF24Mesh/commit/8b584d887b1fdf762a2bd7602a3475fa744fe52b)
- NCurses example for testing/monitoring mesh by \@TMRh20 in [`1e93922`](https://github.com/nRF24/RF24Mesh/commit/1e9392208c08a7479b3c63acac4b3e42e19242df)
- Enable AVR devices (Uno,Nano etc) as master node by \@TMRh20 in [`869bec1`](https://github.com/nRF24/RF24Mesh/commit/869bec149132c8d2f71c1606f2830113185345c3)
- Save nodes to file on RPi, reliable writes by \@TMRh20 in [`f4d4097`](https://github.com/nRF24/RF24Mesh/commit/f4d40971ec5843e2d7cf9ba099bebaa0d0b795b2)
- Minor changes, add setChannel() function by \@TMRh20 in [`ab4fbdf`](https://github.com/nRF24/RF24Mesh/commit/ab4fbdfe704921af573969544cf2ba89d8c68dfb)
- Change mesh.update to return header type by \@TMRh20 in [`0614f2f`](https://github.com/nRF24/RF24Mesh/commit/0614f2f08127a58476302a5f455b6039005a7f77)
- Static addresses, RPD, reduce delays in requests by \@TMRh20 in [`388da41`](https://github.com/nRF24/RF24Mesh/commit/388da41e438900d8aadf07aeeb01b8f248cb3fdb)
- Utilize new returnSysMsgs var in RF24Network by \@TMRh20 in [`5bd9f4d`](https://github.com/nRF24/RF24Mesh/commit/5bd9f4d2e783a4a2f204a7804d19bfe9277fa888)
- Intel Galileo Support by \@spaniakos in [`4ea6013`](https://github.com/nRF24/RF24Mesh/commit/4ea6013a91236755fff85f79f07fa76d5feb3600)
- Merge pull request \#4 from spaniakos/master by \@TMRh20 in [`9319f74`](https://github.com/nRF24/RF24Mesh/commit/9319f74789a68817e160c0766666e7b7bd2df749)
- Get nodeId from a network address by \@l3v5y in [`6a3f566`](https://github.com/nRF24/RF24Mesh/commit/6a3f5666cb9473079f29425eb6930adb62d1c7d1)
- Merge pull request \#5 from l3v5y/master by \@TMRh20 in [`21a9b15`](https://github.com/nRF24/RF24Mesh/commit/21a9b154289fa1d4bd8db2a37dd9366566d33073)
- Update Makefile for non-RPi Linux devices by \@TMRh20 in [`49c976c`](https://github.com/nRF24/RF24Mesh/commit/49c976cc36c257c15735d7527eb9ee0a41ccb3a3)
- Update Makefile with better RPi detection by \@TMRh20 in [`3642a3e`](https://github.com/nRF24/RF24Mesh/commit/3642a3e2a8606129534aa6b7adb0f093b7035dd4)
- Allow setting channel & datarate on mesh.begin() by \@TMRh20 in [`56f6afc`](https://github.com/nRF24/RF24Mesh/commit/56f6afc7e6bda1350d58635d87a895ab6fa5c32d)
- Update RF24Mesh_Example_Master_Statics.ino by \@Avamander in [`efa6651`](https://github.com/nRF24/RF24Mesh/commit/efa66517a559313516852bf2240a977ff432cfb9)
- Merge pull request \#12 from Avamander/patch-1 by \@TMRh20 in [`a70293d`](https://github.com/nRF24/RF24Mesh/commit/a70293dd1344d0cf919e4e2e7d27e3682b25c696)
- Wrong example by \@TMRh20 in [`84ee5bf`](https://github.com/nRF24/RF24Mesh/commit/84ee5bf425bb59e66cf7bc156931bf7a11e2ce72)
- Change timing in example by \@TMRh20 in [`c1d85be`](https://github.com/nRF24/RF24Mesh/commit/c1d85be97357ba4b90ac936b88aece0dc2efbfec)
- Use default channel for new examples by \@TMRh20 in [`a804703`](https://github.com/nRF24/RF24Mesh/commit/a804703de7e65e23c10bdf4d2300fcbcf28e965d)
- Updates for latest RF24Network changes and example by \@TMRh20 in [`f774d01`](https://github.com/nRF24/RF24Mesh/commit/f774d013600e00189e2a4c01ee5da905b8d6e967)
- Update RF24Mesh_Example_Master.ino by \@Avamander in [`7e7d34f`](https://github.com/nRF24/RF24Mesh/commit/7e7d34fc5a34a500b7588e91145df39a70fdd824)
- Merge pull request \#15 from Avamander/patch-2 by \@TMRh20 in [`35b68bf`](https://github.com/nRF24/RF24Mesh/commit/35b68bff6a7ffe2dc2257efadfbe3dddf2830476)
- Create RF24Mesh_Example_Master_To_Nodes.ino by \@Avamander in [`595bc1e`](https://github.com/nRF24/RF24Mesh/commit/595bc1e8320d7e4f8c1a7f5dc681913328cdfe4d)
- Update RF24Mesh_Example_Master_To_Nodes.ino by \@Avamander in [`148a957`](https://github.com/nRF24/RF24Mesh/commit/148a957ab8c2ca24dff5b133ff40bd8a521ac462)
- Merge pull request \#14 from Avamander/patch-1 by \@TMRh20 in [`b4b9825`](https://github.com/nRF24/RF24Mesh/commit/b4b9825ddde5552a6d064014bcbd9dcb505895af)
- Update RF24Mesh_Example.ino by \@Avamander in [`4a7ec5d`](https://github.com/nRF24/RF24Mesh/commit/4a7ec5d693b8ac07356c31f67c73062a5b0442a5)
- Merge pull request \#16 from Avamander/patch-3 by \@TMRh20 in [`ae46dbb`](https://github.com/nRF24/RF24Mesh/commit/ae46dbbd01e025e40b0647dd524a5071adf7df48)
- Update RF24Mesh_SerialConfig.ino by \@Avamander in [`cb61d1e`](https://github.com/nRF24/RF24Mesh/commit/cb61d1e4329b7c96ba212eb4188c8bda8193232a)
- Merge pull request \#17 from Avamander/patch-4 by \@TMRh20 in [`743684f`](https://github.com/nRF24/RF24Mesh/commit/743684f63b31050b3a652217ffb3fa86c957b83d)
- Update RF24Mesh_Example_Node2NodeExtra.ino by \@Avamander in [`b6cfd03`](https://github.com/nRF24/RF24Mesh/commit/b6cfd039dde63862dcbcad9cf938a2c41f351a4c)
- Merge pull request \#18 from Avamander/patch-5 by \@TMRh20 in [`a13c33e`](https://github.com/nRF24/RF24Mesh/commit/a13c33ec34747aa152d560da31642a59f9e2dac0)
- Update RF24Mesh_Example_Master_Statics.ino by \@Avamander in [`b5e6f9f`](https://github.com/nRF24/RF24Mesh/commit/b5e6f9f5f00edddeb7c8806bd4350123f27bf06b)
- Merge pull request \#19 from Avamander/patch-6 by \@TMRh20 in [`fbceb85`](https://github.com/nRF24/RF24Mesh/commit/fbceb85308f19e6c0ad3871cd33c45ffe08939b4)
- Create library.properties by \@Avamander in [`5af874f`](https://github.com/nRF24/RF24Mesh/commit/5af874fbc2aabf76e5cf12738aba3cc2d40824de)
- Merge pull request \#22 from Avamander/patch-1 by \@TMRh20 in [`b207da6`](https://github.com/nRF24/RF24Mesh/commit/b207da68626699376293241765b4d9a452c2ef95)

[1.0-beta]: https://github.com/nRF24/RF24Mesh/compare/19f69b30f552157cbed36e717fea716499f5aea8...v1.0-beta

Full commit diff: [`19f69b3...v1.0-beta`][1.0-beta]

## New Contributors
* \@TMRh20 made their first contribution
* \@l3v5y made their first contribution
* \@spaniakos made their first contribution
<!-- generated by git-cliff -->
