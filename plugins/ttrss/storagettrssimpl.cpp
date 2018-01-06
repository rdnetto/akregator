/*
    This file is part of Akregator.

    Copyright (C) 2005 Stanislav Karchebny <Stanislav.Karchebny@kdemail.net>
                  2005 Frank Osterfeld <osterfeld@kde.org>

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
#include "storagettrssimpl.h"
#include "feedstoragettrssimpl.h"

#include <QMap>
#include <QString>
#include <QStringList>
#include <QTimer>

#include <qdebug.h>
#include <QDir>
#include <QStandardPaths>

typedef void *VPTR;
class Akregator::Backend::StorageTTRSSImpl::StorageTTRSSImplPrivate
{
public:
    StorageTTRSSImplPrivate() : modified(false)
        /*, purl("url")
        , pFeedList("feedList")
        , pTagSet("tagSet")
        , punread("unread")
        , ptotalCount("totalCount")
        , plastFetch("lastFetch")
        */
    {
    }

    void *storage;
    Akregator::Backend::StorageTTRSSImpl *q;
    void* archiveView;
    bool autoCommit;
    bool modified;
    mutable QMap<QString, Akregator::Backend::FeedStorageTTRSSImpl *> feeds;
    QStringList feedURLs;
    VPTR purl, pFeedList, pTagSet;
    VPTR punread, ptotalCount, plastFetch;
    QString archivePath;

    void *feedListStorage;
    void *feedListView;

    Akregator::Backend::FeedStorageTTRSSImpl *createFeedStorage(const QString &url);
};

Akregator::Backend::StorageTTRSSImpl::StorageTTRSSImpl() : d(new StorageTTRSSImplPrivate)
{
    d->q = this;
    setArchivePath(QString());
}

Akregator::Backend::FeedStorageTTRSSImpl *Akregator::Backend::StorageTTRSSImpl::StorageTTRSSImplPrivate::createFeedStorage(const QString &url)
{
    throw "Not implemented";
}

Akregator::Backend::FeedStorage *Akregator::Backend::StorageTTRSSImpl::archiveFor(const QString &url)
{
    return d->createFeedStorage(url);
}

const Akregator::Backend::FeedStorage *Akregator::Backend::StorageTTRSSImpl::archiveFor(const QString &url) const
{
    return d->createFeedStorage(url);
}

void Akregator::Backend::StorageTTRSSImpl::setArchivePath(const QString &archivePath)
{
    if (archivePath.isNull()) { // if isNull, reset to default
        d->archivePath = defaultArchivePath();
    } else {
        d->archivePath = archivePath;
    }
}

QString Akregator::Backend::StorageTTRSSImpl::archivePath() const
{
    return d->archivePath;
}

QString Akregator::Backend::StorageTTRSSImpl::defaultArchivePath()
{
    const QString ret = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/akregator/Archive");
    QDir().mkpath(ret);
    return ret;
}

Akregator::Backend::StorageTTRSSImpl::~StorageTTRSSImpl()
{
    close();
    delete d;
    d = 0;
}

void Akregator::Backend::StorageTTRSSImpl::initialize(const QStringList &)
{
}

bool Akregator::Backend::StorageTTRSSImpl::open(bool autoCommit)
{
    throw "Not implemented";
}

bool Akregator::Backend::StorageTTRSSImpl::autoCommit() const
{
    return d->autoCommit;
}

bool Akregator::Backend::StorageTTRSSImpl::close()
{
    throw "Not implemented";
}

bool Akregator::Backend::StorageTTRSSImpl::commit()
{
    throw "Not implemented";
}

bool Akregator::Backend::StorageTTRSSImpl::rollback()
{
    throw "Not implemented";
}

int Akregator::Backend::StorageTTRSSImpl::unreadFor(const QString &url) const
{
    throw "Not implemented";
}

void Akregator::Backend::StorageTTRSSImpl::setUnreadFor(const QString &url, int unread)
{
    throw "Not implemented";
}

int Akregator::Backend::StorageTTRSSImpl::totalCountFor(const QString &url) const
{
    throw "Not implemented";
}

void Akregator::Backend::StorageTTRSSImpl::setTotalCountFor(const QString &url, int total)
{
    throw "Not implemented";
}

int Akregator::Backend::StorageTTRSSImpl::lastFetchFor(const QString &url) const
{
    throw "Not implemented";
}

void Akregator::Backend::StorageTTRSSImpl::setLastFetchFor(const QString &url, int lastFetch)
{
    throw "Not implemented";
}

void Akregator::Backend::StorageTTRSSImpl::markDirty()
{
    if (!d->modified) {
        d->modified = true;
        // commit changes after 3 seconds
        QTimer::singleShot(3000, this, &StorageTTRSSImpl::slotCommit);
    }
}

void Akregator::Backend::StorageTTRSSImpl::slotCommit()
{
    if (d->modified) {
        commit();
    }
    d->modified = false;
}

QStringList Akregator::Backend::StorageTTRSSImpl::feeds() const
{
    throw "not implemented";
}

void Akregator::Backend::StorageTTRSSImpl::add(Storage *source)
{
    QStringList feeds = source->feeds();
    QStringList::ConstIterator end(feeds.constEnd());

    for (QStringList::ConstIterator it = feeds.constBegin(); it != end; ++it) {
        FeedStorage *fa = archiveFor(*it);
        fa->add(source->archiveFor(*it));
    }
}

void Akregator::Backend::StorageTTRSSImpl::clear()
{
    throw "Not implemented";
}

void Akregator::Backend::StorageTTRSSImpl::storeFeedList(const QString &opmlStr)
{
    throw "Not implemented";
}

QString Akregator::Backend::StorageTTRSSImpl::restoreFeedList() const
{
    throw "Not implemented";
}

void Akregator::Backend::StorageTTRSSImpl::storeTagSet(const QString &xmlStr)
{
    throw "Not implemented";
}

QString Akregator::Backend::StorageTTRSSImpl::restoreTagSet() const
{
    throw "Not implemented";
}
