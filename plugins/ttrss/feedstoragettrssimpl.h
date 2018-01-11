/*
    This file is part of Akregator.

    Copyright (c) 2018 Reuben D'Netto <rdnetto@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

    As a special exception, permission is given to link this program
    with any edition of Qt, and distribute the resulting executable,
    without including the source code for Qt in the source distribution.
*/
#ifndef AKREGATOR_BACKEND_FEEDSTORAGETTRSSIMPL_H
#define AKREGATOR_BACKEND_FEEDSTORAGETTRSSIMPL_H

#include "feedstorage.h"
namespace Akregator {
namespace Backend {
class StorageTTRSSImpl;
class FeedStorageTTRSSImpl : public FeedStorage
{
public:
    FeedStorageTTRSSImpl(const QString &url, StorageTTRSSImpl *main);
    ~FeedStorageTTRSSImpl();

    void add(FeedStorage *source) override;
    void copyArticle(const QString &guid, FeedStorage *source) override;
    void clear() override;

    int unread() const override;
    void setUnread(int unread) override;
    int totalCount() const override;
    int lastFetch() const override;
    void setLastFetch(int lastFetch) override;

    QStringList articles(const QString &tag = QString()) const override;

    QStringList articles(const Category &cat) const override;

    bool contains(const QString &guid) const override;
    void addEntry(const QString &guid) override;
    void deleteArticle(const QString &guid) override;
    int comments(const QString &guid) const override;
    QString commentsLink(const QString &guid) const override;
    void setCommentsLink(const QString &guid, const QString &commentsLink) override;
    void setComments(const QString &guid, int comments) override;
    bool guidIsHash(const QString &guid) const override;
    void setGuidIsHash(const QString &guid, bool isHash) override;
    bool guidIsPermaLink(const QString &guid) const override;
    void setGuidIsPermaLink(const QString &guid, bool isPermaLink) override;
    uint hash(const QString &guid) const override;
    void setHash(const QString &guid, uint hash) override;
    void setDeleted(const QString &guid) override;
    QString link(const QString &guid) const override;
    void setLink(const QString &guid, const QString &link) override;
    uint pubDate(const QString &guid) const override;
    void setPubDate(const QString &guid, uint pubdate) override;
    int status(const QString &guid) const override;
    void setStatus(const QString &guid, int status) override;
    QString title(const QString &guid) const override;
    void setTitle(const QString &guid, const QString &title) override;
    QString description(const QString &guid) const override;
    void setDescription(const QString &guid, const QString &description) override;
    QString content(const QString &guid) const override;
    void setContent(const QString &guid, const QString &content) override;

    void setEnclosure(const QString &guid, const QString &url, const QString &type, int length) override;
    void removeEnclosure(const QString &guid) override;
    void enclosure(const QString &guid, bool &hasEnclosure, QString &url, QString &type, int &length) const override;

    void addTag(const QString &guid, const QString &tag) override;
    void removeTag(const QString &guid, const QString &tag) override;
    QStringList tags(const QString &guid = QString()) const override;

    void addCategory(const QString &guid, const Category &category) override;
    QList<Category> categories(const QString &guid = QString()) const override;

    void setAuthorName(const QString &guid, const QString &name) override;
    void setAuthorUri(const QString &guid, const QString &uri) override;
    void setAuthorEMail(const QString &guid, const QString &email) override;

    QString authorName(const QString &guid) const override;
    QString authorUri(const QString &guid) const override;
    QString authorEMail(const QString &guid) const override;

    void close() override;
    void commit() override;
    void rollback() override;

    void convertOldArchive() override;
private:
    void markDirty();
    /** finds article by guid, returns -1 if not in archive **/
    int findArticle(const QString &guid) const;
    void setTotalCount(int total);
    class FeedStorageTTRSSImplPrivate;
    FeedStorageTTRSSImplPrivate *d;
};
} // namespace Backend
} // namespace Akregator

#endif // AKREGATOR_BACKEND_FEEDSTORAGETTRSSIMPL_H
