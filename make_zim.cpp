#include <zim/writer/creator.h>
#include <zim/writer/item.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

std::string mimeOf(const std::string& ext) {
    if (ext == "html" || ext == "htm") return "text/html";
    if (ext == "css")  return "text/css";
    if (ext == "js")   return "application/javascript";
    if (ext == "png")  return "image/png";
    if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
    if (ext == "gif")  return "image/gif";
    if (ext == "svg")  return "image/svg+xml";
    if (ext == "txt")  return "text/plain";
    if (ext == "json") return "application/json";
    if (ext == "pdf")  return "application/pdf";
    return "application/octet-stream";
}

std::string readFile(const fs::path& p) {
    std::ifstream f(p, std::ios::binary);
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

std::string toZimPath(const fs::path& rel) {
    std::string p = rel.generic_string();
    if (!p.empty() && p[0] == '/') p = p.substr(1);
    return p;
}

std::string getOpt(int argc, char** argv, const std::string& key, const std::string& def) {
    for (int i = 1; i < argc - 1; i++)
        if (key == argv[i]) return argv[i + 1];
    return def;
}

int main(int argc, char** argv) {
    std::string dir   = getOpt(argc, argv, "--dir",   "");
    std::string out   = getOpt(argc, argv, "--out",   "");
    std::string title = getOpt(argc, argv, "--title", "My ZIM");
    std::string desc  = getOpt(argc, argv, "--desc",  "Created on Termux");
    std::string lang  = getOpt(argc, argv, "--lang",  "eng");

    if (dir.empty() || out.empty()) {
        std::cerr << "Usage:\n"
                  << "  make_zim --dir <html_dir> --out <output.zim>\n"
                  << "           --title <title> [--desc <desc>] [--lang <lang>]\n";
        return 1;
    }
    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        std::cerr << "Not a directory: " << dir << "\n";
        return 1;
    }

    zim::writer::Creator creator;
    creator.configIndexing(true, lang);
    creator.startZimCreation(out);

    zim::writer::Hints hints;
    std::string mainPath = "";
    int count = 0;

    for (auto& e : fs::recursive_directory_iterator(dir)) {
        if (!fs::is_regular_file(e)) continue;
        fs::path fp = e.path();
        std::string ext = fp.extension().string();
        if (!ext.empty()) ext = ext.substr(1);

        auto rel = fs::relative(fp, dir);
        std::string zpath = toZimPath(rel);
        std::string content = readFile(fp);
        std::string mime = mimeOf(ext);
        std::string itemTitle = rel.stem().string();

        auto item = zim::writer::StringItem::create(zpath, mime, itemTitle, hints, content);
        creator.addItem(item);
        count++;
        std::cout << "Added: " << zpath << " (" << mime << ")\n";

        if (zpath == "index.html") mainPath = "index.html";
    }

    // 如果没有 index.html，找第一个 html 作为欢迎页
    if (mainPath.empty()) {
        for (auto& e : fs::recursive_directory_iterator(dir)) {
            if (!fs::is_regular_file(e)) continue;
            std::string ext = e.path().extension().string();
            if (ext == ".html" || ext == ".htm") {
                mainPath = toZimPath(fs::relative(e.path(), dir));
                break;
            }
        }
    }
    if (!mainPath.empty()) {
        creator.setMainPath(mainPath);
        std::cout << "Main entry: " << mainPath << "\n";
    }

    creator.addMetadata("Title", title);
    creator.addMetadata("Description", desc);
    creator.addMetadata("Language", lang);
    creator.finishZimCreation();

    std::cout << "Done. " << count << " items -> " << out << "\n";
    return 0;
}