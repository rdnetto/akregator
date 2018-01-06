/*
    This file is part of Akregator.

    Copyright (C) 2005 Frank Osterfeld <osterfeld@kde.org>

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

#include "feedstoragettrssimpl.h"
#include "storagettrssimpl.h"

#include <Syndication/DocumentSource>
#include <Syndication/Global>
#include <Syndication/Feed>
#include <Syndication/Item>

#include <qdom.h>
#include <QFile>
#include <qdebug.h>
#include <QStandardPaths>

namespace {
static uint calcHash(const QString &str)
{
    if (str.isNull()) { // handle null string as "", prevents crash
        return calcHash(QLatin1String(""));
    }
    const char *s = str.toLatin1();
    uint hash = 5381;
    int c;
    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c;    // hash*33 + c
    }
    return hash;
}
}

namespace Akregator {
namespace Backend {
    typedef void *VPTR;
class FeedStorageTTRSSImpl::FeedStorageTTRSSImplPrivate
{
public:
    FeedStorageTTRSSImplPrivate()
        : modified(false)
        /*
        , pguid("guid")
        , ptitle("title")
        , pdescription("description")
        , pcontent("content")
        , plink("link")
        , pcommentsLink("commentsLink")
        , ptag("tag")
        , pEnclosureType("enclosureType")
        , pEnclosureUrl("enclosureUrl")
        , pcatTerm("catTerm")
        , pcatScheme("catScheme")
        , pcatName("catName")
        , pauthorName("authorName")
        , pauthorUri("authorUri")
        , pauthorEMail("authorEMail")
        , phash("hash")
        , pguidIsHash("guidIsHash")
        , pguidIsPermaLink("guidIsPermaLink")
        , pcomments("comments")
        , pstatus("status")
        , ppubDate("pubDate")
        , pHasEnclosure("hasEnclosure")
        , pEnclosureLength("enclosureLength")
        , ptags("tags")
        , ptaggedArticles("taggedArticles")
        , pcategorizedArticles("categorizedArticles")
        , pcategories("categories")
        */
    {
    }

    QString url;
    void *storage;
    StorageTTRSSImpl *mainStorage;
    void *archiveView;

    bool autoCommit;
    bool modified;
    bool convert;
    QString oldArchivePath;
    VPTR pguid, ptitle, pdescription, pcontent, plink, pcommentsLink, ptag, pEnclosureType, pEnclosureUrl, pcatTerm, pcatScheme, pcatName, pauthorName, pauthorUri, pauthorEMail;
    VPTR phash, pguidIsHash, pguidIsPermaLink, pcomments, pstatus, ppubDate, pHasEnclosure, pEnclosureLength;
    VPTR ptags, ptaggedArticles, pcategorizedArticles, pcategories;
};

void FeedStorageTTRSSImpl::convertOldArchive()
{
    if (!d->convert) {
        return;
    }

    d->convert = false;
    QFile file(d->oldArchivePath);

    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    Syndication::DocumentSource src(file.readAll(), QStringLiteral("http://foo"));
    file.close();
    Syndication::FeedPtr feed = Syndication::parse(src);

    if (feed) {
        //QList<Syndication::ItemPtr> items = feed->items();
        //QList<Syndication::ItemPtr>::ConstIterator it = items.constBegin();
        //QList<Syndication::ItemPtr>::ConstIterator en = items.constEnd();
        markDirty();
        commit();
    }
}

FeedStorageTTRSSImpl::FeedStorageTTRSSImpl(const QString &url, StorageTTRSSImpl *main)
{
    throw "Not implemented";
}

FeedStorageTTRSSImpl::~FeedStorageTTRSSImpl()
{
    // TODO
    //delete d->storage;
    delete d;
    d = 0;
}

void FeedStorageTTRSSImpl::markDirty()
{
    if (!d->modified) {
        d->modified = true;
        // Tell this to mainStorage
        d->mainStorage->markDirty();
    }
}

void FeedStorageTTRSSImpl::commit()
{
    throw "not implemented";
}

void FeedStorageTTRSSImpl::rollback()
{
    throw "not implemented";
}

void FeedStorageTTRSSImpl::close()
{
    if (d->autoCommit) {
        commit();
    }
}

int FeedStorageTTRSSImpl::unread() const
{
    return d->mainStorage->unreadFor(d->url);
}

void FeedStorageTTRSSImpl::setUnread(int unread)
{
    d->mainStorage->setUnreadFor(d->url, unread);
}

int FeedStorageTTRSSImpl::totalCount() const
{
    return d->mainStorage->totalCountFor(d->url);
}

void FeedStorageTTRSSImpl::setTotalCount(int total)
{
    d->mainStorage->setTotalCountFor(d->url, total);
}

int FeedStorageTTRSSImpl::lastFetch() const
{
    return d->mainStorage->lastFetchFor(d->url);
}

void FeedStorageTTRSSImpl::setLastFetch(int lastFetch)
{
    d->mainStorage->setLastFetchFor(d->url, lastFetch);
}

QStringList FeedStorageTTRSSImpl::articles(const QString &tag) const
{
    throw "not implemented";
}

QStringList FeedStorageTTRSSImpl::articles(const Category &cat) const
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::addEntry(const QString &guid)
{
    throw "Not implemented";
}

bool FeedStorageTTRSSImpl::contains(const QString &guid) const
{
    return findArticle(guid) != -1;
}

int FeedStorageTTRSSImpl::findArticle(const QString &guid) const
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::deleteArticle(const QString &guid)
{
    throw "not implemented";
}

int FeedStorageTTRSSImpl::comments(const QString &guid) const
{
    throw "not implemented";
}

QString FeedStorageTTRSSImpl::commentsLink(const QString &guid) const
{
    throw "not implemented";
}

bool FeedStorageTTRSSImpl::guidIsHash(const QString &guid) const
{
    throw "not implemented";
}

bool FeedStorageTTRSSImpl::guidIsPermaLink(const QString &guid) const
{
    throw "not implemented";
}

uint FeedStorageTTRSSImpl::hash(const QString &guid) const
{
    throw "not implemented";
}

void FeedStorageTTRSSImpl::setDeleted(const QString &guid)
{
    throw "Not implemented";
}

QString FeedStorageTTRSSImpl::link(const QString &guid) const
{
    throw "not implemented";
}

uint FeedStorageTTRSSImpl::pubDate(const QString &guid) const
{
    throw "not implemented";
}

int FeedStorageTTRSSImpl::status(const QString &guid) const
{
    throw "not implemented";
}

void FeedStorageTTRSSImpl::setStatus(const QString &guid, int status)
{
    throw "Not implemented";
}

QString FeedStorageTTRSSImpl::title(const QString &guid) const
{
    throw "not implemented";
}

QString FeedStorageTTRSSImpl::description(const QString &guid) const
{
    throw "not implemented";
}

QString FeedStorageTTRSSImpl::content(const QString &guid) const
{
    throw "not implemented";
}

void FeedStorageTTRSSImpl::setPubDate(const QString &guid, uint pubdate)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setGuidIsHash(const QString &guid, bool isHash)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setLink(const QString &guid, const QString &link)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setHash(const QString &guid, uint hash)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setTitle(const QString &guid, const QString &title)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setDescription(const QString &guid, const QString &description)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setContent(const QString &guid, const QString &content)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setAuthorName(const QString &guid, const QString &author)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setAuthorUri(const QString &guid, const QString &author)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setAuthorEMail(const QString &guid, const QString &author)
{
    throw "Not implemented";
}

QString FeedStorageTTRSSImpl::authorName(const QString &guid) const
{
    throw "not implemented";
}

QString FeedStorageTTRSSImpl::authorUri(const QString &guid) const
{
    throw "not implemented";
}

QString FeedStorageTTRSSImpl::authorEMail(const QString &guid) const
{
    throw "not implemented";
}

void FeedStorageTTRSSImpl::setCommentsLink(const QString &guid, const QString &commentsLink)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setComments(const QString &guid, int comments)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::setGuidIsPermaLink(const QString &guid, bool isPermaLink)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::addCategory(const QString &guid, const Category &cat)
{
    throw "Not implemented";
}

QList<Category> FeedStorageTTRSSImpl::categories(const QString &guid) const
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::addTag(const QString &guid, const QString &tag)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::removeTag(const QString &guid, const QString &tag)
{
    throw "Not implemented";
}

QStringList FeedStorageTTRSSImpl::tags(const QString &guid) const
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::add(FeedStorage *source)
{
    QStringList articles = source->articles();
    for (QStringList::ConstIterator it = articles.constBegin(); it != articles.constEnd(); ++it) {
        copyArticle(*it, source);
    }
    setUnread(source->unread());
    setLastFetch(source->lastFetch());
    setTotalCount(source->totalCount());
}

void FeedStorageTTRSSImpl::copyArticle(const QString &guid, FeedStorage *source)
{
    if (!contains(guid)) {
        addEntry(guid);
    }
    setComments(guid, source->comments(guid));
    setCommentsLink(guid, source->commentsLink(guid));
    setDescription(guid, source->description(guid));
    setGuidIsHash(guid, source->guidIsHash(guid));
    setGuidIsPermaLink(guid, source->guidIsPermaLink(guid));
    setHash(guid, source->hash(guid));
    setLink(guid, source->link(guid));
    setPubDate(guid, source->pubDate(guid));
    setStatus(guid, source->status(guid));
    setTitle(guid, source->title(guid));
    setAuthorName(guid, source->authorName(guid));
    setAuthorUri(guid, source->authorUri(guid));
    setAuthorEMail(guid, source->authorEMail(guid));

    QStringList tags = source->tags(guid);
    for (QStringList::ConstIterator it = tags.constBegin(); it != tags.constEnd(); ++it) {
        addTag(guid, *it);
    }
}

void FeedStorageTTRSSImpl::setEnclosure(const QString &guid, const QString &url, const QString &type, int length)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::removeEnclosure(const QString &guid)
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::enclosure(const QString &guid, bool &hasEnclosure, QString &url, QString &type, int &length) const
{
    throw "Not implemented";
}

void FeedStorageTTRSSImpl::clear()
{
    throw "Not implemented";
}
} // namespace Backend
} // namespace Akregator
