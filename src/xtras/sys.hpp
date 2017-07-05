#ifndef XTRAS_SYS_HPP_
#define XTRAS_SYS_HPP_

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <string>
#include <system_error>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

namespace xtras {
namespace sys {

using Error = std::system_error;
using FileDescriptor = int;

inline auto open(const std::string& path, int flags, int mode = 0) -> FileDescriptor;
inline auto creat(const std::string& path, int mode) -> FileDescriptor;
inline auto close(FileDescriptor fd) -> void;

inline auto read(FileDescriptor fd, void* buf, std::size_t count) -> std::size_t;
inline auto readahead(FileDescriptor fd, off64_t offset, std::size_t count) -> std::size_t;
inline auto write(FileDescriptor fd, const void* buf, std::size_t count) -> std::size_t;

inline auto fstat(FileDescriptor fd) -> struct stat;

inline auto mmap(void* addr, std::size_t length, int prot, int flags, FileDescriptor fd, off_t offset) -> void*;
inline auto munmap(const void *addr, std::size_t length) -> void;
inline auto madvise(void* addr, std::size_t length, int advice) -> void;

inline auto setenv(const std::string& key, const std::string& value, bool replace = true) -> void;

namespace posix {

    inline auto fallocate(FileDescriptor fd, off_t offset, off_t len) -> void;

} // namespace posix

namespace {

    auto throw_error(const std::string& what, int errnum) -> void
    {
        throw Error{ errnum, std::system_category(), what };
    }

    template<typename F, typename... Args>
    auto call_or_ret(F func, const std::string& fname, Args&&... args) -> decltype(std::declval<F>()(args...))
    {
        auto ret = func(std::forward<Args>(args)...);
        if (ret < 0)
            throw_error(fname, ret);

        return ret;
    }

    template<typename F, typename E, typename... Args>
    auto call_or_errno_ecode(F func, const std::string& fname, E ecode, Args&&... args) -> decltype(std::declval<F>()(args...))
    {
        auto ret = func(std::forward<Args>(args)...);
        if (ret == ecode)
            throw_error(fname, errno);

        return ret;
    }

    template<typename F, typename... Args>
    auto call_or_errno(F func, const std::string& fname, Args&&... args) -> decltype(std::declval<F>()(args...))
    {
        return call_or_errno_ecode(func, fname, -1, std::forward<Args>(args)...);
    }

} // namespace

namespace posix {

    auto fallocate(FileDescriptor fd, off_t offset, off_t len) -> void
    {
        call_or_ret(::posix_fallocate, "posix_fallocate", fd, offset, len);
    }

} // namespace posix

auto read(FileDescriptor fd, void* buf, size_t count) -> std::size_t
{
    return call_or_errno(::read, "read", fd, buf, count);
}

auto open(const std::string& path, int flags, int mode) -> FileDescriptor
{
    return call_or_errno(::open, "open:" + path, path.c_str(), flags, mode);
}

auto readahead(FileDescriptor fd, off64_t offset, std::size_t count) -> std::size_t
{
    return call_or_errno(::readahead, "readahead", fd, offset, count);
}

auto creat(const std::string& path, int mode) -> FileDescriptor
{
    return call_or_errno(::creat, "creat:" + path, path.c_str(), mode);
}

auto fstat(FileDescriptor fd) -> struct stat
{
    struct stat buf;
    call_or_errno(::fstat, "fstat", fd, &buf);
    return buf;
}

auto mmap(void* addr, std::size_t length, int prot, int flags, FileDescriptor fd, off_t offset) -> void*
{
    return call_or_errno_ecode(::mmap, "mmap", MAP_FAILED, addr, length, prot, flags, fd, offset);
}

auto madvise(void* addr, std::size_t length, int advice) -> void
{
    call_or_errno(::madvise, "madvise", addr, length, advice);
}

auto close(FileDescriptor fd) -> void
{
    call_or_errno(::close, "close", fd);
}

auto munmap(const void* addr, size_t length) -> void
{
    call_or_errno(::munmap, "munmap", const_cast<void*>(addr), length);
}

auto write(FileDescriptor fd, const void* buf, std::size_t count) -> std::size_t
{
    return call_or_errno(::write, "write", fd, buf, count);
}

auto setenv(const std::string& key, const std::string& value, bool replace) -> void
{
    call_or_errno(::setenv, "setenv", key.c_str(), value.c_str(), replace);
}

namespace detail {

    auto sigaction(int sig_id, const struct sigaction& sa_new, struct sigaction* sa_prev = nullptr) -> void
    {
        call_or_errno(::sigaction, "sigaction", sig_id, &sa_new, sa_prev);
    }

} // detail

auto sigaction(int sig_id, const struct sigaction& sa_new) -> void
{
    detail::sigaction(sig_id, sa_new);
}

auto sigaction(int sig_id, const struct sigaction& sa_new, struct sigaction& sa_prev) -> void
{
    detail::sigaction(sig_id, sa_new, &sa_prev);
}

auto pthread_sigmask(int sig_id, const sigset_t& mask) -> void
{
    auto res = ::pthread_sigmask(SIG_UNBLOCK, &mask, nullptr);

    if (res != 0)
        throw_error("pthread::sigmask", res);
}

} // namespace sys
} // namespace xtras

#endif // XTRAS_SYS_HPP_
