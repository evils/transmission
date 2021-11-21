/*
 * This file Copyright (C) 2009-2014 Mnemosyne LLC
 *
 * It may be used under the GNU GPL versions 2 or 3
 * or any future license endorsed by Mnemosyne LLC.
 *
 */

#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "transmission.h"

#include "error.h"
#include "error-types.h"
#include "file.h"
#include "session.h"
#include "torrent-metainfo.h"
#include "torrent.h"
#include "tr-assert.h"
#include "utils.h"

using namespace std::literals;

struct tr_ctor
{
public:
    explicit tr_ctor(tr_session* session);
    tr_session* session() const;

    void clearMetainfo();
    bool setMetainfo(std::string_view benc, tr_error** error = nullptr);
    bool setMetainfoFromMagnetLink(std::string_view magnet_link, tr_error** error = nullptr);
    bool setMetainfoFromFile(char const* filename, tr_error** error = nullptr);
    bool getInfo(tr_info& setme, tr_error** error = nullptr) const;
    std::optional<tr_torrent_metainfo> metainfo() const
    {
        return tm_;
    }

    void setFilePriorities(tr_file_index_t const* files, tr_file_index_t fileCount, tr_priority_t priority);
    void setFilesWanted(tr_file_index_t const* files, tr_file_index_t fileCount, bool wanted);

    void setDeleteSource(bool delete_source);
    bool getDeleteSource() const;

    void setPaused(tr_ctorMode mode, bool paused);
    std::optional<bool> paused(tr_ctorMode mode) const;

    void setPeerLimit(tr_ctorMode mode, uint16_t peer_limit);
    std::optional<uint16_t> peerLimit(tr_ctorMode mode) const;

    void setDownloadDir(tr_ctorMode mode, char const* directory);
    std::optional<std::string_view> downloadDir(tr_ctorMode mode) const;

    void setIncompleteDir(std::string_view directory);
    std::optional<std::string_view> incompleteDir() const;

    void setBandwidthPriority(tr_priority_t priority);
    tr_priority_t bandwidthPriority() const;

    std::string_view contents() const;
    std::string_view sourceFile() const;

private:
    static bool isPriority(int i);

    struct optional_args
    {
        std::optional<bool> paused;
        std::optional<uint16_t> peer_limit;
        std::string download_dir;
    };

    tr_session* const session_;
    std::optional<bool> delete_source_;

    tr_priority_t priority_ = TR_PRI_NORMAL;
    std::optional<tr_torrent_metainfo> tm_;

    struct optional_args optional_args_[2];

    std::unordered_set<tr_file_index_t> not_wanted_;
    std::unordered_map<tr_file_index_t, tr_priority_t> priorities_;

    std::vector<char> contents_;

    std::string source_file_;
    std::string incomplete_dir_;
};
