# Changelog

All notable changes to this project will be documented in this file.

## [1.1.0] - 2026-07-08

### Added
- Qt GUI now displays search results as thumbnail cards (image preview, file path, similarity score) in a scrollable grid.
- Search results exclude the query image itself, preventing it from always ranking first with similarity 1.0.

### Changed
- Source package root directory renamed to `MiniImageRetrieval` (ASCII) to avoid CMake/MinGW path issues with non-ASCII characters.
- Updated `search_similar()` API in `SearcherDLL` to accept an optional `exclude_path` parameter.

### Fixed
- Query image preview is now cleared/re-labeled when the selected image cannot be loaded.

## [1.0.0] - 2026-07-07

### Added
- Initial release of MiniImageRetrieval.
- Qt6-based GUI for loading a local image gallery and searching by image.
- Modular DLL architecture: ImageLoader, FeatureExtractor, Indexer, Searcher.
- Multi-feature image representation:
  - HSV color histogram (48-D)
  - Uniform LBP texture histogram (59-D)
  - Hu moment invariants (7-D)
- Weighted cosine-similarity retrieval with configurable weights via `set_search_weights()`.
- Example images under `test_images/`.
- Build scripts: `build.bat`, `launch.bat`, `launch.ps1`.
- CMake post-build copy of project DLLs and MinGW runtime libraries.

### Notes
- Total feature dimension: 114.
- Default retrieval weights: color 0.35, texture 0.35, shape 0.30.
