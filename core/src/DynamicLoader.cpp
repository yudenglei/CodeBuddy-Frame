#include <string>
#include <vector>
#include <iostream>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  include <filesystem>
   namespace fs = std::filesystem;
#else
#  include <dlfcn.h>
#  include <filesystem>
   namespace fs = std::filesystem;
#endif

/// @brief 跨平台动态库句柄（内部类型）
struct LibHandle {
#ifdef _WIN32
    HMODULE handle{nullptr};
#else
    void* handle{nullptr};
#endif
};

LibHandle* dynamicLoad(const std::string& path) {
    auto* lh = new LibHandle();
#ifdef _WIN32
    // 转换为宽字符以支持路径中的Unicode
    int wlen = MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, nullptr, 0);
    std::wstring wpath(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, &wpath[0], wlen);
    lh->handle = LoadLibraryW(wpath.c_str());
    if (!lh->handle) {
        DWORD err = GetLastError();
        std::cerr << "[DynamicLoader] LoadLibrary failed, error=" << err << " path=" << path << "\n";
        delete lh;
        return nullptr;
    }
#else
    lh->handle = dlopen(path.c_str(), RTLD_LAZY | RTLD_LOCAL);
    if (!lh->handle) {
        std::cerr << "[DynamicLoader] dlopen failed: " << dlerror() << "\n";
        delete lh;
        return nullptr;
    }
#endif
    return lh;
}

void* dynamicSymbol(LibHandle* handle, const std::string& symbol) {
    if (!handle) return nullptr;
#ifdef _WIN32
    return reinterpret_cast<void*>(GetProcAddress(handle->handle, symbol.c_str()));
#else
    return dlsym(handle->handle, symbol.c_str());
#endif
}

void dynamicUnload(LibHandle* handle) {
    if (!handle) return;
#ifdef _WIN32
    if (handle->handle) FreeLibrary(handle->handle);
#else
    if (handle->handle) dlclose(handle->handle);
#endif
    delete handle;
}

/// @brief 扫描目录，返回所有指定扩展名的文件路径
std::vector<std::string> scanDirectory(const std::string& dir, const std::string& ext) {
    std::vector<std::string> result;
    std::error_code ec;
    if (!fs::exists(dir, ec) || !fs::is_directory(dir, ec)) {
        std::cerr << "[DynamicLoader] Plugin directory not found: " << dir << "\n";
        return result;
    }
    for (const auto& entry : fs::directory_iterator(dir, ec)) {
        if (entry.is_regular_file(ec) && entry.path().extension() == ext) {
            result.push_back(entry.path().string());
        }
    }
    return result;
}
