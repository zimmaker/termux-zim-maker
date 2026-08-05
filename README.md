# Termux ZIM Maker
 
在裸 Termux（无 proot / 无 glibc / 无 Python / 无 Node）环境下，将本地 HTML 文件夹打包成 .zim 文件的工具。
 
### 功能：
- 递归遍历指定目录下的所有文件
- 自动识别常见文件扩展名并设置 MIME 类型
- 将 index.html 或第一个 HTML 文件设为 ZIM 欢迎页
- 生成全文索引（基于 Xapian）
- 写入标题、描述、语言等元数据
 
### 依赖安装：
pkg update
pkg install -y libzim kiwix-tools
（clang 仅在从源码编译时需要，直接用二进制可跳过）
 
### 关于 libzim（重要）：
- 本工具依赖 libzim，直接从 Termux 官方 apt 安装：pkg install libzim
- Termux 的 libzim 是 bionic 原生 aarch64 编译（位于 /data/data/com.termux/files/usr/lib/libzim.so），在裸 Termux 下可直接加载，不需要 proot、不需要自行交叉编译、不需要 manylinux 版二进制（manylinux 版基于 glibc，在 Termux 的 bionic 环境下无法运行）
- 已验证可用版本：libzim 9.8.1（Termux apt）、kiwix-tools 3.8.2（Termux apt）
- 如果 pkg install libzim 装到的版本与 9.8.1 差异较大导致编译失败，请对照 /usr/include/zim/writer/ 下的头文件调整 make_zim.cpp
- 从源码编译时，确保 pkg-config --cflags --libs libzim 能正常输出
 
## 直接使用编译好的二进制：
```
chmod +x make_zim
./make_zim --dir <html文件夹> --out <输出.zim> --title "标题" --desc "描述" --lang zhp
```
 
## 从源码编译：
```
clang++ make_zim.cpp -o make_zim $(pkg-config --cflags --libs libzim) -std=c++17 -stdlib=libc++
 ```
## 参数说明：
--dir   必填  本地 HTML 文件夹路径（递归遍历）
--out   必填  输出 ZIM 文件名
--title 必填  ZIM 标题（写入元数据）
--desc  可选  描述（默认 Created on Termux）
--lang  可选  语言代码（默认 eng，如 zh）
 
## 示例：
```
mkdir -p ~/myhtml/sub
echo '<html><body><h1>首页</h1><a href="sub/page.html">子页</a></body></html>' > ~/myhtml/index.html
echo '<html><body><h1>子页</h1><p>Hello ZIM</p></body></html>' > ~/myhtml/sub/page.html
./make_zim --dir ~/myhtml --out demo.zim --title "Demo站点" --lang zh
kiwix-manage demo.zim --show
kiwix-serve --port=8080 demo.zim
kiwix-search demo.zim "Hello" 
```
## 浏览 / 验证生成的 ZIM：
```
kiwix-manage output.zim --show
kiwix-serve --port=8080 output.zim
kiwix-search output.zim "关键词"
```
 
## 已知限制：
- 将所有文件内容一次性读入内存，超大目录（GB 级）建议改用流式处理
- 支持所有Linux环境

  ## 依赖

编译前需要安装 libzim 开发库和 C++ 编译工具链。

### Termux (Android)
```
pkg install clang libc++ libzim
```

### Alpine Linux
```
apk add clang libc++-dev pkgconf libzim-dev zlib-dev icu-dev xz-dev zstd-dev xapian-core-dev
```

### Debian / Ubuntu
```
apt install clang libc++-dev pkg-config libzim-dev
```

## 构建

```
git clone https://github.com/zimmaker/termux-zim-maker.git
cd termux-zim-maker
./build.sh 
```
# 或手动：
```
clang++ make_zim.cpp -o make_zim -Ivendor $(pkg-config --cflags --libs libzim) -std=c++17
 ```
## 运行
```
chmod +x ./make_zim
./make_zim
#Usage:
  make_zim --dir <html_dir> --out <output.zim>
           --title <title> [--desc <desc>] [--lang <lang>]
```
## 环境：
Termux/Alpine/Debian/Ubuntu   最新
libzim      9.8.1 (apt)
kiwix-tools 3.8.2 (apt)
clang       termux 默认
 
License: MIT

[helppage](https://zimmaker.github.io/help/)
