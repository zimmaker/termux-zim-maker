# Termux ZIM Maker

A tool for packaging a local HTML directory into a `.zim` file on **bare Termux** (no proot / no glibc / no Python / no Node).

- **Language:** C++17
- **Crawler:** false
- **PythonUsed:** false
- **NetworkAccess:** false
- **GPG:** `2DBE5326AA0C409B9E958C816E443E21F230376B`

---

## Features

- Recursively traverses all files in the specified directory
- Automatically detects common file extensions and sets MIME types
- Uses `index.html` or the first HTML file as the ZIM welcome page
- Builds a full-text search index (based on Xapian)
- Writes metadata: title, description, language, etc.

---

## Dependencies

Install at runtime:

```bash
pkg update
pkg install -y libzim kiwix-tools
```

> `clang` is only needed when compiling from source. Skip it if you use the prebuilt binary.

### About libzim (important)

- This tool depends on `libzim`, installed directly from the official Termux apt repo: `pkg install libzim`
- Termux's `libzim` is a **bionic-native aarch64** build (located at `/data/data/com.termux/files/usr/lib/libzim.so`). It loads directly on bare Termux — **no proot, no cross-compilation, no manylinux binary** required. (manylinux builds are glibc-based and will not run on Termux's bionic environment.)
- Verified versions: `libzim 9.8.1` (Termux apt), `kiwix-tools 3.8.2` (Termux apt)
- If your `pkg install libzim` version differs significantly from 9.8.1 and causes compilation errors, compare the headers under `/usr/include/zim/writer/` and adjust `make_zim.cpp` accordingly
- When compiling from source, ensure `pkg-config --cflags --libs libzim` outputs correctly

---

## Quick Start (Prebuilt Binary)

```bash
chmod +x make_zim
./make_zim --dir <html_folder> --out <output.zim> --title "Title" --desc "Description" --lang zh
```

---

## Build from Source

```bash
clang++ make_zim.cpp -o make_zim $(pkg-config --cflags --libs libzim) -std=c++17 -stdlib=libc++
```

---

## Command-Line Options

| Flag | Required | Description |
|---|---|---|
| `--dir` | ✅ | Path to the local HTML directory (recursively traversed) |
| `--out` | ✅ | Output ZIM filename |
| `--title` | ✅ | ZIM title (written to metadata) |
| `--desc` | ❌ | Description (default: `Created on Termux`) |
| `--lang` | ❌ | Language code (default: `eng`, e.g. `zh`) |

---

## Example

```bash
mkdir -p ~/myhtml/sub
echo '<html><body><h1>Home</h1><a href="sub/page.html">Sub Page</a></body></html>' > ~/myhtml/index.html
echo '<html><body><h1>Sub Page</h1><p>Hello ZIM</p></body></html>' > ~/myhtml/sub/page.html

./make_zim --dir ~/myhtml --out demo.zim --title "Demo Site" --lang zh

kiwix-manage demo.zim --show
kiwix-serve --port=8080 demo.zim
kiwix-search demo.zim "Hello"
```

---

## Verify / Browse the Generated ZIM

```bash
kiwix-manage output.zim --show
kiwix-serve --port=8080 output.zim
kiwix-search output.zim "keyword"
```

---

## Known Limitations

- All file contents are read into memory at once. For very large directories (GB-scale), consider switching to stream-based processing.
- Supports all Linux environments.

---

## Dependencies (Build)

You need the `libzim` development library and a C++ toolchain before building.

### Termux (Android)

```bash
pkg install clang libc++ libzim
```

### Alpine Linux

```bash
apk add clang libc++-dev pkgconf libzim-dev zlib-dev icu-dev xz-dev zstd-dev xapian-core-dev
```

### Debian / Ubuntu

```bash
apt install clang libc++-dev pkg-config libzim-dev
```

---

## Build

```bash
git clone https://github.com/zimmaker/termux-zim-maker.git
cd termux-zim-maker
./build.sh
```

Or manually:

```bash
clang++ make_zim.cpp -o make_zim -Ivendor $(pkg-config --cflags --libs libzim) -std=c++17
```

---

## Run

```bash
chmod +x ./make_zim
./make_zim
```

Output:

```
Usage:
  make_zim --dir <html_dir> --out <output.zim>
           --title <title> [--desc <desc>] [--lang <lang>]
```

---

## Environment

| Component | Version |
|---|---|
| Termux / Alpine / Debian / Ubuntu | latest |
| libzim | 9.8.1 (apt) |
| kiwix-tools | 3.8.2 (apt) |
| clang | Termux default |

---

## License

MIT
